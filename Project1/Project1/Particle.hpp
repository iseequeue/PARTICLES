#pragma once
#include <memory>
#include <filesystem>

namespace myproject
{

	struct Constants
	{
		static const inline double E = 1.0909;
		static const inline std::chrono::microseconds period = static_cast<std::chrono::microseconds>(500000);
		static const inline double r1 = 6.0;
		static const inline double r2 = 6.0;
		static const inline double m1 = 1.0;
		static const inline double m2 = 1.0;

		static const inline const double a = 0.001; // for LG
		static const inline const double d = 1e23;  // for LG
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
		explicit Product(double x, double y, double dx, double dy, double mass, double radius, Particles name) :
			Particle(x, y, dx, dy, mass, radius), m_name(name), m_type(Particles::Reagent), birth(std::chrono::steady_clock::now()), dK(0.0)
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

		virtual std::chrono::steady_clock::time_point get_birth() const { return birth; };

		std::chrono::steady_clock::time_point birth;
		double dK;

	};


}