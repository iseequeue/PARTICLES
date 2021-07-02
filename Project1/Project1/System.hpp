#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include <chrono>
#include <ctime>
#include <cstdint>
#include <cassert>
#include <exception>

#include <memory>
#include <vector>
#include <random>

#include "Particle.hpp"
#include "Calculator.hpp"

#include "ThreadPool.hpp"

#include "json.hpp"
#include <filesystem>
#include <fstream>

#include <istream>
#include <locale>
#include <ostream>
#include <iostream>

#include <boost/boost/multi_array.hpp>

namespace myproject
{
	void initialize_json(nlohmann::json& j, std::shared_ptr<Particle> p);

	class System
	{
	public:

		explicit System(std::size_t amount = 51, std::size_t fraction = 6, std::size_t width = sf::VideoMode::getDesktopMode().width,
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
			field(boost::extents[fraction][fraction]),
			temperatures(boost::extents[fraction][fraction]),
			membership(boost::extents[fraction][fraction]),
			pool(std::thread::hardware_concurrency()),
			A(boost::extents[3][4][2])

		{
			m_particle.reserve(m_amount);
			double x1, y1, x2, y2;

			for (auto i = 0U; i < m_amount / 2; i++)
			{
				x1 = uidx(mersenne);
				y1 = uidy(mersenne);

				x2 = uidx(mersenne);
				y2 = uidy(mersenne);

				m_particle.push_back(std::make_shared<Reagent>(x1, y1, uidv(mersenne), uidv(mersenne), Constants::m1, Constants::r1, Particles::First));
				m_particle.push_back(std::make_shared<Reagent>(x2, y2, uidv(mersenne), uidv(mersenne), Constants::m2, Constants::r2, Particles::Second));
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
		~System() noexcept = default;

		void run();


		double temperature() const
		{
			double t = 0.0;
			for (const auto& p : m_particle)
				t += p->energy();
			return t / m_particle.size();
		}

		void recession();

		void calc_fractions();
		void draw_particles();
		void record_jsons(std::fstream& fout) const;

		ThreadPool pool;
	private:
		const std::size_t m_amount;
		const std::size_t m_fraction;

		const std::size_t m_width;
		const std::size_t m_height;

		sf::RenderWindow m_application;
		sf::Text text;
		sf::Font font;
		std::vector <std::shared_ptr<Particle>> m_particle;
	
		boost::multi_array<double, 3> A;

		boost::multi_array<int, 2> field;
		boost::multi_array<double, 2> temperatures;
		boost::multi_array<std::vector<int>, 2> membership; // A B C

		//std::vector <std::vector<int>> field;
		//std::vector <std::vector<double>> temperatures;
		//std::vector <std::vector<std::vector<std::size_t>>> membership; // A B C

		std::random_device rd;
		std::mt19937 mersenne;
		std::uniform_real_distribution<double> uidx;
		std::uniform_real_distribution<double> uidy;
		std::uniform_real_distribution<double> uidv;


	};
}