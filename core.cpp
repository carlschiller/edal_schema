//
// Created by Carl Schiller on 2018-06-07.
//

#include <string>
#include "core.h"

// Worker class, each object type of Worker will contain all neccessary information about a person.
class Worker{
private:
    std::string m_worker_name;
    Genders m_worker_sex;
    Positions m_position;
    long m_personal_number;
public:
    Worker(std::string, Genders, Positions, long);

    std::string get_string(){return m_worker_name;}
    Genders get_gender(){return m_worker_sex;}
    Positions get_position(){return m_position;}
    long get_personal_number(){return m_personal_number;}

    void change_name(std::string worker_name){
        m_worker_name = std::move(worker_name);
    }
    void change_gender(Genders new_sex){
        m_worker_sex = new_sex;
    }
    void change_position(Positions position){
        m_position = position;
    }
    void change_personal_number(long personal_number){
        m_personal_number = personal_number;
    }
};

Worker::Worker(std::string worker_name, Genders worker_sex, Positions position, long personal_number){
    m_worker_name = std::move(worker_name);
    m_worker_sex = worker_sex;
    m_position = position;
    m_personal_number = personal_number;
}



