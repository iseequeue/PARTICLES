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
	double E = 1.0909;
	std::chrono::microseconds period = static_cast<std::chrono::microseconds>(500000);
	double r1 = 6.0;
	double r2 = 6.0;
	double m1 = 1.0;
	double m2 = 1.0;
};

enum class Particles
{
	Reagent,
	Product,
	First,
	Second,
};


class Particle
{
public:

	Particle(double x, double y, double dx, double dy, double mass, double radius) :
		m_x(x), m_y(y), m_dx(dx), m_dy(dy), m_mass(mass), m_radius(radius)
	{}

	double energy()
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
	Product(double x, double y, double dx, double dy, double mass, double radius, Particles name, double dx1, double dy1, double dx2, double dy2) :
		Particle(x, y, dx, dy, mass, radius), m_name(name), m_type(Particles::Reagent), birth(std::chrono::steady_clock::now()), dK(0.0),
		m_dx1(dx1), m_dy1(dy1), m_dx2(dx2), m_dy2(dy2)
	{}

	Particles m_type;
	Particles m_name;

	std::chrono::steady_clock::time_point birth;
	double dK;

	double m_dx1;
	double m_dy1;

	double m_dx2;
	double m_dy2;

};


//// Class Hierarchy for particles
//class Particle
//{
//public:
//
//    virtual void info() = 0;
//    virtual ~Particle() {}
//
//};
//
//class First : public Particle
//{
//public:
//    void info()
//    {
//        std::cout << "First" << std::endl;
//    };
//};
//
//class Second : public Particle
//{
//public:
//    void info()
//    {
//        std::cout << "Second" << std::endl;
//    };
//};
//
//class Producte : public Particle
//{
//public:
//    void info()
//    {
//        std::cout << "Producte" << std::endl;
//    };
//};
//
//
//// Objects Factories
//class Factory
//{
//public:
//    virtual Particle* createParticle() = 0;
//    virtual ~Factory() {}
//};
//
//class FirstFactory : public Factory
//{
//public:
//    Particle* createParticle()
//    {
//        return new First;
//    }
//};
//
//class SecondFactory : public Factory
//{
//public:
//    Particle* createParticle()
//    {
//        return new Second;
//    }
//};
//
//class ProductFactory : public Factory
//{
//public:
//    Particle* createParticle()
//    {
//        return new Producte;
//    }
//};


//// Создание объектов при помощи фабрик объектов
///*
//int main()
//{
//    FirstFactory* n_factory = new FirstFactory;
//    // std::shared_ptr<FirstFactory> ptr1(n_factory);
//    SecondFactory* o_factory = new SecondFactory;
//    ProducteFactory* no_factory = new ProducteFactory;
//
//    std::vector<Particle*> v;
//    v.push_back(n_factory->createParticle());
//    v.push_back(o_factory->createParticle());
//    v.push_back(no_factory->createParticle());
//
//    for (int i = 0; i < v.size(); i++)
//        v[i]->info();
//    // ...
//}
//*/