//
// Created by Victor on 23.05.2018.
//

#include "Server.h"


size_t Server::read_complete(char *buff, const error_code &err, size_t bytes) {
    if ( err) return 0;
    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void Server::work(ip::tcp::acceptor &acceptor) {
    char buff[1024];
    while ( true) {
        ip::tcp::socket sock(service);
        mutex.lock();
        acceptor.accept(sock);
        mutex.unlock();
        int bytes = read(sock, buffer(buff),
                         boost::bind(&Server::read_complete,buff,_1,_2));
        std::string msg(buff, bytes);
        sock.write_some(buffer(msg));
        sock.close();
    }
}

void Server::handle_connections() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8001));
    boost::thread_group threads;

    std::thread t1(&Server::work,std::ref(acceptor));
    std::thread t2(&Server::work,std::ref(acceptor));
    std::thread t3(&Server::work,std::ref(acceptor));
    std::thread t4(&Server::work,std::ref(acceptor));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
