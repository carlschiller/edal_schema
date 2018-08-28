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
    SEE_WORKERS,
    ADD_TASK,
    REMOVE_TASK,
    NEW_TASK,
    DISPLAY_TASKS,
    DISPLAY_DAY,
    EXIT,
};

Worker menu_add_worker(){
    std::string name_of_worker;
    std::cin.ignore();
    std::cout << "Enter name:" << std::endl;
    getline(std::cin,name_of_worker); //gets whole line
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
    std::vector<std::string> current_tasks = work_day.work_day_tasks.get_all_task_names();
    std::vector<int> current_task_id = work_day.work_day_tasks.get_all_task_values();
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
    Tasks current_tasks = work_day.work_day_tasks;
    // integer below is for padding of spaces in order to get even columns printed.
    int max_length_of_task_names = get_max_char_length(matrix, current_tasks,work_day.get_resolution())+1;

    // printing a header for the columns:
    std::string header;
    unsigned long clock_padding = clock_parser(work_day.get_resolution(), 0).length()+2;
    header += std::string(clock_padding,' '); // padding for clock.
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


Work_day add_new_task_menu(Work_day day){
    std::string name_of_task;
    bool flexibility;
    std::string temp_flex;
    Genders sex;
    std::string temp_sex;
    std::cout << "Enter name of task" << std::endl;
    std::cin >> name_of_task;
    std::cout << "Enter flexibility (true/false)" << std::endl;
    std::cin >> temp_flex;
    flexibility = Converters::string_to_boolean(temp_flex);
    std::cout << "Enter sex req: (MALE/FEMALE/NONE)" << std::endl;
    std::cin >> temp_sex;
    sex = Converters::string_to_sex(temp_sex);
    day.work_day_tasks.add_task(name_of_task,flexibility,sex);
    return day;
}

void display_workers(std::vector<Worker> worker_list){
    int i = 0;
    unsigned int max_char_length = 0;
    std::vector<std::string> console_output;
    for(Worker worker : worker_list){
        std::string console_row;
        console_row += std::to_string(i) + ". ";
        console_row += "Name : ";
        console_row += worker.get_name();
        console_row += ", Gender: ";
        console_row += Converters::sex_to_string(worker.get_gender());
        console_row += ", Position: ";
        console_row += Converters::positions_to_string(worker.get_position());
        console_row += ", Personal number: ";
        console_row += std::to_string(worker.get_personal_number());
        console_row += "\n";
        console_output.push_back(console_row);
        if(max_char_length < console_row.length()){
            max_char_length = static_cast<unsigned int>(console_row.length());
        }
        i++;
    }
    // make padding before and after as to make sure to make it readable.
    std::string padding = std::string(max_char_length, '=');
    padding += "\n";
    std::vector<std::string>::iterator it;
    it = console_output.begin();
    console_output.insert(it,padding);
    console_output.push_back(padding);
    // print output.
    for(const std::string &row : console_output){
        std::cout << row;
    }
}

std::string remove_worker(std::vector<Worker> worker_list){
    std::cout << "Enter number to remove, or any other number or character to ignore." << std::endl;
    std::string selection;
    std::cin >> selection;
    try{
        int integer_selection = std::stoi(selection);
        if(integer_selection < worker_list.size()-1 || integer_selection >= 0){
            return worker_list[integer_selection].get_name();
        }
        else{
            return "";
        }

    }
    catch(std::invalid_argument&){
        return "";
    }
}

Work_day remove_task_id(Work_day current_day){
    if(current_day.get_work_day_reference_size() > 0){
        std::cout << "Please select column to remove" << std::endl;
        int id;
        std::cin >> id;
        current_day.remove_work_day_reference_column(id);
    }
    return current_day;
}

void menu(){
    Work_day current_day = Work_day();
    current_day.change_resolution(48);
    bool user_exit = false;
    while(!user_exit){
        std::cout << "Choose the following:" << std::endl;
        std::cout << Selections::ADD_WORKER << " :add a worker." << std::endl;
        std::cout << Selections::REMOVE_WORKER << " :remove a worker."<< std::endl;
        std::cout << Selections::SEE_WORKERS << " :display workers."<< std::endl;
        std::cout << Selections::ADD_TASK << " :add a task to work day reference matrix."<< std::endl;
        std::cout << Selections::REMOVE_TASK << " :remove a task from work day reference matrix."<< std::endl;
        std::cout << Selections::NEW_TASK << " :create new task."<< std::endl;
        std::cout << Selections::DISPLAY_TASKS << " :display reference matrix."<< std::endl;
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
                current_day.add_worker(menu_add_worker());
                break;
            case Selections::REMOVE_WORKER:
            {
                std::vector<Worker> worker_list = current_day.get_all_workers();
                display_workers(worker_list);
                std::string worker_to_be_removed = remove_worker(worker_list);
                if(worker_to_be_removed.empty()){
                    std::cout << "Nothing removed." << std::endl;
                }
                else{
                    current_day.remove_worker(worker_to_be_removed);
                }
                break;
            }
            case Selections::SEE_WORKERS:
                display_workers(current_day.get_all_workers());
                break;
            case Selections::ADD_TASK:
            {
                std::vector<int> task_adder = menu_add_task(current_day);
                current_day.add_work_day_reference_column(task_adder[2],task_adder[0],task_adder[1]);
                break;
            }
            case Selections::REMOVE_TASK:
            {
                display_day_tasks(current_day);
                current_day = remove_task_id(current_day);
                break;
            }
            case Selections::NEW_TASK:
            {
                current_day = add_new_task_menu(current_day);
                break;
            }
            case Selections::DISPLAY_TASKS:
                display_day_tasks(current_day);
                break;
            case Selections::DISPLAY_DAY:
                break;
            case Selections::EXIT:
                user_exit = true;
                current_day.save_work_day();
                current_day.save_workers_to_file();
                break;

        }
    }
}