//
// Created by Victor on 11.06.2018.
//

#ifndef SERVER_BOOST_SERVER_H
#define SERVER_BOOST_SERVER_H
#include "ClientSession.h"

class Server {


public:
    void process_connection(std::shared_ptr<ClientSession> client);
    void async_accept_new_connection(ip::tcp::acceptor &acceptor, io_service &io_service);
    void run();
};


#endif //SERVER_BOOST_SERVER_H
