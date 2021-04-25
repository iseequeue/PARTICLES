#pragma once
#include <iostream>
#include <boost/asio.hpp>

class Connection
{
public:
	void write_data(boost::asio::ip::tcp::socket& socket, const std::string& name){}

	void read_data_until(boost::asio::ip::tcp::socket& socket){}

	Connection(std::string raw_ip_address, unsigned int port = 8000): m_port(port)

	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

		boost::asio::io_service io_service;


		try
		{
			boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

			acceptor.bind(endpoint);

			acceptor.listen(m_size);

			boost::asio::ip::tcp::socket socket(io_service);

			acceptor.accept(socket);

		}
		catch (boost::system::system_error& e)
		{
			std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

			system("pause");
		}
	}

private:
	std::size_t m_size = 30;
	std::size_t m_port;
};