#include <iostream>
#include <string>

#include "Calculator.hpp"
#include "Connection.hpp"
#include "Particle.hpp"
#include "System.hpp"

class A
{
public:
	A(int xx) :x(xx) {};
	int get() { return x; };
	void set(int t) { x = t; };
private:
	int x;
};

class B: public A
{
public:
	B(int yy, int xx) :y(yy), A(xx) 
	{};
private:
	int y;
};

int main()
{
	B b(9, 5);
	b.set(7);
	std::cout << b.get() << std::endl;
}