//
// Created by Carl Schiller on 2018-06-07.
//

#include <string>
#include "core.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <iomanip>
#include <ctime>
#include <utility>
#include <sys/stat.h>


namespace Utilities{

    // TODO: create cross platform
    void create_directory(const std::string &path){
        const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (-1 == dir_err) {
            if (errno != EEXIST) {
                std::cout << "Error creating path in " + path << std::endl;
                exit(1);
            }
        }
    }

    // stolen from https://stackoverflow.com/questions/4316442/stdofstream-check-if-file-exists-before-writing
    // checks if file exists before writing.
    bool file_exists(const std::string & file_name){
        struct stat buf{};
        return stat(file_name.c_str(), &buf) != -1;
    }

    // recursively finds appropriate file name;
    std::string stream_name(std::string file_name){
        if(file_exists(file_name)){
            // split by delimiter -
            std::string temporary_name;
            std::vector<std::string> tokens = Converters::split_by_delimiter(file_name,'.');
            unsigned int token_place = static_cast<unsigned int>(tokens.size())-2;
            temporary_name += ".." + tokens[token_place];
            std::vector<std::string> number_of_saves_vector = Converters::split_by_delimiter(temporary_name,'n');
            if(number_of_saves_vector.size() > 1){
                std::string number_of_saves = std::to_string(std::stoi(number_of_saves_vector[1])+1);
                temporary_name = number_of_saves_vector[0] + "n" + number_of_saves + ".cfg";
            }
            else{ temporary_name += "n1.cfg"; }
            return stream_name(temporary_name);
        }
        else{ return file_name; }

    }

    void matrix_saver(std::ofstream stream){

    }

    void lexer_save_stream(const std::string & filename,std::ios_base::open_mode mode, Work_day &current_day, bool indent){
        // open stream.
        std::ofstream file_stream(filename, mode);
        int indent_counter = 0; //level of indentation


        // save date in file.
        std::string file_date = Converters::regex_get_first_match(filename,std::regex(R"(\d+-\d+-\d+)"));
        file_stream << std::string(indent_counter, '\t') << "date:<" << file_date << ">\n";
        // save workers who work that day.
        file_stream << std::string(indent_counter, '\t') << "worker_list:{\n";
        if(indent){indent_counter++;}
        std::map<std::string,Worker> worker_map = current_day.get_all_workers();
        for(auto it : worker_map){
            std::string name = it.first;
            std::string gender = std::to_string(it.second.get_gender());
            std::string position = std::to_string(it.second.get_position());
            std::string personal_number = std::to_string(it.second.get_personal_number());
            file_stream << std::string(indent_counter, '\t') << "name:<" << name << ">,sex:<" << gender
                        << ">,position:<" << position << ">,personal_number:<" << personal_number << ">\n";
        }
        if(indent){indent_counter--;}
        file_stream << std::string(indent_counter, '\t') << "}\n";
        // save reference matrix to file.

        file_stream.close();
    }
}

namespace Converters{

    std::string boolean_to_string(bool input){
        if(input){ return "<true>"; }
        else{ return "<false>"; }
    }

    // converts string to boolean from tasks file.
    bool string_to_boolean(const std::string &input){
        if(std::regex_search(input,std::regex("<true>"))){ return true; }
        else if(std::regex_search(input,std::regex("<false>"))){ return false; }
        else{ throw std::invalid_argument("Input string cannot be interpreted as boolean"); }
    }

    int remove_container(std::string & input) {
        std::string sanitized_input = regex_get_first_match(input,std::regex("<\\d+>"));
        sanitized_input = regex_find_and_replace(sanitized_input,std::regex("[<>]"),"");
        return std::stoi(sanitized_input);
    }

    // stolen from fluent{C++}, splitting a line into components based on a delimiter.
    std::vector<std::string> split_by_delimiter(const std::string &line, char delim){
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream token_stream(line); // converts string into a stream.
        while(std::getline(token_stream,token,delim)){ tokens.push_back(token); }
        return tokens;
    }

    std::string regex_find_and_replace(const std::string &line_of_text, const std::regex &reg, const std::string &replacer){
        return std::regex_replace(line_of_text,reg,replacer);
    }

    std::string regex_get_first_match(const std::string &line, const std::regex &reg){
        std::smatch matcher;
        std::regex_search(line,matcher,reg);
        return matcher[0];
    }
}

/**
 * Methods for Genders class
 * -------------
 * constructor: loads genders from file
 * .get_gender(): returns int from text.
 * .load_genders(): loads from file.
 * .save_genders(): saves from file.
 */

