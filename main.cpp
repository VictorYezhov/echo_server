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

using boost::asio::ip::tcp;
using boost::system::error_code;
using namespace boost::asio;
using namespace boost::posix_time;
class client_session
{
public:
    typedef boost::shared_ptr<client_session> pointer;

    static pointer create(boost::asio::io_service &io) {
        return pointer(new client_session(io));
    }

    ~client_session() {
        //log("Connection closed");
        socket_.close();
    }

    tcp::socket &socket() { return socket_; }

    size_t read_complete(char * buff, const error_code & err, size_t bytes) {
        if ( err) return 0;
        bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
        return found ? 0 : 1;
    }


    void start() {
        //log("Connection established");

        char buff[1024];

        socket_.read_some(buffer(buff));

        socket_.write_some(buffer(buff));


    }

protected:
    client_session(boost::asio::io_service &io)
            : socket_(io) {
    }

    std::string make_time_string() {
        char buf[32];
        time_t now = time(0);
        return ctime_r(&now, buf);
    }

    void log(std::string const &message) {
        std::clog << boost::format("%|-25| [client address: %|15|]\n")
                     % message % socket_.remote_endpoint().address().to_string();
    }

private:
    tcp::socket socket_;
};

int main()
{
    boost::asio::io_service io;

    tcp::acceptor acceptor(io, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8001));
    for (;;) {
        try {
            client_session::pointer new_client = client_session::create(io);
            acceptor.accept(new_client->socket());
            boost::thread(boost::bind(&client_session::start, new_client)).detach();
        }catch (...){

        }
    }
}