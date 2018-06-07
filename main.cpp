#include <iostream>
#include "core.h"

int main() {
    std::string name("Carl Schiller");
    Worker carl = Worker(name, Genders::SEX_TYPE_APPACHE_HELICOPTER, Positions::EXTRA_WORKER, 199705266436);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}