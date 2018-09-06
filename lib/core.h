//
// Created by Carl Schiller on 2018-06-07.
//

#ifndef EDAL_SCHEMA_CORE_H
#define EDAL_SCHEMA_CORE_H

#include <vector>
#include <ctime>
#include <chrono>
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

static std::string Positions_name_table[6]{
        "FULL_TIME",
        "HALF_TIME",
        "TEMPORARY_POSITION",
        "PARENT_LEAVE",
        "WEEKEND_WORKER",
        "EXTRA_WORKER"
};

namespace Utilities {
    bool file_exists(const std::string& file_name);
    std::string stream_name(std::string file_name);
}

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

// Instance of this class holds task name, sex requirement etc. Think of this as a container.
class Task{
private:
    std::string m_task_name;
    bool m_task_flexibility;
    Genders m_task_sex_requirement;
public:
    Task(); //default empty constructor
    Task(std::string &name, bool flex, Genders sex);

    std::string name(); // returns name.
    bool flexibility(); // returns flexibility;
    Genders sex_requirement(); //returns sex_requirement

    void change_name(std::string &name);
    void change_flexibility(bool &flex);
    void change_sex_requirement(Genders &sex);
};

// An instance of this class contains a list of all task names and attributes for given task. E.g. sex requirement.
class Tasks{
private:
    std::map<std::string, Task> m_task_map; // maps each Task name to a Task.
public:
    Tasks(); // constructor for Tasks.

    std::vector<std::string> get_all_task_names();
    std::map<std::string, Task> get_all_tasks();
    void add_task(std::string name, bool flexibility, Genders sex);
    void remove_task(std::string name);

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

class Workers{
private:
    std::map<std::string, Worker> m_worker_map; // maps each Worker name to a Worker.
public:
    Workers(); // constructor for Workers.

    std::vector<std::string> get_all_worker_names();
    std::map<std::string, Worker> get_all_workers();
    Worker get_worker(std::string &name);
    void add_worker(std::string &name, Genders sex, Positions position, long personal_number);
    void remove_worker(std::string &name);

    void load_workers_from_file();
    void save_workers_to_file();
};

// Work day class, contains all necessary information about the workday.
class Work_day{
private:
    std::map<std::string,Worker> m_worker_map;
    time_t m_date;

    int m_resolution; // how many chunks a day should be divided into.
    std::vector<std::vector<Task>> m_reference_matrix; // matrix of work day tasks to be done.
    std::vector<std::vector<Task>> m_worker_task_list; // matrix of tasks for workers
    std::string m_loaded_name; // name of file loaded.

    void work_day_lexer(bool indent, bool file_name_override, std::string &filename);
public:
    Work_day();
    Work_day(time_t, int);
    Tasks tasks_list; // tasks used during this day.

    // functions to add workers to day.
    void add_worker(Worker new_worker);
    void remove_worker(std::string &worker_name);
    std::map<std::string,Worker> get_all_workers();
    std::vector<std::string> get_all_worker_names();

    // functions to change resolution of day, how many chunks a day should be divided into.
    void change_resolution(int);
    int get_resolution();

    // functions to add reference columns.
    void add_reference_column(Task task_number, int start_time, int end_time);
    void remove_reference_column(int id);
    int reference_size();
    std::vector<std::vector<Task>> get_reference_matrix();

    // functions for building work day
    void build_work_day(); // creates m_worker_task_list

    // functions for loading and saving.
    void save_work_day();
};

#endif //EDAL_SCHEMA_CORE_H
