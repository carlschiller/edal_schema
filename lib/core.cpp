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
#include <sys/stat.h>

namespace Utilities{

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
            else{
                temporary_name += "n1.cfg";

            }
            return stream_name(temporary_name);
        }
        else{
            return file_name;
        }

    }
}

namespace Converters{

    Positions string_to_positions(const std::string &input){
        int index = 0;
        for(const std::string &position : Positions_name_table){
            if(input == position){
                return static_cast<Positions>(index);
            }
            index++;
        }
        throw std::runtime_error("Error converting string to positions");
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

    std::string positions_to_string(Positions input){
        return Positions_name_table[static_cast<int>(input)];
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

    std::string regex_find_and_replace(const std::string &line_of_text, const std::regex &reg, const std::string &replacer){
        return std::regex_replace(line_of_text,reg,replacer);
    }

    std::string regex_get_first_match(const std::string &line, const std::regex &reg){
        std::smatch matcher;
        std::regex_search(line,matcher,reg);
        return matcher[0];
    }

    // converts string to Genders class from tasks file.
    Genders string_to_sex(const std::string &input){
        if(std::regex_search(input,std::regex("<MALE>"))){
            return Genders::MALE;
        }
        else if(std::regex_search(input,std::regex("<FEMALE>"))){
            return Genders::FEMALE;
        }
        else if(std::regex_search(input,std::regex("<NONE>"))){
            return Genders::NONE;
        }
        else{
            throw std::invalid_argument("Input string cannot be interpreted as Genders class");
        }
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
}

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
 * .save_tasks_to_file(): method for saving Tasks class to separate txt file. O(n)
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
    if(value == m_task_map.end()){
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

// method below loads enum names from separate txt file into a private vector.
// how tasks.cfg is supposed to be formatted for this to work:
// [NAME_OF_TASK],flexibility:[bool],sex_req:[MALE/FEMALE/NULL]'\n'
void Tasks::load_tasks_from_file(){
    const std::string filename = "../lib/tasks.cfg";
    std::ifstream file(filename);
    if(!file.is_open()){
        std::exit(6); // crash program if file is not found.
    }
    else{
        std::string line; // a line representing a task from tasks.cfg.
        int index = 0; // index of the enum.
        char delim = ',';
        std::vector<std::string> tokens; // after splitting line in tasks.cfg by delimiter.
        while(std::getline(file, line)){
            if(line.length() != 0){
                tokens = Converters::split_by_delimiter(line,delim);
                m_task_map[tokens[0]] = index;
                m_task_flexibility[tokens[0]] = Converters::string_to_boolean(tokens[1]);
                m_task_sex_requirement[tokens[0]] = Converters::string_to_sex(tokens[2]);
                index++;
            }
        }
    }
    file.close();
}

void Tasks::save_tasks_to_file() {
    const std::string filename = "../lib/tasks.cfg";
    std::ofstream write_file(filename);
    if(!write_file.is_open()){
        std::exit(6); // TODO: fix a proper exception here.
    }
    else{
        // now add new tasks to file not in original file.
        auto task_size = static_cast<int>(m_task_map.size());
        for(int i = 0; i < task_size; ++i){
            std::string task_name = Tasks::get_task_name(i);
            std::string new_line = task_name + ',' + "flexibility:" +
                     Converters::boolean_to_string(m_task_flexibility[task_name])+ ',' + "sex_req:" +
                                   Converters::sex_to_string(m_task_sex_requirement[task_name]);
            write_file << new_line << std::endl;
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

Worker::Worker(std::string worker_name, Genders worker_sex, Positions position, long personal_number){
    m_worker_name = std::move(worker_name);
    m_worker_sex = worker_sex;
    m_position = position;
    m_personal_number = personal_number;
}

std::string Worker::get_name(){return m_worker_name;}

Genders Worker::get_gender(){return m_worker_sex;}

Positions Worker::get_position(){return m_position;}

long Worker::get_personal_number(){return m_personal_number;}

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


/**
 * Methods for Work_day class
 * -----------------
 *  constructor: creates a new work day with a time stamp.
 *  .load_workers_from_file(): loads workers from txt file.
 * .add_worker : adds a new Worker class to the vector of workers currently available. O(1).
 * .remove_worker: removes a worker, searched by name. O(n).
 * .find_worker: finds and returns a worker, searched by name. O(n).
 * .change_resolution: change resolution of work day. O(1)
 * .get_all_workers: returns worker list for day. O(1) ish.
 * .get_resolutions: returns resolution of day. O(1).
 * .add_work_day_reference_column: adds a column to the reference matrix,
 * a column of tasks to be distributed between some workers. O(n).
 * .build_work_day: TODO finish this function.
 * .save_work_day: saves changes made to members of this function.
 * -----------------
 */

// default constructor.
Work_day::Work_day(){
    m_work_day_date = time(nullptr);
    m_worker_list.clear();
    work_day_tasks = Tasks();
    load_workers_from_file();
}

// constructor for workday with timestamp and worker list.
Work_day::Work_day(time_t date_of_workday,int resolution){
    m_resolution = resolution;
    m_worker_list.clear();
    m_work_day_date = date_of_workday;
    work_day_tasks = Tasks(); // automatically loads from file when Work_day constructor is called.
    load_workers_from_file();
}

void Work_day::load_workers_from_file() {
    const std::string filename = "../lib/workers.cfg"; // make sure this filename is correct.
    std::ifstream file(filename);
    if(!file.is_open()){
        std::exit(6);
    }
    else{
        std::string line;
        std::vector<std::string> splits;
        while(std::getline(file,line)){
            if(line.length()!=0){
                splits = Converters::split_by_delimiter(line,',');

                // getting name of worker
                std::string worker_name = Converters::regex_get_first_match(splits[0],std::regex("<.*>"));
                worker_name = Converters::regex_find_and_replace(worker_name,std::regex("[<>]"),"");

                // getting sex of worker
                Genders worker_sex = Converters::string_to_sex(splits[1]);

                // getting position of worker, but must remove unneccessary clutter first from input.
                std::string worker_position_sanitized_input = Converters::regex_get_first_match(splits[2],std::regex("<.*>"));
                worker_position_sanitized_input = Converters::regex_find_and_replace(worker_position_sanitized_input,std::regex("[<>]"),"");
                Positions worker_position = Converters::string_to_positions(worker_position_sanitized_input);

                // getting personal number.
                std::string worker_personal_number = Converters::regex_get_first_match(splits[3],std::regex("<.*>"));
                worker_personal_number = Converters::regex_find_and_replace(worker_personal_number,std::regex("[<>]"),"");
                long worker_number = std::stol(worker_personal_number);

                // appending to worker list.
                m_worker_list.emplace_back(worker_name,worker_sex,worker_position,worker_number);
            }
        }
    }
}

void Work_day::save_workers_to_file() {
    const std::string filename = "../lib/workers.cfg";
    std::ofstream write_file(filename);
    if(!write_file.is_open()){
        std::exit(6); // TODO: fix a proper exception here.
    }
    else{
        // now add new tasks to file not in original file.
        for(Worker worker : m_worker_list){
            std::string worker_save_line;
            worker_save_line += "name:<" + worker.get_name() + ">,";
            worker_save_line += "sex:" + Converters::sex_to_string(worker.get_gender()) + ",";
            worker_save_line += "position:<" + Converters::positions_to_string(worker.get_position()) + ">,";
            worker_save_line += "personal_number:<" + std::to_string(worker.get_personal_number()) + ">";
            write_file << worker_save_line << std::endl;
        }
    }
    write_file.close();
}

void Work_day::add_worker(Worker new_worker) {
    for(Worker worker: m_worker_list){
        if(worker.get_name() == new_worker.get_name()){
            throw std::invalid_argument("Worker already exists");
        }
    }
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
    }
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
        std::vector<int> list_of_task_numbers = work_day_tasks.get_all_task_values(); // hope this works for speed.r
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
    if(!m_work_day_reference.empty()){
        if(id < 0 || id >= m_work_day_reference.size()){
            throw std::invalid_argument("invalid id number, can't remove reference column.");
        }
        m_work_day_reference.erase(m_work_day_reference.begin()+id);
    }
}

int Work_day::get_work_day_reference_size(){
    return static_cast<int>(m_work_day_reference.size());
}

std::vector<std::vector<int>> Work_day::get_work_day_reference() {
    return m_work_day_reference;
}

void Work_day::build_work_day() {
    // shit complex thing.
}

void Work_day::work_day_lexer(bool indent, bool file_name_override, std::string &custom_file_name) {
    const std::string path = "../lib/saves/"; // path to saves folder.
    // checks if we have a saves directory or not.
    // TODO: Create cross platform solution.
    const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err) {
        if (errno != EEXIST) {
            std::cout << "Error creating path in " + path << std::endl;
            exit(1);
        }
    }

    std::string filename;
    std::ofstream file_stream;
    // checks if we want to save file as name of date or custom name.
    if(file_name_override){
        // name to path
        filename = path + custom_file_name + ".cfg";
    }
    else{
        // converts current time to date of current time as string.
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d");
        std::string time_string = oss.str();
        filename = path + time_string + ".cfg";
        // checks if filename already exits, else it finds a new name.
        filename = Utilities::stream_name(filename);
    }
    // open and close file for debug.
    file_stream.open(filename,std::ofstream::out);
    file_stream.close();
}

// save changes made to work day.
void Work_day::save_work_day(){
    work_day_tasks.save_tasks_to_file(); // overwrites tasks if new have been added.
    std::string custom_name = "hej";
    work_day_lexer(false,false,custom_name);
}






