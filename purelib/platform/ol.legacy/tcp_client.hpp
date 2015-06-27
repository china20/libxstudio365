//
// tcp_client.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2014 Newsoft.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _TCP_CLIENT_HPP_
#define _TCP_CLIENT_HPP_
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class tcp_client
{
public:
    tcp_client(const std::string& address, u_short port)
        : 
        io_service(),
        service_running_(false),
        socket_(io_service),
        timeout_check_timer_(io_service),
        address_(address),
        port_(port),
        error_code_(0),
        expecting_size_(0),
        max_data_len_(8192),
        connect_timeout_(5),
        request_timeout_(10)
        
    {
    }

    void start_request(const void* data, size_t len)
    {
        if(this->socket_.is_open())
        {
            // The connection was successful. Send the request.
            do_timeout_check(request_timeout_);

            boost::asio::async_write(socket_,
                boost::asio::buffer(data, len),
                boost::bind(&tcp_client::handle_write_request,
                this,
                boost::asio::placeholders::error));
        }
        else
        {
            tcp::endpoint ep(boost::asio::ip::address_v4::from_string(address_), port_);
            do_timeout_check(connect_timeout_);
            socket_.open(boost::asio::ip::tcp::v4());
            socket_.async_connect(ep, [this,data,len](const boost::system::error_code& ec){
                
                cancel_timer();

                if(!ec)
                {
                    start_request(data, len);
                }
                else {
                    handle_error(ec);
                }
            });
        }

        if(!service_running_)
        {
            service_running_ = true;
            boost::system::error_code ignored_ec;
            io_service.run(ignored_ec);
            service_running_ = false;
        }
    }

    int get_error_code(void) const
    {
        return this->error_code_;
    }

private:

    void do_timeout_check(size_t sec)
    {
        timeout_check_timer_.expires_from_now(boost::posix_time::seconds(sec));
        timeout_check_timer_.async_wait([this](const boost::system::error_code& ec){
            if(!ec) {
                this->abort();
            }
        });
    }

    void handle_write_request(const boost::system::error_code& ec)
    {
        if (!ec)
        {
            // Read the response data length
            boost::asio::async_read(socket_, 
                boost::asio::buffer((void*)&expecting_size_, sizeof(expecting_size_)),
                boost::asio::transfer_exactly( sizeof(expecting_size_) ),
                boost::bind(&tcp_client::handle_read_length, 
                this, 
                boost::asio::placeholders::error));
        }
        else
        {
            handle_error(ec);
        }
    }

    void handle_read_length(const boost::system::error_code& ec)
    {
        if (!ec)
        {
            expecting_size_ = ntohl(expecting_size_);
            if(expecting_size_ > this->max_data_len_)
            { // too long response received
                cancel_timer();
                return;
            }

            this->total_bytes_read = 0;

            // Start reading remaining data until EOF.
            size_t size = (std::min)(sizeof(buffer_), this->expecting_size_);
            boost::asio::async_read(socket_, 
                boost::asio::buffer(buffer_, size ),
                boost::asio::transfer_exactly( size ),
                boost::bind(&tcp_client::handle_read_content, 
                this, 
                boost::asio::placeholders::error, 
                boost::asio::placeholders::bytes_transferred) );
        }
        else
        {
            handle_error(ec);
        }
    }

    void handle_read_content(const boost::system::error_code& ec, size_t bytes_transferred)
    {
        if (!ec)
        {
            // write buffer to response data
            response_data_.insert(response_data_.end(), buffer_, buffer_ + bytes_transferred);

            this->total_bytes_read += bytes_transferred;

            if(this->total_bytes_read < this->expecting_size_)
            {
                // Continue reading remaining data until EOF.
                size_t size = (std::min)(sizeof(buffer_), this->expecting_size_ - this->total_bytes_read);
                boost::asio::async_read(socket_, 
                    boost::asio::buffer(buffer_, size ),
                    boost::asio::transfer_exactly( size ),
                    boost::bind(&tcp_client::handle_read_content, 
                    this,
                    boost::asio::placeholders::error, 
                    boost::asio::placeholders::bytes_transferred));
            }
            else { // complete
                this->error_code_ = ec.value();
                cancel_timer();
            }
        }
        else
        {
            handle_error(ec);
        }
    }

    void handle_error(const boost::system::error_code& ec)
    {
        this->cancel_timer();
        
        if(ec != boost::asio::error::operation_aborted)
            this->abort();

        error_code_ = ec.value();
    }

public:
    void abort(void)
    {
        boost::system::error_code ignored_ec;
        socket_.close(ignored_ec);
        // total_bytes_read = 0;
    }

    void cancel_timer(void)
    {
        boost::system::error_code ignored_ec;
        this->timeout_check_timer_.cancel(ignored_ec);
    }

    boost::asio::io_service     io_service;

    bool                        service_running_;

    std::string                 address_;
    u_short                     port_;
    tcp::socket                 socket_;
    boost::asio::deadline_timer timeout_check_timer_;
    size_t                      connect_timeout_;
    size_t                      request_timeout_;

    char                        buffer_[512];
    const size_t                max_data_len_;
    size_t                      total_bytes_read;
    std::vector<char>           response_data_;

    int                         error_code_;
    unsigned int                expecting_size_;
};

#endif

