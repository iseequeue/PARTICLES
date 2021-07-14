#pragma once
#include <memory>
#include <filesystem>

namespace myproject
{

	struct Constants
	{
		static const inline double E = 0.1909;
		static const inline std::chrono::microseconds period = static_cast<std::chrono::microseconds>(500000);
		static const inline double r1 = 6.0;
		static const inline double r2 = 6.0;
		static const inline double m1 = 1.0;
		static const inline double m2 = 1.0;
		static const inline double r3 = std::pow(std::pow(r1, 3) + std::pow(r2, 3), 1.0 / 3);

		static const inline double a = 0.001; // for LG
		static const inline double d = 1e23;  // for LG
	};

	enum class Particles
	{
		Reagent,
		Product,
		First,
		Second,
		amount
	};


	class Particle
	{
	public:

		explicit Particle(double x, double y, double dx, double dy, double mass, double radius) noexcept :
			m_x(x), m_y(y), m_dx(dx), m_dy(dy), m_mass(mass), m_radius(radius)
		{}

		virtual ~Particle() noexcept = default;

		auto energy() const noexcept
		{
			return 0.5 * m_mass * (m_dx * m_dx + m_dy * m_dy);
		}

		virtual Particles get_type() const { return Particles::amount; };
		virtual Particles get_name() const { return Particles::amount; };

		virtual double get_dK() const { return 0.0; };
		virtual void set_dK(double x) {};

		virtual double get_dx01() const { return 0.0; };
		virtual double get_dy01() const { return 0.0; };
		virtual double get_dx02() const { return 0.0; };
		virtual double get_dy02() const { return 0.0; };

		virtual std::chrono::steady_clock::time_point get_birth() const { return std::chrono::steady_clock::now(); };

		double m_x, m_y;
		double m_dx, m_dy;

		double m_mass;
		double m_radius;

	};


	class Reagent :public Particle
	{
	public:

		explicit Reagent(double x, double y, double dx, double dy, double mass, double radius, Particles name) :
			Particle(x, y, dx, dy, mass, radius), m_name(name), m_type(Particles::Reagent)
		{}
		~Reagent() noexcept = default;

		Particles m_type;
		Particles m_name;

		Particles get_type() const override
		{
			return m_type;
		}
		Particles get_name() const override
		{
			return m_name;
		}

	};


	class Product :public Particle
	{
	public:
		explicit Product(double x, double y, double dx, double dy, double mass, double radius, std::chrono::steady_clock::time_point b,
			double dx01, double dy01, double dx02, double dy02) :
			Particle(x, y, dx, dy, mass, radius), m_name(Particles::Product), m_type(Particles::Product), birth(b), dK(0.0),
			m_dx01(dx01), m_dy01(dy01), m_dx02(dx02), m_dy02(dy02)
		{}

		~Product() noexcept = default;

		Particles m_type;
		Particles m_name;

		Particles get_type() const override
		{
			return m_type;
		}
		Particles get_name() const override
		{
			return m_name;
		}

		double get_dK() const override { return dK; };
		void set_dK(double x) override { dK = x; };

		double get_dx01() const override { return m_dx01; };
		double get_dy01() const override { return m_dy01; };
		double get_dx02() const override { return m_dx02; };
		double get_dy02() const override { return m_dy02; };

		virtual std::chrono::steady_clock::time_point get_birth() const { return birth; };

		std::chrono::steady_clock::time_point birth;
		double dK;

		double m_dx01, m_dy01, m_dx02, m_dy02;

	};


}