Genders::Genders() { load_genders(); }

int Genders::get_gender(std::string &sex) {
    auto it = m_gender_map.find(sex);
    if(it == m_gender_map.end()){
        std::string error_msg = "Can't find " + sex + " gender.";
        throw std::invalid_argument(error_msg);
    }
    else{ return m_gender_map[sex];}
}

std::string Genders::get_string(int number) {
    for(auto it : m_gender_map){
        if(it.second == number){ return it.first; }
    }
    throw std::invalid_argument("Not found in map.");
}

void Genders::load_genders() {
    const std::string path = "../lib/config/";
    Utilities::create_directory(path); // check if we have the directory;

    const std::string filename = "genders.cfg";
    std::string file_path = path + filename;
    std::ifstream stream(file_path);
    if(!stream.is_open()){
        std::exit(6);
        // TODO: fix error execution;
    }
    else{
        std::string line;
        int index = 0;
        while(std::getline(stream,line)){
            std::vector<std::string> tokens;
            if(line.length() != 0){
                tokens = Converters::split_by_delimiter(line,',');
                m_gender_map[tokens[0]] = std::stoi(tokens[1]);
            }
            index++;
        }
    }
    stream.close();
}

void Genders::save_genders() {
    const std::string path = "../lib/config/";
    Utilities::create_directory(path); // check if we have the directory;

    const std::string filename = "genders.cfg";
    std::string file_path = path + filename;
    std::ofstream stream(file_path,std::ios_base::out);
    if(!stream.is_open()){
        std::exit(6);
        // TODO: fix error execution;
    }
    else{
        for(auto & it : m_gender_map){
            std::string name = it.first;
            std::string number = std::to_string(it.second);
            std::string output = name += "," + number;
            stream <<  output << std::endl;
        }
    }
}

/**
 * Methods for Positions class
 * -------------
 * constructor: loads genders from file
 * .get_position(): returns int from text.
 * .load_positions(): loads from file.
 * .save_positions(): saves from file.
 */

Positions::Positions() { load_positions(); }

int Positions::get_position(std::string &position) {
    auto it = m_positions_map.find(position);
    if(it == m_positions_map.end()){
        std::string error_msg = "Can't find " + position + " position.";
        throw std::invalid_argument(error_msg);
    }
    else{ return m_positions_map[position]; }
}

void Positions::load_positions() {
    const std::string path = "../lib/config/";
    Utilities::create_directory(path); // check if we have the directory;

    const std::string filename = "positions.cfg";
    std::string file_path = path + filename;
    std::ifstream stream(file_path);
    if(!stream.is_open()){
        std::exit(6);
        // TODO: fix error execution;
    }
    else{
        std::string line;
        int index = 0;
        while(std::getline(stream,line)){
            std::vector<std::string> tokens;
            if(line.length() != 0){
                tokens = Converters::split_by_delimiter(line,',');
                m_positions_map[tokens[0]] = std::stoi(tokens[1]);
            }
            index++;
        }
    }
    stream.close();
}

void Positions::save_positions() {
    const std::string path = "../lib/config/";
    Utilities::create_directory(path); // check if we have the directory;

    const std::string filename = "positions.cfg";
    std::string file_path = path + filename;
    std::ofstream stream(file_path,std::ios_base::out);
    if(!stream.is_open()){
        std::exit(6);
        // TODO: fix error execution;
    }
    else{
        for(auto & it : m_positions_map){
            std::string name = it.first;
            std::string number = std::to_string(it.second);
            std::string output = name += "," + number;
            stream <<  output << std::endl;
        }
    }
}


/**
 * Methods for Task class
 * --------------
 * constructor: creates container with a all information for this task.
 * .name(): returns name of task.
 * .flexibility(): returns flexibility of this task.
 * .sex_requirement(): returns sex_requirement of this task.
 * .change_[insert parameter here](): changes [parameter] of this task.
 * --------------
 * O(1) in complexity.
 */

Task::Task() {
    m_task_name = "NOT_AT_WORK";
    m_task_flexibility = false;
    m_task_sex_requirement = 0; // 0 = NONE
}

Task::Task(std::string &name, bool flex, int sex_req) {
    m_task_name = std::move(name);
    m_task_flexibility = flex;
    m_task_sex_requirement = sex_req;
}

std::string Task::name() { return m_task_name; }

bool Task::flexibility() { return m_task_flexibility; }

int Task::sex_requirement() { return m_task_sex_requirement; }

void Task::change_name(std::string &name) { m_task_name = std::move(name); }

void Task::change_flexibility(bool &flex) { m_task_flexibility = flex; }

