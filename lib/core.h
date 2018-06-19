//
// Created by Carl Schiller on 2018-06-07.
//

#ifndef EDAL_SCHEMA_CORE_H
#define EDAL_SCHEMA_CORE_H

#include <vector>
#include <ctime>
#include <map>

// Available genders.
enum class Genders{
    MALE,
    FEMALE,
    APPACHE_ATTACK_HELICOPTER,
    NUM_OF_GENDERS
};

// Types of available work positions.
enum class Positions{
    FULL_TIME,
    HALF_TIME,
    TEMPORARY_POSITION,
    PARENT_LEAVE,
    WEEKEND_WORKER,
    EXTRA_WORKER,
    NUM_OF_POSITIONS
};

/*
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
    SPOL_HOPPET,
    NUM_OF_TASKS
};
*/

class Tasks{
private:
    std::vector<std::string> m_task_list;
public:
    Tasks();
    std::vector<std::string> get_all_task_names();
    std::string get_task_name(int);
    int get_task_value(const std::string &);
    static int not_at_work();
    void add_task(std::string);
    void load_tasks_from_file();
};

// Worker class, each object type of Worker will contain all neccessary information about a person.
class Worker{
private:
    std::string m_worker_name;
    Genders m_worker_sex;
    Positions m_position;
    long m_personal_number;
    int m_date_id;
public:
    Worker(std::string, Genders, Positions, long);

    std::string get_name();
    Genders get_gender();
    Positions get_position();
    long get_personal_number();
    int get_id();

    void change_name(std::string);
    void change_gender(Genders);
    void change_position(Positions);
    void change_personal_number(long);
    void change_id(int);
};

// Work day class, contains all necessary information about the workday.
class Work_day{
private:
    std::vector<Worker> m_worker_list;
    time_t m_work_day_date;
    std::vector<std::vector<Tasks>> m_work_day_tasks; // vector of tasks.
    int m_resolution; // how many chunks a day should be divided into.
public:
    Work_day();
    Work_day(time_t, int, std::vector<Worker>);

    void add_worker(Worker new_worker);
    void change_resolution(int);
    void remove_worker(const std::string &worker_name);
    int find_worker_id(const std::string &worker_name);
    std::vector<Tasks> view_worker_tasks(const std::string &worker_name);
    std::vector<std::vector<Tasks>> view_all_worker_tasks();
    std::vector<Worker> get_all_workers();
    int get_resolution();

};



#endif //EDAL_SCHEMA_CORE_H
