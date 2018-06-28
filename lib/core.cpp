//
// Created by Carl Schiller on 2018-06-07.
//

#include <string>
#include "core.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

/**
 * Methods for Tasks class
 * --------------
 * constructor: loads tasks from txt file.
 * .not_at_work(): returns default not at work task. O(1)
 * .get_task_values(string key): returns task number for given string key. O(log(n))
 * .get_task_name(int index): return string name of task for given int index. O(n)
 * .add_task(string new_key): appends a new task to the list at last position. O(log(n))
 * .get_all_task_names(): returns whole list of tasks. O(n)
 * .get_all_task_values(): returns whole list of tasks values. O(n)
 * .load_tasks_from_file(): method for loading Tasks class from separate txt file. O(n)
 * --------------
 */

Tasks::Tasks(){
    load_tasks_from_file();
};

int Tasks::not_at_work() {
    return 0;
}

int Tasks::get_task_value(const std::string &key) {
    std::map<std::string,int>::iterator value;
    value = m_task_map.find(key);
    if(value == m_task_map.end()){
        throw std::invalid_argument("Name of task not found in map.");
    }
    else{
        int result = value->second;
        return result;
    }
}

std::string Tasks::get_task_name(int index){
    for(auto& it : m_task_map){
        if(it.second == index){
            return it.first;
        }
    }
    throw std::invalid_argument("Value of task not found in map.");
}

// adds a new task in the list.
void Tasks::add_task(std::string new_key,bool flexibility, Genders sex) {
    std::map<std::string,int>::iterator value;
    value = m_task_map.find(new_key);
    if(value != m_task_map.end()){
        auto size = static_cast<int>(m_task_map.size());
        m_task_map[new_key] = size;
        m_task_flexibility[new_key] = flexibility;
        m_task_sex_requirement[new_key] = sex;
    }
    else{
        throw std::invalid_argument("Name of task already exists.");
    }
}

std::vector<std::string> Tasks::get_all_task_names() {
    std::vector<std::string> list_of_tasks;
    for(auto& it : m_task_map){
        list_of_tasks.push_back(it.first);
    }
    return list_of_tasks;
}

std::vector<int> Tasks::get_all_task_values() {
    std::vector<int> list_of_tasks;
    for(auto& it : m_task_map){
        list_of_tasks.push_back(it.second);
    }
    return list_of_tasks;
}

// stolen from fluent{C++}, splitting a line into components based on a delimiter.
std::vector<std::string> split_by_delimiter(const std::string &line, char delim){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(line); // converts string into a stream.
    while(std::getline(token_stream,token,delim)){
        tokens.push_back(token);
    }
    return tokens;
}

// converts string to boolean from tasks file.
bool string_to_boolean(const std::string &input){
    if(std::regex_search(input,std::regex("<true>"))){
        return true;
    }
    else if(std::regex_search(input,std::regex("<false>"))){
        return false;
    }
    else{
        throw std::invalid_argument("Input string cannot be interpreted as boolean");
    }
}

// converts string to Genders class from tasks file.
Genders string_to_sex(const std::string &input){
    if(std::regex_search(input,std::regex("<MALE>"))){
        return Genders::MALE;
    }
    else if(std::regex_search(input,std::regex("<FEMALE>"))){
        return Genders::FEMALE;
    }
    else if(std::regex_search(input,std::regex("<NULL>"))){
        return Genders::NONE;
    }
    else{
        throw std::invalid_argument("Input string cannot be interpreted as Genders class");
    }
}

std::string boolean_to_string(bool input){
    if(input){
        return "<true>";
    }
    else{
        return "<false>";
    }
}

std::string sex_to_string(Genders input){
    if(input == Genders::NONE){
        return "<NONE>";
    }
    else if(input == Genders::MALE){
        return "<MALE>";
    }
    else{
        return "<FEMALE>";
    }
}

std::string regex_find_and_replace(const std::string &line_of_text, const std::string &matcher, const std::string &replacer){
    return std::regex_replace(line_of_text,std::regex(matcher),replacer);
}

