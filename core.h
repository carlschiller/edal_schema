//
// Created by Carl Schiller on 2018-06-07.
//

#ifndef EDAL_SCHEMA_CORE_H
#define EDAL_SCHEMA_CORE_H

// Available genders.
enum class Genders{
    SEX_TYPE_MALE,
    SEX_TYPE_FEMALE,
    SEX_TYPE_APPACHE_HELICOPTER
};

// Types of available work positions.
enum class Positions{
    FULL_TIME,
    HALF_TIME,
    TEMPORARY_POSITION,
    PARENT_LEAVE,
    WEEKEND_WORKER,
    EXTRA_WORKER
};

class Worker;


#endif //EDAL_SCHEMA_CORE_H
