//
// Created by Victor on 23.05.2018.
//

#ifndef SERVER_BOOST_SERVER_H
#define SERVER_BOOST_SERVER_H

#include <mutex>
#include <thread>
#include <mutex>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

class Server {
private:
    std::mutex mutex;
    io_service service;

public:
    size_t read_complete(char * buff, const error_code & err, size_t bytes);
    void work(ip::tcp::acceptor &acceptor);
    void handle_connections();

};


#endif //SERVER_BOOST_SERVER_H
