#pragma once
#include <memory>
#include "Particle.hpp"
#include <vector>
#include <cmath>
#include "ThreadPool.hpp"

namespace myproject
{
    class Calculation
    {
    public:
        
        explicit Calculation():
            pool(std::thread::hardware_concurrency()) {}

        ThreadPool pool;

    };

    class IDEAL_GAS_Calculation : public Calculation
    {
    public:

        void ideal_gas(std::vector<std::shared_ptr<Particle>>& m_particle, std::size_t m_width, std::size_t m_height);

    };

}

