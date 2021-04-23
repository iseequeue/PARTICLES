#pragma once
#include <iostream>
#include <vector>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <memory>

#include "particle.cpp";

class System
{
public:
	void run()
	{

	}

	void ellastic_collision(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2) {};

	void unellastic_collision(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2) {};

	void calc_motion() {};

	void calc_collisions() {};

	void reduction_phase() {};


private:
	std::vector <std::shared_ptr<Particle>> particles;

	const int m_width;
	const int m_height;

	boost::asio::io_service ioService;
	boost::thread_group threadpool;

	double temperature, pressure;
	int amount;

};