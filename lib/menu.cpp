//
// Created by Carl Schiller on 2018-06-11.
//

#include <iostream>
#include "menu.h"
#include "core.h"
#include <cmath>

enum Selections{
    ADD_WORKER = 0,
    REMOVE_WORKER,
    FIND_WORKER,
    ADD_TASK,
    DISPLAY_TASKS,
    DISPLAY_DAY,
    EXIT,
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

std::vector<int> menu_add_task(Work_day work_day){
    std::vector<int> selections;
    selections.reserve(3);
    std::cout << "Enter starting time:" << std::endl;
    int start_time;
    std::cin >> start_time;
    std::cout << "Enter end time:" << std::endl;
    int end_time;
    std::cin >> end_time;
    std::vector<std::string> current_tasks = work_day.get_tasks().get_all_task_names();
    std::vector<int> current_task_id = work_day.get_tasks().get_all_task_values();
    for(int i = 0; i < current_tasks.size(); ++i){
        std::cout << current_task_id[i] << ": "<< current_tasks[i] << std::endl;
    }
    int task;
    std::cin >> task;
    selections.push_back(start_time);
    selections.push_back(end_time);
    selections.push_back(task);
    return selections;
}

// parses resolutions steps into a string representation of time.
std::string clock_parser(int resolution, int time_to_convert){
    double time_step = 24/(double)resolution;
    auto hour = (int)floor(time_step*time_to_convert);
    auto minute = (int)round((time_step*time_to_convert-hour)*60);
    std::string hour_string = std::to_string(hour);
    std::string minute_string = std::to_string(minute);
    if(hour_string.length() == 1){
        hour_string = "0" + hour_string;
    }
    if(minute_string.length() == 1){
        minute_string = "0" + minute_string;
    }
    return hour_string + ":" + minute_string;
}

// finds max length of name of task in the matrix.
int get_max_char_length(std::vector<std::vector<int>> matrix, Tasks current_tasks, int resolution){
    int max_guess = 0;
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            if(max_guess < current_tasks.get_task_name(matrix[j][i]).length()){
                max_guess = (int)current_tasks.get_task_name(matrix[j][i]).length();
            }
        }
    }
    return max_guess;
}

// displays a column view of tasks, with names of tasks, for user readability and debugging purposes.
void display_day_tasks(Work_day work_day){
    std::vector<std::vector<int>> matrix = work_day.get_work_day_reference();
    Tasks current_tasks = work_day.get_tasks();
    // integer below is for padding of spaces in order to get even columns printed.
    int max_length_of_task_names = get_max_char_length(matrix, current_tasks,work_day.get_resolution());

    // printing a header for the columns:
    std::string header;
    unsigned long clock_padding = clock_parser(work_day.get_resolution(), 0).length();
    header += std::string(clock_padding + 2,' '); // padding for clock.
    for(int k = 0; k < matrix.size(); ++k){
        std::string column_name = "Column " + std::to_string(k);
        unsigned long column_padding = max_length_of_task_names - column_name.length();
        column_name += std::string(column_padding,' ');
        header += column_name;
    }
    // printing header and a separation line.
    std::cout << header << std::endl;
    std::cout << std::string(clock_padding, '-') + std::string((unsigned long)max_length_of_task_names*matrix.size(), '-') << std::endl;

    for(int i = 0; i < work_day.get_resolution(); ++i){
        std::string temp_row = clock_parser(work_day.get_resolution(),i) + ": "; // print clock as first column.
        for(int j = 0; j < matrix.size(); ++j){
            // padding for current column.
            int padding = max_length_of_task_names - (int)current_tasks.get_task_name(matrix[j][i]).length();
            if(padding > 0){
                temp_row += current_tasks.get_task_name(matrix[j][i]) + std::string((unsigned long)padding, ' ');
            }
            else{
                temp_row += current_tasks.get_task_name(matrix[j][i]);
            }
        }
        std::cout << temp_row << std::endl; // prints row, repeat with next time slot.
    }
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
        std::cout << Selections::ADD_TASK << " :add task."<< std::endl;
        std::cout << Selections::DISPLAY_TASKS << " :display tasks."<< std::endl;
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
            case Selections::ADD_TASK:
            {
                std::vector<int> task_adder = menu_add_task(current_day);
                current_day.add_work_day_reference_column(task_adder[2],task_adder[0],task_adder[1]);
                break;
            }
            case Selections::DISPLAY_TASKS:
                display_day_tasks(current_day);
                break;
            case Selections::DISPLAY_DAY:
                break;
            case Selections::EXIT:
                user_exit = true;
                break;

        }
    }
}