// method below loads enum names from separate txt file into a private vector.
// how tasks.txt is supposed to be formatted for this to work:
// [NAME_OF_TASK],flexibility:[bool],sex_req:[MALE/FEMALE/NULL]'\n'
void Tasks::load_tasks_from_file(){
    const std::string filename = "../lib/tasks.txt";
    std::ifstream file(filename);
    if(!file.is_open()){
        std::exit(6); // crash program if file is not found.
    }
    else{
        std::string line; // a line representing a task from tasks.txt.
        int index = 0; // index of the enum.
        char delim = ',';
        std::vector<std::string> tokens; // after splitting line in tasks.txt by delimiter.
        while(std::getline(file, line)){
            tokens = split_by_delimiter(line,delim);
            m_task_map[tokens[0]] = index;
            m_task_flexibility[tokens[0]] = string_to_boolean(tokens[1]);
            m_task_sex_requirement[tokens[0]] = string_to_sex(tokens[2]);
            index++;
        }
    }
    file.close();
}

void Tasks::save_tasks_from_file() {
    const std::string filename = "../lib/tasks.txt";
    std::ifstream read_file(filename);
    std::ofstream write_file(filename);
    if(!read_file.is_open() || !write_file.is_open()){
        std::exit(6); // TODO: fix a proper exception here.
    }
    else{
        std::string line; // a line representing a task from tasks.txt.
        int index = 0; // index of the enum.
        char delim = ',';
        std::vector<std::string> tokens; // after splitting line in tasks.txt by delimiter.
        while(std::getline(read_file,line)){
            tokens = split_by_delimiter(line,delim);
            if(m_task_flexibility[tokens[0]] != string_to_boolean(tokens[1])){
                // string_to_boolean(token[1]) converts original text file's boolean token into a boolean, then back into a string.
                // this is for getting the right regex match string.
                line = regex_find_and_replace(line,boolean_to_string(string_to_boolean(tokens[1])),boolean_to_string(m_task_flexibility[tokens[0]]));
            }
            if(m_task_sex_requirement[tokens[0]] != string_to_sex(tokens[2])){
                // same reasoning as above.
                line = regex_find_and_replace(line,sex_to_string(string_to_sex(tokens[2])),sex_to_string(m_task_sex_requirement[tokens[0]]));
            }
            write_file << line << std::endl;
        }
        read_file.close();
        write_file.close();
    }
}


/**
 * Methods for Worker class
 * -----------------
 * constructor : creates a new worker.
 * .get_name : returns name of worker.
 * .get_gender : gets gender of worker.
 * .get_position : gets position of worker.
 * .get_personal_number: gets personal number of worker.
 * .get_id : gets id of worker for the current day.
 * .change_name: change name of worker.
 * .change_gender: changes (!) gender of worker.
 * .change_position: change position of worker.
 * .change_personal_number : change personal number of worker.
 * .change_id : changes id of worker for the current day.
 * -----------------
 * All methods are O(1).
 */

Worker::Worker(std::string worker_name, Genders worker_sex, Positions position, long personal_number){
    m_worker_name = std::move(worker_name);
    m_worker_sex = worker_sex;
    m_position = position;
    m_personal_number = personal_number;
    m_date_id = 0;
}

std::string Worker::get_name(){return m_worker_name;}

Genders Worker::get_gender(){return m_worker_sex;}

Positions Worker::get_position(){return m_position;}

long Worker::get_personal_number(){return m_personal_number;}

int Worker::get_id() {return m_date_id;}

void Worker::change_name(std::string worker_name){
    m_worker_name = std::move(worker_name);
}

void Worker::change_gender(Genders new_sex){
    m_worker_sex = new_sex;
}

void Worker::change_position(Positions position){
    m_position = position;
}

void Worker::change_personal_number(long personal_number){
    m_personal_number = personal_number;
}

void Worker::change_id(int new_id) {
    m_date_id = new_id;
}


/**
 * Methods for Work_day class
 * -----------------
 *  constructor: creates a new work day with a time stamp.
 * .add_worker : adds a new Worker class to the vector of workers currently available. O(1).
 * .remove_worker: removes a worker, searched by name. O(n).
 * .find_worker: finds and returns a worker, searched by name. O(n).
 * .change_resolution: change resolution of work day. O(1)
 * .get_all_workers: returns worker list for day. O(1) ish.
 * .get_resolutions: returns resolution of day. O(1).
 * .add_work_day_reference_column: adds a column to the reference matrix,
 * a column of tasks to be distributed between some workers. O(n).
 *
 * -----------------
 */

