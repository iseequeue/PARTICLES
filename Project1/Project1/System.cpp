#include "System.hpp"

void System::run()
{
	try
	{
		auto path = std::filesystem::current_path();
		std::filesystem::create_directory(path / "directory_1");
		path /= "directory_1";

		IDEAL_GAS_Calculation ig;

		std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();

		const std::chrono::microseconds delay(50000);

		std::chrono::microseconds timer(0);

		while (m_application.isOpen())
		{
			timer += std::chrono::duration_cast <std::chrono::microseconds> (std::chrono::steady_clock::now() - time_point);

			time_point = std::chrono::steady_clock::now();

			sf::Event event;

			while (m_application.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					m_application.close();
				}
			}

			text.setString("Temperature: " + std::to_string(temperature()));

			for (auto i = 0u; i < m_fraction; i++)
			{
				for (auto j = 0u; j < m_fraction; j++)
				{
					sf::RectangleShape rectangle(sf::Vector2f(m_width * 1.0f / m_fraction, m_height * 1.0f / m_fraction));

					rectangle.setPosition(i * m_width * 1.0f / m_fraction, j * m_height * 1.0f / m_fraction);

					rectangle.setFillColor(sf::Color(static_cast<sf::Uint8>(std::abs(255 - field[i][j] * 20)), 255, 255));

					m_application.draw(rectangle);

					field[i][j] = 0;
				}
			}

			ig.iDeal_gas(m_particle, m_width, m_height);

			for (auto i = 0U; i < m_particle.size(); i++)
			{
				sf::CircleShape circle(m_particle[i]->m_radius * 1.0);
				circle.setPosition(static_cast<float>(m_particle[i]->m_x - m_particle[i]->m_radius),
					static_cast<float>(m_particle[i]->m_y - m_particle[i]->m_radius));
				if (m_particle[i]->get_name() == Particles::First)
				{
					circle.setFillColor(sf::Color::Blue);
				}
				if (m_particle[i]->get_name() == Particles::Second)
				{
					circle.setFillColor(sf::Color::Magenta);
				}
				if (m_particle[i]->get_name() == Particles::Product)
				{
					circle.setFillColor(sf::Color::Black);
				}
				m_application.draw(circle);

				field[static_cast<int>(m_particle[i]->m_x / (m_width / m_fraction)) % m_fraction]
					[static_cast<int>(m_particle[i]->m_y / (m_height / m_fraction)) % m_fraction]++; 
			}
			m_application.draw(text);
			m_application.display();

			std::filesystem::path path_output = path/"output.txt";
			std::fstream fout(path_output.string(), std::ios::out);

			for (const auto &p : m_particle)
			{
				nlohmann::json j;
				initialize_json(j, p);
				fout << std::setw(4) << j << std::endl;
			}			

		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Undefinded\n";
	}
}

void initialize_json(nlohmann::json& j, std::shared_ptr<Particle> p)
{
	j["x"] = p->m_x;
	j["y"] = p->m_y;
	j["radius"] = p->m_radius;
}