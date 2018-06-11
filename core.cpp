//
// Created by Carl Schiller on 2018-06-07.
//

#include <string>
#include "core.h"

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
 * .add_worker : adds a new Worker class to the vector of workers currently available. O(1).
 * .remove_worker: removes a worker, searched by name. O(n).
 * .find_worker: finds and returns a worker, searched by name. O(n).
 * -----------------
 */

// default constructor.
Work_day::Work_day(){
    m_work_day_date = time(nullptr);
    m_worker_list = nullptr;
}

// constructor for workday with timestamp and worker list.
Work_day::Work_day(time_t date_of_workday, std::vector<Worker> worker_list){
    m_worker_list = std::move(worker_list);
    m_work_day_date = date_of_workday;
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
    bool worker_found = false;
    for(Worker worker : m_worker_list){
        if(worker.get_name() == worker_name){
            m_worker_list.erase(m_worker_list.begin()+position);
            worker_found = true;
        }
        position++;
    }
    if(!worker_found){
        throw std::invalid_argument("Name not found in list of workers for the day");
    }
}

Worker Work_day::find_worker(const std::string &worker_name) {
    for(Worker worker : m_worker_list){
        if(worker.get_name() == worker_name){
            return worker;
        }
    }
    throw std::invalid_argument("Name not found in list of workers for the day");
}