// default constructor.
Work_day::Work_day(){
    m_work_day_date = time(nullptr);
    m_worker_list.clear();
    m_work_day_tasks = Tasks();
}

// constructor for workday with timestamp and worker list.
Work_day::Work_day(time_t date_of_workday,int resolution, std::vector<Worker> worker_list){
    m_resolution = resolution;
    m_worker_list = std::move(worker_list);
    m_work_day_date = date_of_workday;
    m_work_day_tasks = Tasks(); // automatically loads from file when Work_day constructor is called.
}

void Work_day::add_worker(Worker new_worker) {
    for(Worker worker: m_worker_list){
        if(worker.get_name() == new_worker.get_name()){
            throw std::invalid_argument("Worker already exists");
        }
    }

    // assigning an id to new worker.
    int id_assignment = (int)m_worker_list.size();
    new_worker.change_id(id_assignment);
    m_worker_list.push_back(new_worker);

}

void Work_day::remove_worker(const std::string &worker_name) {
    int position = 0;
    int position_of_found_worker = 0;
    bool worker_found = false;
    for(Worker worker : m_worker_list){
        if(worker.get_name() == worker_name){
            worker_found = true;
            position_of_found_worker = position;
        }
        position++;
    }
    if(!worker_found){
        throw std::invalid_argument("Name not found in list of workers for the day");
    }
    else{
        // removing worker from task list based on position and id.
        m_worker_list.erase(m_worker_list.begin()+position_of_found_worker);
        for (int i = position_of_found_worker; i < m_worker_list.size(); ++i) {
            m_worker_list[i].change_id(m_worker_list[i].get_id()-1);
        }

    }
}

int Work_day::find_worker_id(const std::string &worker_name) {
    for(Worker worker : m_worker_list){
        if(worker.get_name() == worker_name){
            return worker.get_id();
        }
    }
    throw std::invalid_argument("Name not found in list of workers for the day");
}

void Work_day::change_resolution(int new_resolution) {
    m_resolution = new_resolution;
}

std::vector<Worker> Work_day::get_all_workers() {
    return m_worker_list;
}

int Work_day::get_resolution() {
    return m_resolution;
}

void Work_day::add_work_day_reference_column(int task_number, int start_time, int end_time) {
    // first check if start or end time has allowed values.
    if(start_time < end_time && start_time < m_resolution && end_time < m_resolution){
        // check if task number is in list of tasks.
        bool found_task_flag = false;
        std::vector<int> list_of_task_numbers = m_work_day_tasks.get_all_task_values(); // hope this works for speed.r
        for(int task : list_of_task_numbers){
            if(task == task_number){
                found_task_flag = true;
            }
        }
        if(found_task_flag){
            // add task to reference matrix.
            std::vector<int> reference_column;
            reference_column.reserve((unsigned long)m_resolution);
            for (int i = 0; i < start_time; ++i) {
                // assigning no work before start time.
                reference_column.push_back(Tasks::not_at_work());
            }
            for (int j = start_time; j < end_time; ++j){
                // assigning task number
                reference_column.push_back(task_number);
            }
            for (int k = end_time; k < m_resolution; ++k) {
                reference_column.push_back(Tasks::not_at_work());
            }
            // appending column to reference matrix.
            m_work_day_reference.push_back(reference_column);
        }
        else{
            throw std::invalid_argument("invalid task id");
        }
    }
    else{
        throw std::invalid_argument("invalid start and/or end time");
    }
}

void Work_day::remove_work_day_reference_column(int id) {
    m_work_day_reference.erase(m_work_day_reference.begin()+id);
}

std::vector<std::vector<int>> Work_day::get_work_day_reference() {
    return m_work_day_reference;
}

void Work_day::build_work_day() {
    // shit complex thing.
}

Tasks Work_day::get_tasks() {
    return m_work_day_tasks;
}




