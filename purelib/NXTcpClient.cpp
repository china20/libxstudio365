#include "NXTcpClient.h"
#include "NXMacroDefs.h"

#define HAS_SELECT_OP 0

#ifdef _WIN32
#define msleep(msec) Sleep((msec))
#define sleep(sec) msleep((sec) * 1000)
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
#else
#define msleep(msec) usleep((msec) * 1000)
#endif

#define RECONNECT_DELAY 5
#define TIMER_DURATION 100 // 100 miliseconds

#define MAX_PACKET_LEN 16384

using namespace inet;

namespace inet {
    class TcpSendingPacket : public cocos2d::Ref
    {
    public:
        TcpSendingPacket(std::vector<char>&& right, const TcpClient::OnPacketSendCallback& callback)
        {
            data = std::move(right);
            onSend = callback;
            timestamp = std::clock();
        }
        std::vector<char>      data;
        TcpClient::OnPacketSendCallback onSend;
        long                   timestamp;
    };
}

TcpClient::TcpClient() : bAppExiting(false),
threadStarted(false),
address("192.168.1.107"),
port(8001),
connected(false),
timeoutForConnect(5),
timeoutForSend(10),
decodePacketLength(nullptr),
offset(0),
error(ErrorCode::ERR_OK),
expectedPacketLength(-1),
idle(true),
reconnClock(-RECONNECT_DELAY * CLOCKS_PER_SEC) // confirm connect immediately when program startup
{
}

TcpClient::~TcpClient()
{
    bAppExiting = true;

    close();

    while (threadStarted) {
        msleep(1);
    }
}

void TcpClient::setEndpoint(const char* address, u_short port)
{
    this->address = address;
    this->port = port;
}

bool TcpClient::init()
{
    if (!threadStarted) {
        threadStarted = true;

        this->scheduleCollectResponse();    //

        std::thread t([this]{

            this->service();

            threadStarted = false;
        });

        t.detach();
    }

    return true;
}

void TcpClient::close()
{
    if (impl.is_open())
        impl.close();
}

void TcpClient::handleError(void)
{
    reconnClock = clock();

    impl.close();
    connected = false;

    if (this->connectionLostListener != nullptr) {
        CCRUNONGL([this]{
            connectionLostListener();
        });
    }
}

void TcpClient::setConnectionEstablishedListener(const std::function<void()>& callback)
{
    this->connectionEstablishedListener = callback;
}

void TcpClient::setConnectionLostListener(const std::function<void()>& callback)
{
    this->connectionLostListener = callback;
}

void TcpClient::setDecodeLengthFunc(DecodePacketLengthFunc func)
{
    decodePacketLength = func;
}

void TcpClient::setOnRecvListener(const OnPacketRecvCallback& callback)
{
    onRecvPacket = callback;
}

void TcpClient::send(std::vector<char>&& data, const OnPacketSendCallback& callback)
{
    auto pdu = new TcpSendingPacket(std::move(data), callback);

    std::unique_lock<std::mutex> autolock(sendQueueMtx);
    sendQueue.push(pdu);   //消息队列
}

void TcpClient::moveReceivedPacket()
{
    std::unique_lock<std::mutex> autolock(recvQueueMtx);
    recvQueue.push(std::move(this->recvingPacket));
    autolock.unlock();

    expectedPacketLength = -1;

    CCSCHTASKS->resumeTarget(this);
}

bool TcpClient::connect(void)
{
    impl.open();
    int ret = impl.connect_n(this->address.c_str(), this->port, this->timeoutForConnect);
    if (ret == 0)
    {
#if HAS_SELECT_OP == 1
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&excepfds);

        auto fd = impl.native_handle();
        FD_SET(fd, &readfds);
        FD_SET(fd, &writefds);
        FD_SET(fd, &excepfds);
#endif

        impl.set_nonblocking(true);

        expectedPacketLength = -1;
        error = ErrorCode::ERR_OK;
        offset = 0;
        recvingPacket.clear();

        cocos2d::log("connect server: %s:%u succeed.", address.c_str(), port);

        if (this->connectionEstablishedListener != nullptr) {
            CCRUNONGL([this]{
                connectionEstablishedListener();
            });
        }

        return true;
    }
    else {
        int ec = xxsocket::get_last_errno();
        cocos2d::log("connect server: %s:%u failed, error code:%d, error msg:%s!", address.c_str(), port, ec, xxsocket::get_error_msg(ec));
    }

    return false;
}

