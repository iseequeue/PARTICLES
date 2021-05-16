#include "Calculator.hpp"

bool isCollide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
{
	auto d = (p2->m_x - p1->m_x) * (p2->m_x - p1->m_x) + (p2->m_y - p1->m_y) * (p2->m_y - p1->m_y);
	return d <= (p1->m_radius + p2->m_radius) * (p1->m_radius + p2->m_radius);
}

double sqr(double ax, double ay, double bx, double by)
{
	return (ax - bx) * (ax - bx) + (ay - by) * (ay - by);
}

void Calculation::ellastic_Collide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
{

	if (isCollide(p1, p2))
	{
		auto d = (p2->m_x - p1->m_x) * (p2->m_x - p1->m_x) + (p2->m_y - p1->m_y) * (p2->m_y - p1->m_y);

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
	auto& p1 = m_particle[i];
	auto& p2 = m_particle[j];


	if (isCollide(p1, p2))
	{
		auto vx = (p1->m_mass * p1->m_dx + p2->m_mass * p2->m_dx) / (p1->m_mass + p2->m_mass);

		auto vy = (p1->m_mass * p1->m_dy + p2->m_mass * p2->m_dy) / (p1->m_mass + p2->m_mass);

		double dK = 0.5 * p1->m_mass * p2->m_mass / (p1->m_mass * p2->m_mass) * sqr(p1->m_dx, p1->m_dy, p2->m_dx, p2->m_dy);


		p1 = std::make_shared<Product>((p1->m_x + p2->m_x) / 2, (p1->m_y + p2->m_y) / 2, vx, vy, p1->m_mass + p2->m_mass,
			std::pow(std::pow(p1->m_radius, 3) + std::pow(p2->m_radius, 3), 1.0 / 3), Particles::Product, p1->m_dx, p1->m_dy, p2->m_dx, p2->m_dy);

		m_particle.erase(std::next(m_particle.begin(), j));

		std::static_pointer_cast<Product>(p1)->dK = dK;

		j--;
	}
}

//void Calculation::lindemann(std::size_t& i, std::size_t& j, std::vector<std::shared_ptr<Particle>>& m_particle)
//{
//	auto& p1 = m_particle[i];
//	auto& p2 = m_particle[j];
//
//	auto dist = (p2->m_x - p1->m_x) * (p2->m_x - p1->m_x) + (p2->m_y - p1->m_y) * (p2->m_y - p1->m_y);
//
//	if (dist <= (p1->m_radius + p2->m_radius) * (p1->m_radius + p2->m_radius))
//	{
//		auto vx = (p1->m_mass * p1->m_dx + p2->m_mass * p2->m_dx) / (p1->m_mass + p2->m_mass);
//
//		auto vy = (p1->m_mass * p1->m_dy + p2->m_mass * p2->m_dy) / (p1->m_mass + p2->m_mass);
//		if (std::static_pointer_cast<)
//		if (p1->energy() + p2->energy() - (p1->m_mass + p2->m_mass) * (vx * vx + vy * vy) < e_activation)
//		{
//			ellastic_Collide(p1, p2);
//		}
//	}
//
//}

void IDEAL_GAS_Calculation::iDeal_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height)
{
	Constants constants;

	for (auto i = 0U; i < m_particle.size(); i++)
	{
		for (auto j = i; j < m_particle.size(); j++)
		{
			if (i != j)
			{
				auto& p1 = m_particle[i];
				auto& p2 = m_particle[j];
				if (std::static_pointer_cast<Reagent>(p1)->m_name == std::static_pointer_cast<Product>(p2)->m_name) 			
				{
					ellastic_Collide(p1, p2);
				}
				if (std::static_pointer_cast<Reagent>(p1)->m_type == Particles::Reagent && std::static_pointer_cast<Product>(p2)->m_name == Particles::Product)
				{
					std::static_pointer_cast<Product>(p2)->dK = -1.0;
					ellastic_Collide(p1, p2);
				}
				if (std::static_pointer_cast<Product>(p1)->m_name == Particles::Product && std::static_pointer_cast<Reagent>(p2)->m_type == Particles::Reagent)
				{
					std::static_pointer_cast<Product>(p1)->dK = -1.0;
					ellastic_Collide(p1, p2);
				}

				if (std::static_pointer_cast<Product>(p1)->m_name == Particles::First && std::static_pointer_cast<Product>(p2)->m_name == Particles::Second ||
					std::static_pointer_cast<Product>(p1)->m_name == Particles::Second && std::static_pointer_cast<Product>(p2)->m_name == Particles::First)
				{
					double dK = 0.5 * p1->m_mass * p2->m_mass / (p1->m_mass * p2->m_mass) * sqr(p1->m_dx, p1->m_dy, p2->m_dx, p2->m_dy);

					if (dK > constants.E)
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
		if (std::static_pointer_cast<Product>(m_particle[i])->m_name == Particles::Product)
		{
			if (std::chrono::duration_cast<std::chrono::microseconds>(time_point - std::static_pointer_cast<Product>(m_particle[i])->birth) > constants.period
				&& std::static_pointer_cast<Product>(m_particle[i])->dK > 0.0)
			{
				auto p = std::static_pointer_cast<Product>(m_particle[i]);
				auto q1 = std::make_shared<Reagent>(p->m_x + constants.r1, p->m_y + constants.r1, p->m_dx1, -p->m_dy1, constants.m1, constants.r1, Particles::Reagent);
				auto q2 = std::make_shared<Reagent>(p->m_x + constants.r2, p->m_y + constants.r2, p->m_dx2, -p->m_dy2, constants.m2, constants.r2, Particles::Reagent);

				m_particle.push_back(q1);

				m_particle.push_back(q2);

				m_particle.erase(std::next(m_particle.begin(), i));
			}
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

