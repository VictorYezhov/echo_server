#include "ClientSession.h"
#include <deque>


using boost::asio::ip::tcp;
using boost::system::error_code;
using namespace boost::asio;
using namespace boost::posix_time;

void start_work(std::shared_ptr<ClientSession> client) {

    char buff[1024];
    client->socket_.read_some(buffer(buff));
    client->socket_.write_some(buffer(buff));
    client->socket_.close();
    //client.recreate();
}

void accept_and_run(ip::tcp::acceptor& acceptor, io_service& io_service, int i)
{
    std::shared_ptr<ClientSession> ses;
    ses.reset(new ClientSession(io_service));
    acceptor.async_accept(ses->socket_, [ses, &acceptor, &io_service, i](const error_code& accept_error)
    {
        // Call to contiue listening

        int count = i+1;
        accept_and_run(acceptor, io_service, count);

        if(!accept_error)
        {
            start_work(ses);
        }
    });
}





int main()
{
    boost::asio::io_service io;

    boost::asio::io_service::work some_work(io);
    std::cout<<"Server ready"<<std::endl;

    tcp::acceptor acceptor(io, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8001));
    int i =0;
    acceptor.listen();

    boost::thread_group threads;
    for (int i = 0; i < 3; ++i) {
        threads.create_thread(boost::bind(&io_service::run, &io));
    }

    accept_and_run(acceptor, io, 0);

    io.run();

    threads.join_all();

}