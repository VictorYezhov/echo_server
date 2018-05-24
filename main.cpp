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

std::mutex mutex;

using boost::asio::ip::tcp;
using boost::system::error_code;
using namespace boost::asio;
using namespace boost::posix_time;
class client_session
{
public:
    bool ready;

    static client_session* create(boost::asio::io_service &io) {
        return new client_session(io);
    }

    ~client_session() {
        //log("Connection closed");
        socket_->close();
        ready = false;
        std::cout<<"client deleted\n";
    }

    boost::shared_ptr<tcp::socket> socket() { return socket_; }

    size_t read_complete(char * buff, const error_code & err, size_t bytes) {
        if ( err) return 0;
        bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
        return found ? 0 : 1;
    }


    void recreate(){
        socket_.reset(new tcp::socket(io));
       // log("socket recreated");
    }

    void delete_sock(){
        socket_.reset();
       // log("socket deleted");
    }

protected:
    client_session(boost::asio::io_service &io):io(io){
        socket_.reset(new tcp::socket(io));
        ready = true;
    }

    std::string make_time_string() {
        char buf[32];
        time_t now = time(0);
        return ctime_r(&now, buf);
    }

    void log(std::string const &message) {
        std::clog << boost::format("%|-25| [client address: %|15|]\n")
                     % message % socket_->remote_endpoint().address().to_string();
    }

private:

    boost::asio::io_service &io;
    boost::shared_ptr<tcp::socket> socket_;
};

void start_work(client_session &client) {


    char buff[1024];
    client.socket()->read_some(buffer(buff));
    client.socket()->write_some(buffer(buff));
    client.socket()->close();
    client.delete_sock();
    //client.recreate();
}



std::vector<client_session*> available_sessions;
int main()
{
    boost::asio::io_service io;
    for(int i=0; i< 50001; i++){
        available_sessions.push_back(client_session::create(io));
    }
    std::cout<<"Server ready"<<std::endl;

    tcp::acceptor acceptor(io, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8001));
    int i =0;
    for (;;) {
        try {
            acceptor.accept(*(available_sessions[i]->socket()));
            std::thread t4(start_work, std::ref(*available_sessions[i]));
            i++;
            t4.detach();
            //if(i == 999){
              //  i = 0;
            //}

        }catch (...){
        }
    }
}