void Task::change_sex_requirement(int &sex) { m_task_sex_requirement = sex; }

/**
 * Methods for Tasks class
 * --------------
 * constructor: loads tasks from txt file.
 * .not_at_work(): returns default not at work task. O(1)
 * .add_task(string new_key): appends a new task to the list at last position. O(log(n))
 * .get_all_task_names(): returns whole list of tasks. O(n)
 * .get_all_tasks(): returns all tasks as a map. O(1)
 * .load_tasks_from_file(): method for loading Tasks class from separate txt file. O(n)
 * .save_tasks_to_file(): method for saving Tasks class to separate txt file. O(n)
 * --------------
 */

Tasks::Tasks(){ load_tasks_from_file(); };

// adds a new task in the list.
void Tasks::add_task(std::string new_name,bool flexibility, int sex) {
    std::map<std::string,Task>::iterator value;
    value = m_task_map.find(new_name);
    if(value == m_task_map.end()){
        std::string key = new_name;
        m_task_map[key] = Task(new_name,flexibility,sex);
    }
    else{ throw std::invalid_argument("Name of task already exists."); }
}

// removes a task from the list by name.
void Tasks::remove_task(std::string name) {
    std::map<std::string,Task>::iterator value;
    value = m_task_map.find(name);
    if(value != m_task_map.end()){ m_task_map.erase(value); } // removes by iterator.
}

// returns a list of all names of tasks from map.
std::vector<std::string> Tasks::get_all_task_names() {
    std::vector<std::string> list_of_tasks;
    for(auto it : m_task_map){ list_of_tasks.push_back(it.first); }
    return list_of_tasks;
}

// returns map of all Tasks.
std::map<std::string,Task> Tasks::get_all_tasks() { return m_task_map; }

// method below loads enum names from separate txt file into a private map.
// how tasks.cfg is supposed to be formatted for this to work:
// [NAME_OF_TASK],flexibility:[bool],sex_req:[MALE/FEMALE/NULL]'\n'
void Tasks::load_tasks_from_file(){
    const std::string path = "../lib/config/";
    Utilities::create_directory(path); // check if we have the directory;

    const std::string filename = "tasks.cfg";
    std::string file_path = path + filename;
    std::ifstream file(file_path);
    if(!file.is_open()){
        std::exit(6); // crash program if file is not found.
        // TODO: fix this error if crashes.
    }
    else{
        std::string line; // a line representing a task from tasks.cfg.
        char delim = ',';
        std::vector<std::string> tokens; // after splitting line in tasks.cfg by delimiter.
        while(std::getline(file, line)){
            if(line.length() != 0){
                tokens = Converters::split_by_delimiter(line,delim);
                std::string temp_name = tokens[0];
                bool temp_flex = Converters::string_to_boolean(tokens[1]);
                int temp_sex = Converters::remove_container(tokens[2]);
                add_task(temp_name,temp_flex,temp_sex);
            }
        }
    }
    file.close();
}

