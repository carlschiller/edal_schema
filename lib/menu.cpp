//
// Created by Carl Schiller on 2018-06-11.
//

#include <iostream>
#include "menu.h"
#include "core.h"
#include <cmath>

enum Outer_selections{
    WORKER_MANAGEMENT = 0,
    REFERENCE_MANAGEMENT,
    SCHEDULE_MANAGEMENT,
};

enum Worker_selections{
    DISPLAY_WORK_FORCE = 0,
    ADD_NEW_WORKER_TO_WORK_FORCE,
};