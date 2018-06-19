//
// Created by Carl Schiller on 2018-06-07.
//

#include <string>
#include "core.h"
#include <fstream>
#include <sstream>

/**
 * Methods for Tasks class
 * --------------
 * constructor: loads tasks from txt file.
 * .not_at_work(): returns default not at work task.
 * .get_task_values(string key): returns task number for given string key.
 * .get_task_name(int index): return string name of task for given int index.
 * .add_task(string new_key): appends a new task to the list at last position.
 * .get_all_task_names(): returns whole list of tasks.
 * .load_tasks_from_file(): method for loading Tasks class from separate txt file.
 * --------------
 * All methods are O(1) except get_task_value, add_task, load_tasks_from_file which are O(n).
 */

Tasks::Tasks(){
    load_tasks_from_file();
};

static int Tasks::not_at_work() {
    return 0;
}

int Tasks::get_task_value(const std::string &key) {
    int counter = 0;
    for(const std::string &task_name : m_task_list){
        if(task_name == key){
            return counter;
        }
        counter++;
    }
    throw std::invalid_argument("Name of task not found.");
}

std::string Tasks::get_task_name(int index){
    if(index > m_task_list.size()-1 || index < 0){
        throw std::invalid_argument("Index out of range");
    }
    else{
        return m_task_list[index];
    }
}

// adds a new task in the list.
void Tasks::add_task(std::string new_key) {
    for(const std::string &task : m_task_list){
        if(new_key == task){
            throw std::invalid_argument("Task already exists.");
        }
    }
    m_task_list.push_back(new_key);
}

std::vector<std::string> Tasks::get_all_task_names() {
    return m_task_list;
}

// method below loads enum names from separate txt file into a private vector.
void Tasks::load_tasks_from_file(){
    const std::string filename = "tasks.txt";
    std::ifstream file;
    file.exceptions(std::ifstream::failbit);
    try{
        file.open(filename);
        if(file.is_open()){
            std::string key;
            while(file >> key){
                m_task_list.push_back(key);
                key = "";
            }
        }
    }
    catch(const std::exception& e){
        std::ostringstream msg;
        msg << "Opening file " << filename << " failed, probably doesn't exist.";
        throw std::runtime_error(msg.str());
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
 * -----------------
 */

// default constructor.
Work_day::Work_day(){
    m_work_day_date = time(nullptr);
    m_worker_list.clear();
}

// constructor for workday with timestamp and worker list.
Work_day::Work_day(time_t date_of_workday,int resolution, std::vector<Worker> worker_list){
    m_resolution = resolution;
    m_worker_list = std::move(worker_list);
    m_work_day_date = date_of_workday;

    int id_assignment = 0;
    for(Worker worker : m_worker_list){
        worker.change_id(id_assignment);
        id_assignment++;
    }
    // resizing list of tasks for workers to match length of worker list, each worker receives a list of size resolution with empty tasks.
    std::vector<Tasks> init_tasks((unsigned long)m_resolution,Tasks::not_at_work());
    m_work_day_tasks.resize(m_worker_list.size(),init_tasks);
}

void Work_day::add_worker(Worker new_worker) {
    for(Worker worker: m_worker_list){
        if(worker.get_name() == new_worker.get_name()){
            throw std::invalid_argument("Worker already exists");
        }
    }

    // assigning an id to new worker, adding worker to day task list with NOT_AVAILABLE as standard.
    int id_assignment = (int)m_worker_list.size();
    new_worker.change_id(id_assignment);
    m_worker_list.push_back(new_worker);
    std::vector<Tasks> init_tasks((unsigned long)m_resolution,Tasks::NOT_AVAILABLE);
    m_work_day_tasks.push_back(init_tasks);

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
        m_work_day_tasks.erase(m_work_day_tasks.begin()+m_worker_list[position_of_found_worker].get_id());
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

std::vector<Tasks> Work_day::view_worker_tasks(const std::string &worker_name) {
    return m_work_day_tasks[find_worker_id(worker_name)];
}

std::vector<std::vector<Tasks>> Work_day::view_all_worker_tasks() {
    return m_work_day_tasks;
}

std::vector<Worker> Work_day::get_all_workers() {
    return m_worker_list;
}

int Work_day::get_resolution() {
    return m_resolution;
}
