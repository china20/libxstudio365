#ifndef _NXTCPCLIENT_H_
#define _NXTCPCLIENT_H_
#include "utils/xxsocket.h"
#include "utils/endian_portable.h"
#include "utils/singleton.h"
#include <cocos2d.h>
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>
#include <condition_variable>

using namespace purelib;
using namespace purelib::net;

namespace inet {

    class TcpSendingPacket;

    // single thread only
    class TcpClient : public cocos2d::Ref
    {
    public:
        enum ErrorCode {
            ERR_OK, // NO ERROR
            ERR_CONNECT_FAILED, // connect failed
            ERR_CONNECT_TIMEOUT, // connect timeout
            ERR_SEND_FAILED, // send error, failed
            ERR_SEND_TIMEOUT, // send timeout
            ERR_RECV_FAILED, // recv failed
            ERR_NETWORK_UNREACHABLE, // wifi or 2,3,4G not open
            ERR_CONNECTION_LOST, // connection lost
            ERR_PACKET_TOO_LONG, // packet too long
            ERR_DPL_ILLEGAL_PACKET, // decode packet error.
        };

        // End user packet decode length func
        typedef bool(*DecodePacketLengthFunc)(const char* data, size_t datalen, int& len);

        // callbacks
        typedef std::function<void(ErrorCode)> OnPacketSendCallback;
        typedef std::function<void(std::vector<char>&&)> OnPacketRecvCallback;

    public:
        TcpClient();
        ~TcpClient();

        bool init();

        void close();

        void setEndpoint(const char* address, u_short port);
        void setConnectionEstablishedListener(const std::function<void()>& callback);
        void setConnectionLostListener(const std::function<void()>& callback);

        void setDecodeLengthFunc(DecodePacketLengthFunc func);
        void setOnRecvListener(const OnPacketRecvCallback& callback);

        void send(std::vector<char>&& data, const OnPacketSendCallback& callback = nullptr);

        void setTimeoutForConnect(int timeout);
        void setTimeoutForSend(int timeout);
        ErrorCode  getErrorCode(void) { return error; }

    private:
        // collect
        void scheduleCollectResponse();
        void unscheduleCollectResponse();
        void dispatchResponseCallbacks(float delta);
    private:

        bool connect(void);

        void service(void);

        bool doWrite(void);
        bool doRead(void);

        void handleError(void); // TODO: add errorcode parameter

        void moveReceivedPacket(); // move received properly packet to recv queue

    private:
        bool                    bAppExiting;
        bool                    threadStarted;

        std::string             address;
        u_short                 port;

        xxsocket                impl;
        bool                    connected;
        ErrorCode               error;

        int                     timeoutForConnect;
        int                     timeoutForSend;

        std::mutex              sendQueueMtx;
        std::mutex              recvQueueMtx;

        std::queue<TcpSendingPacket*> sendQueue;
        std::queue<std::vector<char>> recvQueue;

        char                    buffer[512]; // recv buffer
        int                     offset; // recv buffer offset
        std::vector<char>       recvingPacket;
        int                     expectedPacketLength;


        // socket event set
        fd_set readfds, writefds, excepfds;

        //      
        std::function<void()>   connectionLostListener;
        std::function<void()>   connectionEstablishedListener;

        OnPacketRecvCallback    onRecvPacket;

        DecodePacketLengthFunc  decodePacketLength;

        bool                    idle;

        long                    reconnClock;
    };

}
#endif

#define tcpclient purelib::gc::delayed<inet::TcpClient>::instance(&inet::TcpClient::init)
