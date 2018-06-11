//
// Created by Victor on 11.06.2018.
//

#include "Server.h"

void Server::process_connection(std::shared_ptr<ClientSession> client) {
    char buff[1024];
    client->socket_.read_some(buffer(buff)); /// зчитуємо повідомлення в буфер
    client->socket_.write_some(buffer(buff)); /// записуємо його назад
    client->socket_.close(); /// закриваємо з'єднання
}

void Server::async_accept_new_connection(ip::tcp::acceptor &acceptor, io_service &io_service) {

    std::shared_ptr<ClientSession> ses; /// використовуємо shared_prt, що б об'єкт не знищувався при виході з області видимості
    ses.reset(new ClientSession(io_service)); /// Встановлюємо вказівник на відповідне нове підключення
    acceptor.async_accept(ses->socket_, [ses, &acceptor, &io_service, this](const error_code& accept_error)
    {
        // Call to contiue listening
        async_accept_new_connection(acceptor, io_service); /// ассинхронно приймаємо новий конекшн

        if(!accept_error)
        {
            process_connection(ses); /// і обробляємо підключення
        }
    });
}

void Server::run() {

    boost::asio::io_service io; /// Це якась магічна штука яка займається багатьма низькорівневими речами, нам то знати не треба
    boost::asio::io_service::work some_work(io); /// The work class is used to inform the io_service when work starts and finishes.
    /// This ensures that the io_service object's run() function will not exit while work is underway,
    /// and that it does exit when there is no unfinished work remaining.
    std::cout<<"Server ready"<<std::endl;
    tcp::acceptor acceptor(io, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8001));/// От ця штука приймає всі конекшини
    acceptor.listen(); /// починаємо слухати
    boost::thread_group threads;
    for (int i = 0; i < std::thread::hardware_concurrency() - 1; ++i) {
        threads.create_thread(boost::bind(&io_service::run, &io)); /// Створюємо n-1 потік виконання, де n - hardware_concurrency() компа
    }
    async_accept_new_connection(acceptor, io); /// і запускаємо процес обробки
    io.run();  /// ше одна магічна штука, то шось для того io_service
    threads.join_all(); /// Ну і Джоінимо  всі потоки
}
