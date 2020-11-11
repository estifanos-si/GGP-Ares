#include "utils/gdl/variable.hh"
#include "utils/gdl/function.hh"
#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace Ares;
    Variable* v = new Variable("x");
    Function* f = new Function("f",3);
    std::cout << v->getName() << std::endl;
    std::cout << f->getName()<< std::endl;
    return 0;
}
