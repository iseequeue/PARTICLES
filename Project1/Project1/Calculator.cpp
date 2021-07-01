#include "Calculator.hpp"

bool isCollide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
{
	auto d = (p2->m_x - p1->m_x) * (p2->m_x - p1->m_x) + (p2->m_y - p1->m_y) * (p2->m_y - p1->m_y);
	return d <= (p1->m_radius + p2->m_radius) * (p1->m_radius + p2->m_radius);
}

double sqr(double ax, double ay)
{
	return ax * ax + ay*ay;
}


void Calculation::ellastic_Collide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
{

	if (isCollide(p1, p2))
	{
		auto d = sqr(p2->m_x - p1->m_x, p2->m_y - p1->m_y);

		auto v1x = ((p2->m_x - p1->m_x) * p1->m_dx + (p2->m_y - p1->m_y) * p1->m_dy) / d;
		auto v1y = ((p1->m_y - p2->m_y) * p1->m_dx + (p2->m_x - p1->m_x) * p1->m_dy) / d;

		auto v2x = ((p2->m_x - p1->m_x) * p2->m_dx + (p2->m_y - p1->m_y) * p2->m_dy) / d;
		auto v2y = ((p1->m_y - p2->m_y) * p2->m_dx + (p2->m_x - p1->m_x) * p2->m_dy) / d;

		auto v1x_new = ((p1->m_mass - p2->m_mass) * v1x + 2 * p2->m_mass * v2x) / (p1->m_mass + p2->m_mass);
		auto v2x_new = (2 * p1->m_mass * v1x + (p2->m_mass - p1->m_mass) * v2x) / (p1->m_mass + p2->m_mass);

		p1->m_dx = (p2->m_x - p1->m_x) * v1x_new + (p1->m_y - p2->m_y) * v1y;
		p1->m_dy = (p2->m_y - p1->m_y) * v1x_new + (p2->m_x - p1->m_x) * v1y;

		p2->m_dx = (p2->m_x - p1->m_x) * v2x_new + (p1->m_y - p2->m_y) * v2y;
		p2->m_dy = (p2->m_y - p1->m_y) * v2x_new + (p2->m_x - p1->m_x) * v2y;

	}
}

