//
// Created by Carl Schiller on 2018-06-11.
//

#include <iostream>
#include "menu.h"
#include "core.h"

enum Selections{
    ADD_WORKER = 0,
    REMOVE_WORKER,
    FIND_WORKER,
    DISPLAY_DAY,
    EXIT,
    NUM_OF_SELECTIONS
};

Worker menu_add(){
    std::cout << "Enter name:" << std::endl;
    std::string name_of_worker;
    std::cin >> name_of_worker;
    std::cout << "Enter sex:" << std::endl;
    std::cout << static_cast<int>(Genders::MALE) << " : male" << std::endl;
    std::cout << static_cast<int>(Genders::FEMALE) << " : female" << std::endl ;
    int gender_of_worker;
    Genders gender_of_worker_cast;
    std::cin >> gender_of_worker;
    gender_of_worker_cast = static_cast<Genders>(gender_of_worker);
    std::cout << "Enter position:" << std::endl;
    std::cout << static_cast<int>(Positions::FULL_TIME) << " : full time." << std::endl;
    std::cout << static_cast<int>(Positions::HALF_TIME) << " : half time." << std::endl;
    std::cout << static_cast<int>(Positions::EXTRA_WORKER) << " : extra worker." << std::endl;
    std::cout << static_cast<int>(Positions::PARENT_LEAVE) << " : parental leave." << std::endl;
    std::cout << static_cast<int>(Positions::TEMPORARY_POSITION) << " : temporary position." << std::endl;
    std::cout << static_cast<int>(Positions::WEEKEND_WORKER) << " : weekend worker." << std::endl;
    int position_of_worker;
    Positions position_of_worker_cast;
    std::cin >> position_of_worker;
    position_of_worker_cast = static_cast<Positions>(position_of_worker);
    std::cout << "Enter personal number:" << std::endl;
    long personal_number;
    std::cin >> personal_number;
    return Worker(name_of_worker,gender_of_worker_cast,position_of_worker_cast, personal_number);
}

void display_day(Work_day this_day){

}


void menu(){
    Work_day current_day = Work_day();
    current_day.change_resolution(48);
    bool user_exit = false;
    while(!user_exit){
        std::cout << "Choose the following:" << std::endl;
        std::cout << Selections::ADD_WORKER << " :add a user." << std::endl;
        std::cout << Selections::REMOVE_WORKER << " :remove a user."<< std::endl;
        std::cout << Selections::FIND_WORKER << " :find a user."<< std::endl;
        std::cout << Selections::DISPLAY_DAY << " :display the day."<< std::endl;
        std::cout << Selections::EXIT << " :exit."<< std::endl;
        int user_selection;
        std::cin >> user_selection;
        auto user_switch = static_cast<Selections>(user_selection);
        switch(user_switch){
            default:
                user_exit = false;
                break;
            case Selections::ADD_WORKER:
                current_day.add_worker(menu_add());
                break;
            case Selections::REMOVE_WORKER:
                break;
            case Selections::FIND_WORKER:
                break;
            case Selections::DISPLAY_DAY:
                break;
            case Selections::EXIT:
                user_exit = true;
                break;

        }
    }
}