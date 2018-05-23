
#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

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

std::mutex mutex;
io_service service;
size_t read_complete(char * buff, const error_code & err, size_t bytes) {
    if ( err) return 0;
    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
    return found ? 0 : 1;
}


void work(ip::tcp::acceptor &acceptor){
    char buff[1024];
    while ( true) {
        ip::tcp::socket sock(service);
        mutex.lock();
        acceptor.accept(sock);
        mutex.unlock();
        int bytes = read(sock, buffer(buff),
                         boost::bind(read_complete,buff,_1,_2));
        std::string msg(buff, bytes);
        sock.write_some(buffer(msg));
        sock.close();
    }
}

void handle_connections() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8001));
    boost::thread_group threads;

    std::thread t1(work,std::ref(acceptor));
    std::thread t2(work,std::ref(acceptor));
    std::thread t3(work,std::ref(acceptor));
    std::thread t4(work,std::ref(acceptor));

    t1.join();
    t2.join();
    t3.join();
    t4.join();

}




int main(int argc, char* argv[]) {
    handle_connections();
}



