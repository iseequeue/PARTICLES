#include "Calculator.hpp"
#include "mymath.hpp"
namespace myproject
{

	bool is_collide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
	{
		auto d = (p2->m_x - p1->m_x) * (p2->m_x - p1->m_x) + (p2->m_y - p1->m_y) * (p2->m_y - p1->m_y);
		return d <= (p1->m_radius + p2->m_radius) * (p1->m_radius + p2->m_radius);
	}

	


	void ellastic_collide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2)
	{
		auto d = sqr(p2->m_x - p1->m_x, p2->m_y - p1->m_y);
		if (is_collide(p1, p2) && d!=0)
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


	void non_ellastic_collide(std::size_t& i, std::size_t& j, std::vector<std::shared_ptr<Particle>> &m_particle)
	{
		auto& p1 = m_particle[i];
		auto& p2 = m_particle[j];


		if (is_collide(p1, p2))
		{
			auto vx = (p1->m_mass * p1->m_dx + p2->m_mass * p2->m_dx) / (p1->m_mass + p2->m_mass);

			auto vy = (p1->m_mass * p1->m_dy + p2->m_mass * p2->m_dy) / (p1->m_mass + p2->m_mass);

			double dK = 0.5 * p1->m_mass * p2->m_mass / (p1->m_mass + p2->m_mass) * sqr(p1->m_dx - p2->m_dx, p1->m_dy - p2->m_dy);

			if (p1->get_name() == Particles::First)
			{
				p1 = std::make_shared<Product>((p1->m_x + p2->m_x) / 2, (p1->m_y + p2->m_y) / 2, vx, vy, p1->m_mass + p2->m_mass,
					Constants::r3, std::chrono::steady_clock::now(), p1->m_dx, p1->m_dy, p2->m_dx, p2->m_dy);
			}
			if (p2->get_name() == Particles::First)
			{
				p1 = std::make_shared<Product>((p1->m_x + p2->m_x) / 2, (p1->m_y + p2->m_y) / 2, vx, vy, p1->m_mass + p2->m_mass,
					Constants::r3, std::chrono::steady_clock::now(), p2->m_dx, p2->m_dy, p1->m_dx, p1->m_dy);
			}

			m_particle.erase(std::next(m_particle.begin(), j));

			p1->set_dK(dK);

			j--;
		}
	}

	void boom(std::shared_ptr<Particle> &p, int j, std::vector< std::shared_ptr<Particle>>& m_particle)
	{

		double x1, y1, x2, y2;

		if (p->get_dx01() >= p->get_dx02())
		{
			x1 = p->m_x + p->m_radius;
			x2 = p->m_x - p->m_radius;
		}
		else
		{
			x1 = p->m_x - p->m_radius;
			x2 = p->m_x + p->m_radius;
		}

		if (p->get_dy01() >= p->get_dy02())
		{
			y1 = p->m_y + p->m_radius;
			y2 = p->m_y - p->m_radius;
		}
		else
		{
			y1 = p->m_y - p->m_radius;
			y2 = p->m_y + p->m_radius;
		}


		auto q2 = std::make_shared<Reagent>(x2, y2, p->get_dx02(), p->get_dy02(),
			Constants::m2, Constants::r2, Particles::Second);

		p = std::make_shared<Reagent>(x1, y1, p->get_dx01(), p->get_dy01(), 
			Constants::m1, Constants::r1, Particles::First);

		m_particle.push_back(q2);
	}

	//void internal_for(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t i, std::size_t N)
	//{
	//	for (auto j = i + 1; j < N; j++)
	//	{
	//		//lg(m_particle[i], m_particle[j]);
	//		auto& p1 = m_particle[i];
	//		auto& p2 = m_particle[j];

	//		double dK = 0.5 * sqr(p1->m_dx - p2->m_dx, p1->m_dy - p2->m_dy) * p1->m_mass * p2->m_mass / (p1->m_mass + p2->m_mass);


	//		if (is_collide(p1, p2))
	//		{
	//			if (p1->get_name() == p2->get_name())
	//			{
	//				ellastic_collide(p1, p2);
	//			}

	//			if (p1->get_type() == Particles::Reagent && p2->get_name() == Particles::Product)
	//			{
	//				ellastic_collide(p1, p2);

	//				if (dK > Constants::E)
	//				{
	//					boom(p2, j, m_particle);

	//				}
	//				else
	//				{
	//					p2->set_dK(-1.0);
	//				}

	//			}
	//			if (p1->get_name() == Particles::Product && p2->get_type() == Particles::Reagent)
	//			{
	//				ellastic_collide(p1, p2);
	//				if (dK > Constants::E)
	//				{
	//					boom(p1, i, m_particle);
	//				}
	//				else
	//				{
	//					p1->set_dK(-1.0);
	//				}

	//			}

	//			if (p1->get_name() == Particles::First && p2->get_name() == Particles::Second ||
	//				p1->get_name() == Particles::Second && p2->get_name() == Particles::First)
	//			{


	//				if (dK > Constants::E)
	//				{
	//					non_ellastic_collide(i, j, m_particle);
	//				}
	//				else
	//				{
	//					ellastic_collide(p1, p2);
	//				}

	//			}
	//		}
	//	}

	//}

	//void shift(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t i, std::size_t N, std::size_t m_width, std::size_t m_height)
	//{
	//	for (auto j = i; j < N; j++)
	//	{
	//		m_particle[j]->m_x += m_particle[j]->m_dx;

	//		m_particle[j]->m_y += m_particle[j]->m_dy;

	//		if (m_particle[j]->m_x + m_particle[j]->m_radius > m_width || m_particle[j]->m_x - m_particle[j]->m_radius < 0.0)
	//		{
	//			m_particle[j]->m_dx = -m_particle[j]->m_dx;
	//		}

	//		if (m_particle[j]->m_y + m_particle[j]->m_radius > m_height || m_particle[j]->m_y - m_particle[j]->m_radius < 0.0)
	//		{
	//			m_particle[j]->m_dy = -m_particle[j]->m_dy;
	//		}

	//	}
	//}

	void IDEAL_GAS_Calculation::ideal_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height)
	{
		for (auto i = 0U; i < m_particle.size() - 1; i++)
		{
			//auto length = m_particle.size() - i;

			//const std::size_t min_per_thread = 25;
			//const std::size_t max_threads = (length + min_per_thread - 1) / min_per_thread;

			//const std::size_t hardware_threads = std::thread::hardware_concurrency();

			//const std::size_t num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

			//const std::size_t block_size = length / num_threads;

			//for (std::size_t q = 0; q < (num_threads - 1); ++q)
			//{
			//	//pool.enqueue(boost::bind(&internal_for, std::ref(m_particle), i + block_size * q, i + block_size * (q + 1)));
			//	internal_for(m_particle, i + block_size * q, i + block_size * (q + 1));
			//}
			///*pool.enqueue(boost::bind(&internal_for, std::ref(m_particle), i + block_size * (num_threads - 1), i + length));
			//pool.wait();*/
			for (auto j = i + 1; j < m_particle.size(); j++)
			{
				//lg(m_particle[i], m_particle[j]);
				auto& p1 = m_particle[i];
				auto& p2 = m_particle[j];

				double dK = 0.5 * sqr(p1->m_dx - p2->m_dx, p1->m_dy - p2->m_dy) * p1->m_mass * p2->m_mass / (p1->m_mass + p2->m_mass);


				if (is_collide(p1, p2))
				{
					if (p1->get_name() == p2->get_name())
					{
						ellastic_collide(p1, p2);
					}

					if (p1->get_type() == Particles::Reagent && p2->get_name() == Particles::Product)
					{
						ellastic_collide(p1, p2);

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
						ellastic_collide(p1, p2);
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
							non_ellastic_collide(i, j, m_particle);
						}
						else
						{
							ellastic_collide(p1, p2);
						}

					}
				}
			}
		}

		//auto length = m_particle.size();

		//const std::size_t min_per_thread = 25;
		//const std::size_t max_threads = (length + min_per_thread - 1) / min_per_thread;

		//const std::size_t hardware_threads = std::thread::hardware_concurrency();

		//const std::size_t num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

		//const std::size_t block_size = length / num_threads;


		/*for (std::size_t q = 0; q < (num_threads - 1); ++q)
		{

			pool.enqueue(boost::bind(&shift, std::ref(m_particle), block_size * (q - 1), block_size * q, m_width, m_height));

		}
		pool.wait();*/

		


		std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();
		for (auto i = 0U; i < m_particle.size(); i++)
		{
			if (m_particle[i]->get_type() == Particles::Product)
			{
				if (std::chrono::duration_cast<std::chrono::microseconds>(time_point - m_particle[i]->get_birth()) > Constants::period
					&& m_particle[i]->get_dK() > 0.0)
				{
					boom(m_particle[i], i, m_particle);
				}
			}

		}

		for (auto p : m_particle)
		{
			p->m_x += p->m_dx;

			p->m_y += p->m_dy;

			if (p->m_x + p->m_radius > m_width || p->m_x - p->m_radius < 0.0)
			{
				p->m_dx = -p->m_dx;
			}

			if (p->m_y + p->m_radius > m_height || p->m_y - p->m_radius < 0.0)
			{
				p->m_dy = -p->m_dy;
			}

		}
	}

}



