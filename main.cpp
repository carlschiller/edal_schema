#include <iostream>
#include "core.h"

int main() {
    std::string name("Carl Schiller");
    Worker carl(name, Genders::SEX_TYPE_APPACHE_HELICOPTER, Positions::EXTRA_WORKER, 199705266436);
    std::cout << carl.get_name() << std::endl;
    return 0;
}