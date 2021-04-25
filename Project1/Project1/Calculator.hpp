#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "Particle.hpp"

// Class Hierarchy for Gas Model
class Calculation
{
public:
    virtual ~Calculation() {}
    virtual void calculate(const std::string& file) = 0;

    void ellastic_collision(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2);

    void unellastic_collision(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2);

    void calc_motion();

    void calc_collisions();

    void reduction_phase();
};

class IDEAL_GAS_Calculation : public Calculation
{
public:
    void calculate(const std::string& file)
    {
        std::cout << "IDEAL_GAS calculation" << std::endl;
    }
};

class LJ_GAS_Calculation : public Calculation
{
public:
    void calculate(const std::string& file)
    {
        std::cout << "LJ_GAS calculation" << std::endl;
    }
};


// Class for using
class Calculator
{
public:
    Calculator(Calculation* comp) : p(comp) {}
    ~Calculator() { }
    void calculate(const std::string& file)
    {
        p->calculate(file);
    }
private:
    std::shared_ptr<Calculation>  p;
};



//int main()
//{
//    auto p = std::make_shared<Calculator>(IDEAL_GAS_Calculation());
//
//    auto q = std::make_shared<Calculator>(LJ_GAS_Calculation());
//
//
//    p->calculate("file.txt");
//
//    q->calculate("qfile.txt");
//
//    return 0;
//} 
