#pragma once
#include <memory>
#include "Particle.hpp"
#include <vector>
#include <cmath>

namespace myproject
{
    class Calculation
    {
    public:


        void ellastic_collide(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2);

        void non_ellastic_collide(std::size_t& i, std::size_t& j, std::vector<std::shared_ptr<Particle>>& m_particle);


    };

    class IDEAL_GAS_Calculation : public Calculation
    {
    public:

        void ideal_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height);
    };

}

