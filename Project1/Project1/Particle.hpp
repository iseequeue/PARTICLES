#pragma once
#include <memory>

#include "json.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <istream>
#include <locale>
#include <ostream>
#include <iostream>

struct Constants
{
	static const inline double E = 1.0909;
	static const inline std::chrono::microseconds period = static_cast<std::chrono::microseconds>(500000);
	static const inline double r1 = 6.0;
	static const inline double r2 = 6.0;
	static const inline double m1 = 1.0;
	static const inline double m2 = 1.0;

	static const inline const double a = 0.001;
	static const inline const double d = 1e23;
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

	double m_x, m_y;
	double m_dx, m_dy;

	double m_mass;
	double m_radius;

};


class Reagent :public Particle
{
public:

	Reagent(double x, double y, double dx, double dy, double mass, double radius, Particles name) :
		Particle(x, y, dx, dy, mass, radius), m_name(name), m_type(Particles::Reagent)
	{}

	Particles m_type;
	Particles m_name;
	
};


class Product :public Particle
{
public:
	Product(double x, double y, double dx, double dy, double mass, double radius, Particles name) :
		Particle(x, y, dx, dy, mass, radius), m_name(name), m_type(Particles::Reagent), birth(std::chrono::steady_clock::now()), dK(0.0)
	{}

	Particles m_type;
	Particles m_name;

	std::chrono::steady_clock::time_point birth;
	double dK;

};


