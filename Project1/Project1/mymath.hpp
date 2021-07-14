#pragma once
#include <vector>
#include <algorithm>

namespace myproject
{
	double sqr(double ax, double ay)
	{
		return ax * ax + ay * ay;
	}


    template <typename T>
    void erase(std::vector<T>& nums, std::vector<size_t>& indexes)
    {
        std::sort(indexes.begin(), indexes.end(), [](size_t lhs, size_t rhs) {return lhs < rhs; });
      
        auto b = indexes.begin();
        auto e = indexes.end();

        // левый
        auto new_iter = nums.begin() + *b++;
        // правый
        auto iter = new_iter + 1;
        size_t cnt = 1;

        while (b < e)
        {
            if (*b == iter - nums.begin())
            {
                ++b;
                ++iter;
                ++cnt;
            }
            else 
            {
                *new_iter++ = *iter++;
            }
        }

        while (iter < nums.end())
        {
            *new_iter++ = *iter++;
        }

        nums.resize(nums.size() - cnt);
    }
}