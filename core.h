//
// Created by Carl Schiller on 2018-06-07.
//

#ifndef EDAL_SCHEMA_CORE_H
#define EDAL_SCHEMA_CORE_H

#include <vector>

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

// Worker class, each object type of Worker will contain all neccessary information about a person.
class Worker{
private:
    std::string m_worker_name;
    Genders m_worker_sex;
    Positions m_position;
    long m_personal_number;
public:
    Worker(std::string, Genders, Positions, long);

    std::string get_name();
    Genders get_gender();
    Positions get_position();
    long get_personal_number();

    void change_name(std::string);
    void change_gender(Genders);
    void change_position(Positions);
    void change_personal_number(long);
};

// Work day class, contains all necessary information about the workday.
class Work_day{
private:
    std::vector<Worker> m_worker_list;
public:
    Work_day();

    void add_worker(Worker new_worker);
    void remove_worker(const std::string &worker_name);
    Worker find_worker(const std::string &worker_name);
};



#endif //EDAL_SCHEMA_CORE_H