void TcpClient::service()
{
#ifdef _WIN32
    timeBeginPeriod(1);
#endif

    while (!bAppExiting) {

        bool connection_ok = impl.is_open();

        auto elapsed = (clock() - reconnClock);
        if (!connection_ok && elapsed > (RECONNECT_DELAY * CLOCKS_PER_SEC))
        {
            cocos2d::log("connecting server %s:%u...", address.c_str(), port);
            connected = connect();
            if (!connected) {
                CCRUNONGL([this]{
                    cocos2d::log("connect server %s:%u failed", this->address.c_str(), this->port);
                });
                // sleep(RECONNECT_DELAY);
                reconnClock = clock();
                continue;
            }
        }

        if (!connection_ok) {
            msleep(1);
            continue;
        }
        // event loop
#if HAS_SELECT_OP == 1
        fd_set read_set, write_set, excep_set;
        timeval timeout;
#endif

        for (; !bAppExiting;)
        {
            idle = true;

#if HAS_SELECT_OP == 1
            timeout.tv_sec = 5 * 60; // 5 minutes
            timeout.tv_usec = 0;     // 10 milliseconds

            memcpy(&read_set, &readfds, sizeof(fd_set));
            memcpy(&write_set, &writefds, sizeof(fd_set));
            memcpy(&excep_set, &excepfds, sizeof(fd_set));

            int nfds = ::select(1, &read_set, &write_set, &excep_set, &timeout);

            if (nfds == -1)
            {
                // log("select failed, error code: %d\n", GetLastError());
                msleep(TIMER_DURATION);
                continue;            // select again
            }

            if (nfds == 0)
            {
                continue;
            }
#endif

#if HAS_SELECT_OP == 1
            if (FD_ISSET(impl, &write_set))
            { // do send
#endif
                if (!doWrite())
                    break;
#if HAS_SELECT_OP == 1
            }
#endif

#if HAS_SELECT_OP == 1
            if (FD_ISSET(impl, &read_set))
            { // do read
#endif
                if (!doRead())
                    break;

#if HAS_SELECT_OP == 1
                idle = false;
            }
#endif

#if HAS_SELECT_OP == 1
            if (FD_ISSET(impl, &excep_set))
            { // do close
                handleError();
                break; // end loop, try next connect
            }
#endif

            // Avoid high Occupation CPU
            if (idle) {
                msleep(1);
            }

            // L_Break:;
        }
    }

#ifdef _WIN32
    timeEndPeriod(1);
#endif
}

bool TcpClient::doWrite(void)
{
    bool bRet = false;

    do {
        int n;

        std::unique_lock<std::mutex> autolock(this->sendQueueMtx);
        if (!sendQueue.empty()) {
            auto& v = sendQueue.front();
            n = impl.send_i(v->data.data(), v->data.size());
            if (n == v->data.size()) {
                sendQueue.pop();
                CCRUNONGL([v]{
                    if (v->onSend != nullptr)
                        v->onSend(ErrorCode::ERR_OK);
                    v->release();
                });
            }
            else if (n > 0) { // TODO: add time
                if ((std::clock() - v->timestamp) < (CLOCKS_PER_SEC * timeoutForSend))
                { // erase sent data, remain data will send next time.
                    v->data.erase(v->data.begin(), v->data.begin() + n);
                }
                else { // send timeout
                    sendQueue.pop();
                    CCRUNONGL([v]{
                        if (v->onSend)
                            v->onSend(ErrorCode::ERR_SEND_TIMEOUT);
                        v->release();
                    });
                }
            }
            else { // n <= 0, TODO: add time
                int ec = xxsocket::get_last_errno();
                if (SHOULD_CLOSE_1(n, ec)) {
                    sendQueue.pop();
                    CCRUNONGL([v]{
                        if (v->onSend)
                            v->onSend(ErrorCode::ERR_CONNECTION_LOST);
                        v->release();
                    });

                    handleError();
                    break;
                }
            }
            idle = false;
        }
        autolock.unlock();

        bRet = true;
    } while (false);

    return bRet;
}

