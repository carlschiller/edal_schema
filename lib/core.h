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


namespace Utilities {
    void create_directory(const std::string &path);
    bool file_exists(const std::string& file_name);
    std::string stream_name(std::string file_name);
}

// Functions for saving and loading.
namespace Converters {
    bool string_to_boolean(const std::string &input);
    std::string boolean_to_string(bool input);
    std::string regex_find_and_replace(const std::string &line_of_text,
                                              const std::regex &reg, const std::string &replacer);
    std::string regex_get_first_match(const std::string &line, const std::regex &reg);
    std::vector<std::string> split_by_delimiter(const std::string &line, char delim);
}

// custom "enum class", loads genders from file.
class Genders{
public:
    Genders();
    std::map<std::string,int> m_gender_map;

    int get_gender(std::string &sex);
    std::string get_string(int number);
    void load_genders();
    void save_genders();
};

// custom "positions class", loads positions from file.
class Positions{
private:
    std::map<std::string,int> m_positions_map;
    void load_positions();
public:
    Positions();
    int get_position(std::string &position);
    void save_positions();
};

// Instance of this class holds task name, sex requirement etc. Think of this as a container.
class Task{
private:
    std::string m_task_name;
    bool m_task_flexibility;
    int m_task_sex_requirement;
public:
    Task(); //default empty constructor
    Task(std::string &name, bool flex, int sex);

    std::string name(); // returns name.
    bool flexibility(); // returns flexibility;
    int sex_requirement(); //returns sex_requirement

    void change_name(std::string &name);
    void change_flexibility(bool &flex);
    void change_sex_requirement(int &sex);
};

// An instance of this class contains a list of all task names and attributes for given task. E.g. sex requirement.
class Tasks{
private:
    std::map<std::string,Task> m_task_map; // maps each Task name to a Task.
    void load_tasks_from_file();
public:
    Tasks(); // constructor for Tasks.

    std::vector<std::string> get_all_task_names();
    std::map<std::string, Task> get_all_tasks();
    void add_task(std::string name, bool flexibility, int sex);
    void remove_task(std::string name);

    void save_tasks_to_file();
};


// Worker class, each object type of Worker will contain all neccessary information about a person.
class Worker{
private:
    std::string m_worker_name;
    int m_worker_sex;
    int m_position;
    long m_personal_number;
public:
    Worker();
    Worker(std::string, int, int, long);

    std::string get_name();
    int get_gender();
    int get_position();
    long get_personal_number();

    void change_name(std::string);
    void change_gender(int);
    void change_position(int);
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
    void add_worker(std::string &name, int sex, int position, long personal_number);
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
    void clear_reference_matrix();

    // functions for building work day
    void build_work_day(); // creates m_worker_task_list

    // functions for loading and saving.
    void save_work_day();
};

#endif //EDAL_SCHEMA_CORE_H