void lg(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
{

	double dist = std::sqrt((p2->m_x - p1->m_x) * (p2->m_x - p1->m_x) + (p2->m_y - p1->m_y) * (p2->m_y - p1->m_y));

	double ex = (p2->m_x - p1->m_x) / dist;
	double ey = (p2->m_y - p1->m_y) / dist;

	double force = 12 * Constants::d * (std::pow(Constants::a / dist, 13) - std::pow(Constants::a / dist, 7)) / Constants::a;

	double ax1 = force * ex / p1->m_mass;
	double ay1 = force * ey / p1->m_mass;

	double ax2 = -force * ex / p2->m_mass;
	double ay2 = -force * ey / p2->m_mass;

	p1->m_dx += ax1;
	p1->m_dy += ay1;

	p2->m_dx += ax2;
	p2->m_dy += ay2;

}


void Calculation::non_ellastic_Collide(std::size_t& i, std::size_t& j, std::vector<std::shared_ptr<Particle>>& m_particle)
{
	auto& p1 = m_particle[i];
	auto& p2 = m_particle[j];


	if (isCollide(p1, p2))
	{
		auto vx = (p1->m_mass * p1->m_dx + p2->m_mass * p2->m_dx) / (p1->m_mass + p2->m_mass);

		auto vy = (p1->m_mass * p1->m_dy + p2->m_mass * p2->m_dy) / (p1->m_mass + p2->m_mass);

		double dK = 0.5 * p1->m_mass * p2->m_mass / (p1->m_mass + p2->m_mass) * sqr(p1->m_dx - p2->m_dx, p1->m_dy - p2->m_dy);


		p1 = std::make_shared<Product>((p1->m_x + p2->m_x) / 2, (p1->m_y + p2->m_y) / 2, vx, vy, p1->m_mass + p2->m_mass,
			std::pow(std::pow(p1->m_radius, 3) + std::pow(p2->m_radius, 3), 1.0 / 3), Particles::Product);

		m_particle.erase(std::next(m_particle.begin(), j));

		p1->set_dK(dK);

		j--;
	}
}

void boom (std::shared_ptr<Particle> p, int j, std::vector< std::shared_ptr<Particle>> & m_particle)
{
	auto v1 = (2 * p->get_dK() + p->m_mass * sqr(p->m_dx, p->m_dy)) * Constants::m1 * Constants::m2
		/ (Constants::m1 + Constants::m2);

	auto q1 = std::make_shared<Reagent>(p->m_x + Constants::r1 * 1.1, p->m_y + Constants::r1 * 1.1, std::pow(0.5 * v1, 0.5), std::pow(0.5 * v1, 0.5), Constants::m1, Constants::r1, Particles::First);

	auto q2 = std::make_shared<Reagent>(p->m_x - Constants::r2 * 1.1, p->m_y - Constants::r2 * 1.1, -std::pow(0.5 * v1 * Constants::m2 / Constants::m1, 0.5), -std::pow(0.5 * v1 * Constants::m2 / Constants::m1, 0.5), Constants::m2,
		Constants::r2, Particles::Second);

	m_particle.erase(std::next(m_particle.begin(), j));
	m_particle.insert(std::next(m_particle.begin(), j), q2);
	m_particle.insert(std::next(m_particle.begin(), j), q1);
}


void IDEAL_GAS_Calculation::iDeal_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height)
{
	for (auto i = 0U; i < m_particle.size() - 1; i++)
	{
		for (auto j = i + 1; j < m_particle.size(); j++)
		{	
			//lg(m_particle[i], m_particle[j]);
			auto& p1 = m_particle[i];
			auto& p2 = m_particle[j];

			double dK = 0.5 * sqr(p1->m_dx - p2->m_dx, p1->m_dy - p2->m_dy)* p1->m_mass * p2->m_mass / (p1->m_mass + p2->m_mass);


			if (isCollide(p1, p2))
			{
				if (p1->get_name() == p2->get_name())
				{
					ellastic_Collide(p1, p2);
				}

				if (p1->get_type() == Particles::Reagent && p2->get_name() == Particles::Product)
				{
					ellastic_Collide(p1, p2);

					if (dK > Constants::E)
					{
						boom(p2, j, m_particle);
							
					}
					else
					{
						p2->set_dK(-1.0);
					}

				}
				if (p1->get_name() == Particles::Product && p2->get_type() == Particles::Reagent)
				{
					ellastic_Collide(p1, p2);
					if (dK > Constants::E)
					{
						boom(p1, i, m_particle);
					}
					else
					{
						p1->set_dK(-1.0);
					}

				}

				if (p1->get_name() == Particles::First && p2->get_name() == Particles::Second ||
					p1->get_name() == Particles::Second && p2->get_name() == Particles::First)
				{


					if (dK > Constants::E)
					{
						non_ellastic_Collide(i, j, m_particle);
					}
					else
					{
						ellastic_Collide(p1, p2);
					}

				}
			}			
		}
	}
	

	

	for (auto i = 0U; i < m_particle.size(); i++)
	{
		m_particle[i]->m_x += m_particle[i]->m_dx;

		m_particle[i]->m_y += m_particle[i]->m_dy;

		if (m_particle[i]->m_x + m_particle[i]->m_radius > m_width || m_particle[i]->m_x - m_particle[i]->m_radius < 0.0)
		{
			m_particle[i]->m_dx = -m_particle[i]->m_dx;
		}

		if (m_particle[i]->m_y + m_particle[i]->m_radius > m_height || m_particle[i]->m_y - m_particle[i]->m_radius < 0.0)
		{
			m_particle[i]->m_dy = -m_particle[i]->m_dy;
		}

	}
	std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();
	for (auto i = 0U; i < m_particle.size(); i++)
	{
		if (m_particle[i]->get_name() == Particles::Product)
		{
			if (std::chrono::duration_cast<std::chrono::microseconds>(time_point - m_particle[i]->get_birth()) > Constants::period
				&& m_particle[i]->get_dK() > 0.0)
			{
				boom(m_particle[i], i, m_particle);
			}
		}

	}
}





