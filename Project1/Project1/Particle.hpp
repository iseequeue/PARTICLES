#pragma once
#include "json.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <istream>
#include <locale>
#include <ostream>
#include <iostream>
class Particle
{
public:

	Particle(double x, double y, double dx, double dy, double mass, double radius) :
		m_x(x), m_y(y), m_dx(dx), m_dy(dy), m_mass(mass), m_radius(radius)
	{}

	double m_mass;
	double m_radius;
	double m_x, m_y;
	double m_dx, m_dy;

};



//#include <chrono>
//#include <iostream>
//#include <vector>
//#include <memory>
//
//// Class Hierarchy for particles
//class Particle
//{
//public:
//    using clock_t = std::chrono::steady_clock;
//    using time_point_t = clock_t::time_point;
//
//    virtual void info() = 0;
//    virtual ~Particle() {}
//
//public:
//    double m_x, m_y, m_vx, m_vy, m_ax, m_ay;
//    double m_mass, m_radius;
//    time_point_t m_birth;
//};
//
//class Nitrogen : public Particle
//{
//public:
//    void info()
//    {
//        std::cout << "Nitrogen" << std::endl;
//    };
//};
//
//class Oxigen : public Particle
//{
//public:
//    void info()
//    {
//        std::cout << "Oxigen" << std::endl;
//    };
//};
//
//class Nitrogen_Oxide : public Particle
//{
//public:
//    void info()
//    {
//        std::cout << "Nitrogen_Oxide" << std::endl;
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
//class NitrogenFactory : public Factory
//{
//public:
//    Particle* createParticle()
//    {
//        return new Nitrogen;
//    }
//};
//
//class OxigenFactory : public Factory
//{
//public:
//    Particle* createParticle()
//    {
//        return new Oxigen;
//    }
//};
//
//class Nitrogen_OxideFactory : public Factory
//{
//public:
//    Particle* createParticle()
//    {
//        return new Nitrogen_Oxide;
//    }
//};
//
//
//// Создание объектов при помощи фабрик объектов
///*
//int main()
//{
//    NitrogenFactory* n_factory = new NitrogenFactory;
//    // std::shared_ptr<NitrogenFactory> ptr1(n_factory);
//    OxigenFactory* o_factory = new OxigenFactory;
//    Nitrogen_OxideFactory* no_factory = new Nitrogen_OxideFactory;
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