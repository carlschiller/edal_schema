//
// Created by Carl Schiller on 2018-06-07.
//

#ifndef EDAL_SCHEMA_CORE_H
#define EDAL_SCHEMA_CORE_H

#include <vector>
#include <ctime>
#include <map>
#include <regex>


// Available sex.
enum class Genders{
    NONE = 0,
    MALE,
    FEMALE,
};

// Types of available work positions.
enum class Positions{
    FULL_TIME,
    HALF_TIME,
    TEMPORARY_POSITION,
    PARENT_LEAVE,
    WEEKEND_WORKER,
    EXTRA_WORKER,
};

// Functions for saving and loading.

namespace Converters {
    bool string_to_boolean(const std::string &input);
    Genders string_to_sex(const std::string &input);
    Positions string_to_positions(const std::string &input);
    std::string boolean_to_string(bool input);
    std::string sex_to_string(Genders input);
    std::string positions_to_string(Positions input);
    std::string regex_find_and_replace(const std::string &line_of_text,
                                              const std::regex &reg, const std::string &replacer);
    std::string regex_get_first_match(const std::string &line, const std::regex &reg);
    std::vector<std::string> split_by_delimiter(const std::string &line, char delim);
}

class Tasks{
private:
    std::map<std::string,int> m_task_map; // maps each name of task to an int.
    std::map<std::string,bool> m_task_flexibility; // if worker is able to leave earlier than req. time
    std::map<std::string,Genders> m_task_sex_requirement; // sex specific task [MALE/FEMALE/NULL]
public:
    Tasks();
    std::vector<std::string> get_all_task_names();
    std::vector<int> get_all_task_values();
    std::string get_task_name(int);
    int get_task_value(const std::string &);
    static int not_at_work();
    void add_task(std::string key, bool flexibility, Genders sex);
    void load_tasks_from_file();
    void save_tasks_to_file();
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

    int m_resolution; // how many chunks a day should be divided into.
    std::vector<std::vector<int>> m_work_day_reference; // matrix of work day tasks to be done.
    std::vector<std::vector<int>> m_worker_task_list; // matrix of tasks for workers
    void load_workers_from_file();
    void save_workers_to_file();
public:
    Work_day();
    Work_day(time_t, int);

    Tasks work_day_tasks; // tasks used during this day.
    void add_worker(Worker new_worker);
    void change_resolution(int);
    void remove_worker(const std::string &worker_name);
    std::vector<Worker> get_all_workers();
    int get_resolution();
    void add_work_day_reference_column(int task_number, int start_time, int end_time);
    void remove_work_day_reference_column(int id);
    std::vector<std::vector<int>> get_work_day_reference();
    void build_work_day(); // creates m_worker_task_list;
    void save_work_day();
};

#endif //EDAL_SCHEMA_CORE_H
