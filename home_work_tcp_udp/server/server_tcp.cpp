//
// Created by flyon21 on 25.01.25.
//
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>
#include <set>

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

std::set<std::shared_ptr<ssl::stream<tcp::socket>>> clients;
std::mutex clients_mutex;

void broadcast_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        try {
            boost::asio::write(*client, boost::asio::buffer(message + "\n"));
        } catch (const std::exception& e) {
            std::cerr << "Error broadcasting message: " << e.what() << std::endl;
        }
    }
}

void handle_client(std::shared_ptr<ssl::stream<tcp::socket>> ssl_socket) {
    try {
        char buffer[1024];
        while (true) {
            boost::system::error_code ec;
            size_t bytes_read = ssl_socket->read_some(boost::asio::buffer(buffer), ec);

            if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset) {
                std::cerr << "Client disconnected." << std::endl;
                break;
            } else if (ec) {
                std::cerr << "Error reading from client: " << ec.message() << std::endl;
                break;
            }

            std::string message(buffer, bytes_read);
            std::cout << "Client says: " << message << std::endl;

            boost::asio::write(*ssl_socket, boost::asio::buffer("Echo: " + message + "\n"));

            broadcast_message("Broadcast from server: " + message);
        }
    } catch (const std::exception& e) {
        std::cerr << "Client handling exception: " << e.what() << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(ssl_socket);
        std::cout << "A client has disconnected. Total connected clients: " << clients.size() << std::endl;
    }
}

void start_ssl_server(unsigned short port) {
    try {
        boost::asio::io_context io_context;

        ssl::context ssl_context(ssl::context::tlsv12_server);
        ssl_context.set_options(ssl::context::default_workarounds | ssl::context::no_sslv2);

        ssl_context.use_certificate_chain_file("server.crt");
        ssl_context.use_private_key_file("server.key", ssl::context::pem);

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
        std::cout << "SSL server is running on port " << port << std::endl;

        while (true) {
            auto socket = std::make_shared<tcp::socket>(io_context);
            acceptor.accept(*socket);

            auto ssl_socket = std::make_shared<ssl::stream<tcp::socket>>(std::move(*socket), ssl_context);

            try {
                ssl_socket->handshake(ssl::stream_base::server);

                {
                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.insert(ssl_socket);
                    std::cout << "New client connected. Total connected clients: " << clients.size() << std::endl;
                }

                std::thread(handle_client, ssl_socket).detach();
            } catch (const std::exception& e) {
                std::cerr << "SSL handshake failed: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Server exception: " << e.what() << std::endl;
    }
}

int main() {
    const unsigned short port = 21058;
    start_ssl_server(port);
    return 0;
}