#pragma once
#include <iostream>
#include <vector>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <memory>


#include "Calculator.hpp"
#include "Connection.hpp"
#include "Particle.hpp"


class System
{
public:
	void run()
	{

	}




private:
	std::vector <std::shared_ptr<Particle>> particles;

	const int m_width;
	const int m_height;

	boost::asio::io_service ioService;
	boost::thread_group threadpool;

	double temperature, pressure;
	int amount;

};