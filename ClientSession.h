//
// Created by Victor on 11.06.2018.
//

#ifndef SERVER_BOOST_CLIENTSESSION_H
#define SERVER_BOOST_CLIENTSESSION_H
#include <ctime>
#include <string>
#include <iostream>

#include <thread>
#include <mutex>

#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <deque>


using boost::asio::ip::tcp;
using boost::system::error_code;
using namespace boost::asio;
using namespace boost::posix_time;


/**
 * Це просто клас обгортка для сокета  до якого підключаються клієнти
 */
class ClientSession {

public:

    ~ClientSession() {
        socket_.close();
    }

    ClientSession(boost::asio::io_service &io):io(io), socket_(io){}// Кожен сокет ініціалізується io-сервісом
    tcp::socket socket_;
private:
    boost::asio::io_service &io;
};


#endif //SERVER_BOOST_CLIENTSESSION_H
