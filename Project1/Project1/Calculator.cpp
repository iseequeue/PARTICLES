#include "Calculator.hpp"


void Calculation::ellastic_Collide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
{
	auto d = (p2->m_x - p1->m_x) * (p2->m_x - p1->m_x) + (p2->m_y - p1->m_y) * (p2->m_y - p1->m_y);
	if (d <= (p1->m_radius + p2->m_radius) * (p1->m_radius + p2->m_radius))
	{
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


void Calculation::non_ellastic_Collide(std::size_t& i, std::size_t& j, std::vector<std::shared_ptr<Particle>>& m_particle)
{
	auto d = (m_particle[j]->m_x - m_particle[i]->m_x) * (m_particle[j]->m_x - m_particle[i]->m_x) +
		(m_particle[j]->m_y - m_particle[i]->m_y) * (m_particle[j]->m_y - m_particle[i]->m_y);

	if (d <= (m_particle[i]->m_radius + m_particle[j]->m_radius) * (m_particle[i]->m_radius + m_particle[j]->m_radius))
	{
		auto vx = (m_particle[i]->m_mass * m_particle[i]->m_dx + m_particle[j]->m_mass * m_particle[j]->m_dx)
			/ (m_particle[i]->m_mass + m_particle[j]->m_mass);

		auto vy = (m_particle[i]->m_mass * m_particle[i]->m_dy + m_particle[j]->m_mass * m_particle[j]->m_dy)
			/ (m_particle[i]->m_mass + m_particle[j]->m_mass);


		m_particle[i] = std::make_shared<Particle>((m_particle[i]->m_x + m_particle[j]->m_x) / 2, (m_particle[i]->m_y + m_particle[j]->m_y) / 2,
			vx, vy, m_particle[i]->m_mass + m_particle[j]->m_mass,
			std::pow(std::pow(m_particle[i]->m_radius, 3) + std::pow(m_particle[j]->m_radius, 3), 1.0 / 3));
		m_particle.erase(std::next(m_particle.begin(), j));
		j--;
	}
}


void IDEAL_GAS_Calculation::iDeal_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height)
{
	for (auto i = 0U; i < m_particle.size(); i++)
	{
		for (auto j = i; j < m_particle.size(); j++)
		{
			if (i != j)
			{
				non_ellastic_Collide(i, j, m_particle);
				// isCollide(m_particle[i], m_particle[j]);
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
}


void LJ_GAS_Calculation::lg(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
{
	const double a = 0.001;
	const double d = 1e23;
	double dist = std::sqrt((p2->m_x - p1->m_x) * (p2->m_x - p1->m_x) + (p2->m_y - p1->m_y) * (p2->m_y - p1->m_y));

	double ex = (p2->m_x - p1->m_x) / dist;
	double ey = (p2->m_y - p1->m_y) / dist;

	double force = 12 * d * (std::pow(a / dist, 13) - std::pow(a / dist, 7)) / a;

	double ax1 = force * ex / p1->m_mass;
	double ay1 = force * ey / p1->m_mass;

	double ax2 = -force * ex / p2->m_mass;
	double ay2 = -force * ey / p2->m_mass;

	p1->m_dx += ax1;
	p1->m_dy += ay1;

	p2->m_dx += ax2;
	p2->m_dy += ay2;

}


void LJ_GAS_Calculation::lG_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height)
{
	for (auto i = 0U; i < m_particle.size(); i++)
	{
		for (auto j = i; j < m_particle.size(); j++)
		{
			if (i != j)
			{
				non_ellastic_Collide(i, j, m_particle);
				//isCollide(m_particle[i], m_particle[j]);
				lg(m_particle[i], m_particle[j]);
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
}

