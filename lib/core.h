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

class Tasks{
private:
    std::map<std::string,int> m_task_map;
public:
    Tasks();
    std::vector<std::string> get_all_task_names();
    std::vector<int> get_all_task_values();
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
    Tasks m_work_day_tasks; // tasks used during this day.
    int m_resolution; // how many chunks a day should be divided into.
    std::vector<std::vector<int>> m_work_day_reference; // matrix of work day tasks to be done.
    std::vector<std::vector<int>> m_worker_task_list; // matrix of tasks for workers
public:
    Work_day();
    Work_day(time_t, int, std::vector<Worker>);

    void add_worker(Worker new_worker);
    void change_resolution(int);
    void remove_worker(const std::string &worker_name);
    int find_worker_id(const std::string &worker_name);
    std::vector<Worker> get_all_workers();
    int get_resolution();
    void add_work_day_reference_column(int task_number, int start_time, int end_time);
    void remove_work_day_reference_column(int id);
    std::vector<std::vector<int>> get_work_day_reference();
    void build_work_day(); // creates m_worker_task_list;
    Tasks get_tasks();
};

#endif //EDAL_SCHEMA_CORE_H
