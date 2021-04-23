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


private:
	std::vector <std::shared_ptr<Particle>> particles;

};