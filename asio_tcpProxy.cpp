#include <iostream>
#include <asio.hpp>

using namespace std;
using namespace asio::ip;

class tcp_proxy {
public:
    tcp_proxy(asio::io_context& io_context, const tcp::endpoint& local_endpoint, const tcp::endpoint& remote_endpoint)
        : io_context_(io_context),
          acceptor_(io_context, local_endpoint),
          remote_endpoint_(remote_endpoint) {}

    void start() {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](asio::error_code ec, tcp::socket socket) {
                if (!ec) {
                    cout << "Accepted connection from " << socket.remote_endpoint() << endl;
                    auto remote_socket = make_shared<tcp::socket>(io_context_);
                    remote_socket->async_connect(
                        remote_endpoint_,
                        [this, remote_socket = move(remote_socket), socket = move(socket)](asio::error_code ec) {
                            if (!ec) {
                                cout << "Connected to remote endpoint " << remote_endpoint_ << endl;
                             //   do_proxy(socket, *remote_socket);
                              //  do_proxy((*remote_socket), (socket));
                            }
                        });
                }
                do_accept();
            });
    }

    void do_proxy(tcp::socket source_socket, tcp::socket destination_socket) {
        source_socket.async_read_some(
            asio::buffer(data_, max_length),
            [this, source_socket1 = move(source_socket), destination_socket = move(destination_socket)](asio::error_code ec, size_t length) {
                if (!ec && length > 0) {
                    asio::async_write(
                        destination_socket,
                        asio::buffer(data_, length),
                        [this, source_socket2 = std::move(source_socket1), destination_socket = move(destination_socket)](asio::error_code ec, size_t length) {
                            if (!ec) {
                            //    do_proxy(move(source_socket), move(destination_socket));
                            }
                        });
                }
            });
    }

    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    tcp::endpoint remote_endpoint_;
    enum { max_length = 1024 };
    char data_[max_length];
};

int main() {
    try {
        asio::io_context io_context;

        // Define the local and remote endpoints
        tcp::endpoint local_endpoint(tcp::v4(), 10009);
        tcp::endpoint remote_endpoint(address::from_string("192.168.123.103"), 10008);

        // Create the proxy and start it
        tcp_proxy proxy(io_context, local_endpoint, remote_endpoint);
        proxy.start();

        // Run the io_context
        io_context.run();
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
