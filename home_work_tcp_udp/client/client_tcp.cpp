//
// Created by flyon21 on 25.01.25.
//
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <string>
#include <algorithm>

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

void cleanup_message(std::string& msg) {
    msg.erase(std::remove(msg.begin(), msg.end(), '\n'), msg.end());
    msg.erase(std::remove(msg.begin(), msg.end(), '\r'), msg.end());
}

void start_ssl_client(const std::string& host, unsigned short port) {
    try {
        boost::asio::io_context io_context;
        ssl::context ssl_context(ssl::context::tlsv12_client);
        ssl_context.set_default_verify_paths();

        ssl::stream<tcp::socket> ssl_socket(io_context, ssl_context);

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(ssl_socket.lowest_layer(), endpoints);
        ssl_socket.handshake(ssl::stream_base::client);

        std::cout << "Connected to server at " << host << ":" << port << std::endl;

        std::string message;
        while (true) {
            std::cout << "Enter message (or 'exit' to quit): ";
            if (!std::getline(std::cin, message) || message == "exit") {
                std::cout << "Exiting client." << std::endl;
                break;
            }

            cleanup_message(message);
            if (!message.empty()) {
                try {
                    boost::asio::write(ssl_socket, boost::asio::buffer(message + "\n"));

                    boost::asio::streambuf buffer;
                    boost::system::error_code ec;
                    boost::asio::read_until(ssl_socket, buffer, '\n', ec);

                    if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset) {
                        std::cerr << "Server disconnected." << std::endl;
                        break;
                    } else if (ec) {
                        std::cerr << "Error: " << ec.message() << std::endl;
                        break;
                    }

                    std::istream stream(&buffer);
                    std::string reply;
                    std::getline(stream, reply);
                    cleanup_message(reply);
                    std::cout << "Server replied: " << reply << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    break;
                }
            }
        }

        ssl_socket.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		ssl_socket.lowest_layer().close();
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

int main() {
    const std::string host = "127.0.0.1";
    const unsigned short port = 21058;
    start_ssl_client(host, port);
    return 0;
}
