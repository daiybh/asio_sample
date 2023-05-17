//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using asio::ip::tcp;

const int max_length = 1024;

asio::io_context io_context;
void session(tcp::socket sock)
{
	try
	{
		tcp::socket upStreamSocket(io_context); 
		tcp::resolver resolver(io_context);
		tcp::endpoint remote_endpoint(asio::ip::address::from_string("192.168.123.103"), 10008);
		upStreamSocket.connect(remote_endpoint);
		for (;;)
		{
			char data[max_length];

			std::error_code error;
			size_t length = sock.read_some(asio::buffer(data), error);
			if (error == asio::stream_errc::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw std::system_error(error); // Some other error.
			asio::write(upStreamSocket, asio::buffer(data, length));
			asio::write(sock, asio::buffer(data, length));
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void server(asio::io_context& io_context, unsigned short port)
{
	tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		tcp::socket sock(io_context);
		a.accept(sock);
		std::thread(session,  std::move(sock)).detach();
	}
}

int main(int argc, char* argv[])
{
	try
	{
		
		int port = 10009;
	//	asio::io_context io_context;

		server(io_context, port);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
