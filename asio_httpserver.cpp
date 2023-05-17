#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;
using namespace asio;
using namespace asio::ip;

class HttpServer {
public:
  HttpServer(boost::asio::io_context& io_context, const tcp::endpoint& endpoint) :
    acceptor_(io_context, endpoint),
    socket_(io_context) {
    do_accept();
  }

private:
  void do_accept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
      if (!ec) {
        // Read the HTTP request from the client
        boost::asio::streambuf request_buf;
        boost::asio::read_until(socket_, request_buf, "\r\n\r\n");
        std::istream request_stream(&request_buf);
        std::string request_method, request_path, request_http_version;
        request_stream >> request_method >> request_path >> request_http_version;

        // Send an HTTP response back to the client
        std::ostringstream response_stream;
        response_stream << "HTTP/1.1 200 OK\r\n";
        response_stream << "Content-Type: text/plain\r\n";
        response_stream << "Content-Length: 13\r\n";
        response_stream << "\r\n";
        response_stream << "Hello, world!";
        std::string response_str = response_stream.str();
        boost::asio::write(socket_, boost::asio::buffer(response_str));

        // Close the connection to the client
        boost::system::error_code ignored_ec;
        socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
      }

      // Accept the next incoming connection
      do_accept();
    });
  }

  tcp::acceptor acceptor_;
  tcp::socket socket_;
};

int main() {
  try {
    // Create an io_context object
    boost::asio::io_context io_context;

    // Create an HTTP server object and listen on port 8080
    HttpServer server(io_context, tcp::endpoint(tcp::v4(), 8080));

    // Run the io_context object
    io_context.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
