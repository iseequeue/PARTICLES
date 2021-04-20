#include <iostream>
#include <string>

// Class Hierarchy for Gas Model
class Calculation
{
public:
    virtual ~Calculation() {}
    virtual void calculate(const std::string& file) = 0;
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
    ~Calculator() { delete p; }
    void calculate(const std::string& file)
    {
        p->calculate(file);
    }
private:
    Calculation* p;
};


/*
int main()
{
    Calculator* p = new Calculator(new IDEAL_GAS_Calculation);

    Calculator* q = new Calculator(new LJ_GAS_Calculation);


    p->calculate("file.txt");

    q->calculate("qfile.txt");

    delete p;
    delete q;
    return 0;
} */