#pragma once
#include <memory>
#include "Particle.hpp"
#include <vector>
#include <cmath>

#include "ThreadPool.hpp"
// Class Hierarchy for Gas Model
class Calculation
{
public:
   

    void ellastic_Collide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2);

    void non_ellastic_Collide(std::size_t& i, std::size_t& j, std::vector<std::shared_ptr<Particle>>& m_particle);

    void lindemann(std::size_t& i, std::size_t& j, std::vector<std::shared_ptr<Particle>>& m_particle);

};

class IDEAL_GAS_Calculation : public Calculation
{
public:
       
    void iDeal_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height);
};

class LJ_GAS_Calculation : public Calculation
{
public:

    void lg(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2);

    void lG_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height);
};


