#include <iostream>
#include <boost/asio.hpp>

void write_data(boost::asio::ip::tcp::socket& socket, const std::string& name)
{
	std::string data;

	do
	{
		std::getline(std::cin, data);

		boost::asio::write(socket, boost::asio::buffer(name + " : " + data + "\n"));

	} while (data != "exit");

	boost::asio::write(socket, boost::asio::buffer("exit\n"));

	std::cout << "write ended\n";
}

void read_data_until(boost::asio::ip::tcp::socket& socket)
{
	while (true)
	{
		boost::asio::streambuf buffer;

		boost::asio::read_until(socket, buffer, '\n');

		std::string message;

		std::istream input_stream(&buffer);



		std::getline(input_stream, message, '\n');

		if (message != "exit")
			std::cout << message << std::endl;
		else
		{
			std::cout << "read ended\n";
			return;
		}
	}
}
