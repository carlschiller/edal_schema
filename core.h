//
// Created by Carl Schiller on 2018-06-07.
//

#ifndef EDAL_SCHEMA_CORE_H
#define EDAL_SCHEMA_CORE_H

#include <vector>
#include <ctime>

// Available genders.
enum class Genders{
    MALE,
    FEMALE,
    APPACHE_ATTACK_HELICOPTER
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

// Hard coded for now.
enum class Tasks{
    NOT_AVAILABLE,
    AT_WORK,
    SPOL_HERR,
    SPOL_DAM,
    STORSTAED_HERR,
    STORSTAED_DAM,
    AEVENTYR,
    RECEPTION,
    UTE_RECEPTION,
    UTE_1,
    UTE_2,
    UTE_3,
    UTE_4,
    ROND,
    ROND_BEVAKNING,
    ROND_UTE,
    UTEFIX,
    INNEFIX,
    BOTTENSUG_INNE,
    AVRAEKNING,
    GYM,
    KONDITIONSPARK,
    TOALETT_PLAN_1,
    TOALETT_PLAN_23,
    SPOL_25,
    SPOL_50,
    SPOL_AEVENTYR,
    SPOL_AEVENTYRGANG,
    SPOL_HOPPET
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
    time_t m_work_day_date;
    std::vector<Tasks[48]> work_day_tasks; // vector of tasks.
public:
    Work_day();
    Work_day(time_t, std::vector<Worker>);

    void add_worker(Worker new_worker);
    void remove_worker(const std::string &worker_name);
    Worker find_worker(const std::string &worker_name);

};



#endif //EDAL_SCHEMA_CORE_H
