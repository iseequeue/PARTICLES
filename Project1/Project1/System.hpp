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


#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

#include <istream>
#include <locale>
#include "logger .hpp"
#include <ostream>
#include <iostream>

#include <boost/boost/multi_array.hpp>

namespace myproject
{
	void initialize_json(nlohmann::json& j, std::shared_ptr<Particle> p);

	class System
	{
	public:

		explicit System(std::size_t fraction = 6, std::size_t width = sf::VideoMode::getDesktopMode().width,
			std::size_t height = sf::VideoMode::getDesktopMode().height, std::size_t a = 15, std::size_t b = 15, std::size_t c = 0 ) :
			m_amount(a+b+c),
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
			amount_a(a),
			amount_b(b),
			amount_c(c)


		{
			m_particle.reserve(m_amount);
			double x, y;

			/*m_particle.push_back(std::make_shared<Reagent>(900, 500, 2, 0, Constants::m1, Constants::r1, Particles::First));

			m_particle.push_back(std::make_shared<Product>(500, 500, -2, 0, Constants::m1 + Constants::m2, Constants::r3,
				Particles::Product, std::chrono::steady_clock::now() - Constants::period * 2));*/

			

			for (auto i = 0U; i < amount_a; i++)
			{
				x = uidx(mersenne);
				y = uidy(mersenne);

				m_particle.push_back(std::make_shared<Reagent>(x, y, uidv(mersenne), uidv(mersenne), Constants::m1, Constants::r1, Particles::First));
			}

			for (auto i = 0U; i < amount_b; i++)
			{
				x = uidx(mersenne);
				y = uidy(mersenne);

				m_particle.push_back(std::make_shared<Reagent>(x, y, uidv(mersenne), uidv(mersenne), Constants::m2, Constants::r2, Particles::Second));
			}

			/*for (auto i = 0U; i < amount_c; i++)
			{
				x = uidx(mersenne);
				y = uidy(mersenne);

				m_particle.push_back(std::make_shared<Product>(x, y, uidv(mersenne), uidv(mersenne), Constants::m1 + Constants::m2, Constants::r3,
				std::chrono::steady_clock::now() - Constants::period*2));
			}*/


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

	private:
		const std::size_t m_amount;
		const std::size_t m_fraction;

		std::size_t amount_a;
		std::size_t amount_b;
		std::size_t amount_c;

		const std::size_t m_width;
		const std::size_t m_height;

		sf::RenderWindow m_application;
		sf::Text text;
		sf::Font font;
		std::vector <std::shared_ptr<Particle>> m_particle;
	

		boost::multi_array<int, 2> field;
		boost::multi_array<double, 2> temperatures;
		boost::multi_array<std::vector<int>, 2> membership; // A B C


		std::random_device rd;
		std::mt19937 mersenne;
		std::uniform_real_distribution<double> uidx;
		std::uniform_real_distribution<double> uidy;
		std::uniform_real_distribution<double> uidv;


	};
}