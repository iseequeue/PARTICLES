#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include <chrono>
#include <ctime>
#include <cstdint>
#include <exception>

#include <memory>
#include <vector>
#include <random>

#include "Particle.hpp"
#include "Calculator.hpp"

#include "ThreadPool.hpp"

void initialize_json(nlohmann::json& j, std::shared_ptr<Particle> p);



class System
{
public:

	System(std::size_t amount = 51, std::size_t fraction = 6, std::size_t width = sf::VideoMode::getDesktopMode().width,
		std::size_t height = sf::VideoMode::getDesktopMode().height) :
		m_amount(amount),
		m_fraction(fraction),
		m_width(width),
		m_height(height),
		m_application(sf::VideoMode(m_width, m_height), "Particles"),
		mersenne(rd()),
		uidx(0.0, m_width),
		uidy(0.0, m_height),
		uidv(-4.0 / 5, 4.0 / 5),
		field(m_fraction, std::vector<int>(m_fraction, 0)),
		temperatures(m_fraction, std::vector<double>(m_fraction, 0.0)),
		membership(m_fraction, std::vector < std::vector<std::size_t>>(m_fraction, { 0,0,0 }))

	{
		m_particle.reserve(m_amount);

		m_particle.push_back(std::make_shared<Reagent>(width / 2, height / 2, 0.1, 0.1, 1.0, 6.0, Particles::First));

		double x, y;

		for (auto i = 0U; i < m_amount/2; i++)
		{
			x = uidx(mersenne);
			y = uidy(mersenne);

			while ((x - width / 2) * (x - width / 2) +
				(y - height / 2) * (y - height / 2)
				<= (m_particle[0]->m_radius + 10.0) * (m_particle[0]->m_radius + 10.0))
			{
				x = uidx(mersenne);
				y = uidy(mersenne);
			}

			m_particle.push_back(std::make_shared<Reagent>( x, y, uidv(mersenne), uidv(mersenne), 1.0, 6.0, Particles::First));
		}
		for (auto i = 0U; i < m_amount / 2; i++)
		{
			x = uidx(mersenne);
			y = uidy(mersenne);

			while ((x - width / 2) * (x - width / 2) +
				(y - height / 2) * (y - height / 2)
				<= (m_particle[0]->m_radius + 10.0) * (m_particle[0]->m_radius + 10.0))
			{
				x = uidx(mersenne);
				y = uidy(mersenne);
			}

			m_particle.push_back(std::make_shared<Reagent>(x, y, uidv(mersenne), uidv(mersenne), 1.0, 6.0, Particles::Second));
		}

		if (!font.loadFromFile("arial.ttf"))
		{
			std::cerr << "error..." << std::endl;
		}

		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::Red);
		text.setPosition(50, 50);

	}

	void run();


	double temperature()
	{
		double t = 0.0;
		for (const auto &p : m_particle)
			t += 0.5 * (p->m_mass * std::pow(p->m_dx, 2) + std::pow(p->m_dy, 2));
		return t/m_particle.size();
	}

	void recession()
	{
		auto x = 0U;
		auto y = 0U;
		for (const auto& p : m_particle)
		{
			x = static_cast<int>(p->m_x / (m_width / m_fraction)) % m_fraction;
			y = static_cast<int>(p->m_y / (m_height / m_fraction)) % m_fraction;
			temperatures[x][y]+= 0.5 * (p->m_mass * std::pow(p->m_dx, 2) + std::pow(p->m_dy, 2));
			
			switch (p->get_name())
			{
			case Particles::First:
				membership[x][y][0]++;
				break;
			case Particles::Second:
				membership[x][y][1]++;
				break;
			case Particles::Product:
				membership[x][y][2]++;
				break;
			default:
				break;
			}
		}
	}

	

	const std::size_t m_amount;
	const std::size_t m_fraction;

	const std::size_t m_width;
	const std::size_t m_height;

	sf::RenderWindow m_application;
	sf::Text text;
	sf::Font font;
	std::vector <std::shared_ptr<Particle>> m_particle;


	std::vector <std::vector<int>> field;
	std::vector <std::vector<double>> temperatures;
	std::vector <std::vector<std::vector<std::size_t>>> membership; // A B C

	std::random_device rd;
	std::mt19937 mersenne;
	std::uniform_real_distribution<double> uidx;
	std::uniform_real_distribution<double> uidy;
	std::uniform_real_distribution<double> uidv;
};