bool TcpClient::doRead(void)
{
    bool bRet = false;
    do {
        int n = impl.recv_i(this->buffer + offset, sizeof(buffer) - offset);

        if (n > 0 || (n == -1 && offset != 0)) {

            if (n == -1)
                n = 0;

            cocos2d::log("TcpClient::doRead --- received data len: %d, buffer data len: %d", n, n + offset);

            if (expectedPacketLength == -1) { // decode length
                if (decodePacketLength(this->buffer, offset + n, expectedPacketLength))
                {
                    if (expectedPacketLength < 0) { // header insuff
                        offset += n;
                    }
                    else { // ok
                        auto bytes_transferred = n + offset;
                        this->recvingPacket.insert(recvingPacket.end(), buffer, buffer + (std::min)(expectedPacketLength, bytes_transferred));
                        if (bytes_transferred >= expectedPacketLength)
                        { // packet received properly
                            offset = bytes_transferred - expectedPacketLength; // set offset to bytes of remain buffer
                            if (offset > 0)
                                ::memmove(this->buffer, this->buffer + expectedPacketLength, offset);

                            moveReceivedPacket();
                        }
                        else { // all buffer consumed, set offset to ZERO
                            offset = 0;
                        }
                    }
                }
                else {
                    error = ErrorCode::ERR_DPL_ILLEGAL_PACKET;
                    handleError();
                    break;
                }
            }
            else { // recv remain packet data
                auto bytes_transferred = n + offset;
                if ((recvingPacket.size() + bytes_transferred) > MAX_PACKET_LEN) // TODO: config MAX_PACKET_LEN, now is 16384
                {
                    error = ErrorCode::ERR_PACKET_TOO_LONG;
                    handleError();
                    break;
                }
                else {
                    auto bytesNeeded = expectedPacketLength - static_cast<int>(recvingPacket.size()); // never equal zero or less than zero
                    if (bytesNeeded > 0) {
                        this->recvingPacket.insert(recvingPacket.end(), buffer, buffer + (std::min)(bytes_transferred, bytesNeeded));

                        offset = bytes_transferred - bytesNeeded; // set offset to bytes of remain buffer
                        if (offset >= bytesNeeded)
                        { // packet received properly
                            if (offset > 0)
                                ::memmove(this->buffer, this->buffer + bytesNeeded, offset);

                            moveReceivedPacket();
                        }
                        else { // all buffer consumed, set offset to ZERO, continue recv this packetdata
                            offset = 0;
                        }
                    }
                    else {
                        assert(false);
                    }
                }
            }

            idle = false;
        }
        else {
            int ec = xxsocket::get_last_errno();
            if (SHOULD_CLOSE_0(n, ec)){
                if (n == 0) {
                    cocos2d::log("peer close the connection.");
                }
                handleError();
                break;
            }
        }

        bRet = true;

    } while (false);

    return bRet;
}


void TcpClient::scheduleCollectResponse()
{
    CCSCHTASKS->schedule(
        schedule_selector(TcpClient::dispatchResponseCallbacks), this, 0, false);
    CCSCHTASKS->pauseTarget(this);
}

void TcpClient::unscheduleCollectResponse()
{
    CCSCHTASKS->unschedule(schedule_selector(TcpClient::dispatchResponseCallbacks), this);
}

void TcpClient::dispatchResponseCallbacks(float delta)
{
    std::unique_lock<std::mutex> autolock(this->recvQueueMtx);
    if (!this->recvQueue.empty()) {
        auto packet = std::move(this->recvQueue.front());
        this->recvQueue.pop();

        if (this->recvQueue.empty()) {
            autolock.unlock();
            CCSCHTASKS->pauseTarget(this);
        }
        else
            autolock.unlock();

        if (onRecvPacket != nullptr)
            this->onRecvPacket(std::move(packet));
    }
    else {
        autolock.unlock();
        CCSCHTASKS->pauseTarget(this);
    }
}