void Tasks::save_tasks_to_file() {
    const std::string path = "../lib/config/";
    Utilities::create_directory(path); // check if we have the directory;

    const std::string filename = "tasks.cfg";
    std::string file_path = path + filename;
    std::ofstream write_file(file_path);
    if(!write_file.is_open()){ std::exit(6); } // TODO: fix a proper exception here.
    else{
        // now add new tasks to file not in original file.
        for(auto it : m_task_map){
            std::string temp_name = it.first;
            std::string temp_sex_string = "<" + std::to_string(it.second.sex_requirement()) + ">";
            std::string temp_flex_string = "<" + std::to_string(it.second.flexibility()) + ">";
            std::string output_line = temp_name += ",flexibility:" + temp_sex_string += ",sex_req:" + temp_flex_string;
            write_file << output_line;
        }
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
 * .change_name: change name of worker.
 * .change_gender: changes (!) gender of worker.
 * .change_position: change position of worker.
 * .change_personal_number : change personal number of worker.
 * -----------------
 * All methods are O(1).
 */

Worker::Worker() = default;

Worker::Worker(std::string worker_name, int worker_sex, int position, long personal_number){
    m_worker_name = std::move(worker_name);
    m_worker_sex = worker_sex;
    m_position = position;
    m_personal_number = personal_number;
}

std::string Worker::get_name(){ return m_worker_name; }

int Worker::get_gender(){ return m_worker_sex; }

int Worker::get_position(){ return m_position; }

long Worker::get_personal_number(){ return m_personal_number; }

void Worker::change_name(std::string worker_name){ m_worker_name = std::move(worker_name); }

void Worker::change_gender(int new_sex){ m_worker_sex = new_sex; }

void Worker::change_position(int position){ m_position = position; }

void Worker::change_personal_number(long personal_number){ m_personal_number = personal_number; }

/**
 * Methods for Workers class
 * -----------------
 * constructor : loads workers from file.
 * .get_name : returns name of worker.
 * .get_gender : gets gender of worker.
 * .get_position : gets position of worker.
 * .get_personal_number: gets personal number of worker.
 * .change_name: change name of worker.
 * .change_gender: changes (!) gender of worker.
 * .change_position: change position of worker.
 * .change_personal_number : change personal number of worker.
 * -----------------
 * All methods are O(1).
 */

Workers::Workers() { load_workers_from_file(); }

std::vector<std::string> Workers::get_all_worker_names() {
    std::vector<std::string> worker_list;
    for(auto it : m_worker_map){ worker_list.push_back(it.first); }
    return worker_list;
}

std::map<std::string, Worker> Workers::get_all_workers() { return m_worker_map; }

Worker Workers::get_worker(std::string &name) {
    auto it = m_worker_map.find(name);
    if(it != m_worker_map.end()) { return m_worker_map[name]; }
    else { throw std::invalid_argument("Worker name doesn't exist."); }
}

void Workers::add_worker(std::string &name, int sex, int position, long personal_number) {
    auto it = m_worker_map.find(name);
    if(it == m_worker_map.end()){ m_worker_map[name] = Worker(name,sex,position,personal_number); }
    else{ throw std::invalid_argument("Worker name already exists"); }
}

void Workers::remove_worker(std::string &name) {
    auto it = m_worker_map.find(name);
    if(it != m_worker_map.end()){ m_worker_map.erase(name); }
    else{ throw std::invalid_argument("Worker name not found to remove"); }
}

void Workers::load_workers_from_file() {
    const std::string path = "../lib/config/";
    Utilities::create_directory(path); // check if we have the directory;

    const std::string filename = "workers.cfg"; // make sure this filename is correct.
    std::string file_path = path + filename;
    std::ifstream file(file_path);
    if(!file.is_open()){ std::exit(6); }
    else {
        std::string line;
        std::vector<std::string> tokens;
        while (std::getline(file, line)) {
            if (line.length() != 0) {
                tokens = Converters::split_by_delimiter(line, ',');

                // getting name of worker
                std::string worker_name = Converters::regex_get_first_match(tokens[0], std::regex("<.*>"));
                worker_name = Converters::regex_find_and_replace(worker_name, std::regex("[<>]"), "");

                // getting sex of worker
                int worker_sex = Converters::remove_container(tokens[1]);

                // getting position of worker, but must remove unnecessary clutter first from input.
                int worker_position = Converters::remove_container(tokens[2]);

                // getting personal number.
                std::string worker_personal_number = Converters::regex_get_first_match(tokens[3], std::regex("<.*>"));
                worker_personal_number = Converters::regex_find_and_replace(worker_personal_number, std::regex("[<>]"), "");
                long worker_number = std::stol(worker_personal_number);

                // add to map.
                add_worker(worker_name,worker_sex,worker_position,worker_number);
            }
        }
    }
}

void Workers::save_workers_to_file() {
    const std::string path = "../lib/config/";
    Utilities::create_directory(path); // check if we have the directory;

    std::string filename = "workers.cfg";
    std::string file_path = path + filename; // path to file.

    std::ofstream write_file(file_path);
    if(!write_file.is_open()){ std::exit(6); } // TODO: fix a proper exception here.
    else{
        for(auto &it : m_worker_map){
            std::string worker_save_line;
            std::string worker_name = it.first;
            std::string worker_sex = std::to_string(it.second.get_gender());
            std::string worker_position = std::to_string(it.second.get_position());
            std::string worker_personal_number = std::to_string(it.second.get_personal_number());

            worker_save_line += "name:<" + worker_name + ">,";
            worker_save_line += "sex:" + worker_sex + ",";
            worker_save_line += "position:<" + worker_position + ">,";
            worker_save_line += "personal_number:<" + worker_personal_number + ">";

            write_file << worker_save_line;
        }
    }
    write_file.close();
}


/**
 * Methods for Work_day class
 * -----------------
 *  constructor: creates a new work day with a time stamp.
 *
 *  Worker class related function:
 *  =====================
 *   .add_worker : adds a new Worker for the day to the worker_map. O(log(n))
 *   .remove_worker: removes a worker, searched by name. O(log(n)).
 *   .get_all_workers: returns worker map for day. O(1)
 *   .get_all_worker_names: returns vector of all names for the day. O(log(n))
 *
 *  Resolution related functions:
 *  =====================
 *   .change_resolution: change resolution of work day. O(1)
 *   .get_resolution: returns resolution of day. O(1).
 *
 *  Work day reference matrix related functions:
 *  =====================
 *   .add_reference_column: adds a column to the reference matrix,
 *                                   a column of tasks to be distributed between some workers. O(n).
 *   .remove_reference_column: removes a reference column by id. O(n)
 *   .reference_matrix_size: returns column size of work_day matrix. O(fast)
 *   .get_reference_matrix: returns reference_matrix. O(1)
 *
 *  Work day schedule matrix related functions:
 *  =====================
 *   .build_work_day: TODO finish this function.
 *
 *  Utility functions:
 *  =====================
 *   .work_day_lexer: converts class into a .cfg file.
 *   .save_work_day: saves changes made to members of this function.
 * -----------------
 */

// default constructor.
Work_day::Work_day(){
    m_date = time(nullptr);
    m_date = std::time(nullptr);
}

// constructor for workday with timestamp and worker list.
Work_day::Work_day(time_t date_of_workday,int resolution){
    m_resolution = resolution;
    m_date = date_of_workday;
}

// add worker to work_day map.
void Work_day::add_worker(Worker new_worker) {
    std::string worker_name = new_worker.get_name();
    auto it = m_worker_map.find(worker_name);
    if(it == m_worker_map.end()){ m_worker_map[worker_name] = new_worker; }
    else{
        std::string error_msg = "Worker " + worker_name + "already exits for this day.";
        throw std::invalid_argument(error_msg);
    }
}

void Work_day::remove_worker(std::string &name) {
    auto it = m_worker_map.find(name);
    if(it != m_worker_map.end()){ m_worker_map.erase(name); }
    else{
        std::string error_msg = name + " is not in the list of workers for the day.";
        throw std::invalid_argument(error_msg);
    }
}

std::map<std::string,Worker> Work_day::get_all_workers() { return m_worker_map; }

std::vector<std::string> Work_day::get_all_worker_names() {
    std::vector<std::string> worker_list;
    for(auto it : m_worker_map){ worker_list.push_back(it.first); }
    return worker_list;
}

void Work_day::change_resolution(int new_resolution) { m_resolution = new_resolution; }

int Work_day::get_resolution() { return m_resolution; }

void Work_day::add_reference_column(Task task, int start_time, int end_time) {
    // first check if start or end time has allowed values.
    if(start_time < end_time && start_time < m_resolution && end_time < m_resolution){
        std::vector<Task> reference_column;
        Task empty_task = Task();
        for(int i = 0; i < m_resolution; ++i){
            if(i >= start_time && i < end_time){ reference_column.push_back(task); }
            else { reference_column.push_back(empty_task); }
        }
        m_reference_matrix.push_back(reference_column);
    }
    else{ throw std::invalid_argument("invalid start and/or end time"); }
}

void Work_day::remove_reference_column(int id) {
    if(!m_reference_matrix.empty()){
        if(id < 0 || id >= m_reference_matrix.size()){
            throw std::invalid_argument("invalid id number, can't remove reference column.");
        }
        m_reference_matrix.erase(m_reference_matrix.begin()+id);
    }
}

int Work_day::reference_size(){ return static_cast<int>(m_reference_matrix.size()); }

std::vector<std::vector<Task>> Work_day::get_reference_matrix() { return m_reference_matrix; }

void Work_day::build_work_day() {
    // shit complex thing.
}

void Work_day::work_day_lexer(bool indent, bool file_name_override, std::string &custom_file_name) {
    const std::string path = "../lib/saves/"; // path to saves folder.
    // checks if we have a saves directory or not.
    Utilities::create_directory(path);

    std::string filename;
    // checks if we want to save file as name of date or custom name.
    if(file_name_override){ filename = path + custom_file_name + ".cfg"; } // name to path
    else{
        // converts current time to date of current time as string.
        auto tm = *std::localtime(&m_date);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d");
        std::string time_string = oss.str();
        filename = path + time_string + ".cfg";
        // checks if filename already exits, else it finds a new name.
        filename = Utilities::stream_name(filename);
    }
    // open and close file for debug.
    Utilities::lexer_save_stream(filename,std::ios_base::out,*this,true);
}

// save changes made to work day.
void Work_day::save_work_day(){
    std::string custom_name = "hej";
    work_day_lexer(false,false,custom_name);
}






