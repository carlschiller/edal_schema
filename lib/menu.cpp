//
// Created by Carl Schiller on 2018-06-11.
//

#include <iostream>
#include "menu.h"
#include "core.h"
#include <cmath>

#include <unistd.h>
#include <term.h>
#include <curses.h>


void clear_terminal()
{
    if (!cur_term)
    {
        int result;
        setupterm( NULL, STDOUT_FILENO, &result );
        if (result <= 0) return;
    }

    putp( tigetstr(const_cast<char *>("clear")) );
}

enum Outer_selections{
    WORKER_MANAGEMENT = 0,
    TASK_MANAGEMENT,
    WORK_DAY_MANAGEMENT,
    REFERENCE_MANAGEMENT,
    EXIT,
    WRONG
};

static const char* outer_strings[] = {
        "Worker management",
        "Task management",
        "Work day management",
        "Reference management",
        "Exit"
};

namespace w_management{
    enum Worker_selections{
        DISPLAY_WORK_FORCE = 0,
        ADD_NEW_WORKER_TO_WORK_FORCE,
        REMOVE_WORKER_FROM_WORK_FORCE,
        GO_BACK,
        WRONG
    };

    static const char* worker_strings[] = {
            "Display work force",
            "Add new worker to work force",
            "Remove worker from work force",
            "Go back"
    };

    void work_menu(){
        bool user_exit = false;
        while(!user_exit){
            int index = 0;
            std::cout << "You are here: [" << outer_strings[0] << "]\n";
            for(const char * string_selection : worker_strings){
                std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
                index++;
            }
            std::cout << "Enter selection:" << std::endl;
            std::string selection;
            std::cin.clear();
            std::cin >> selection;
            Worker_selections user_choice;
            try {
                int before_cast = std::stoi(selection);
                user_choice = static_cast<Worker_selections >(before_cast);
            }
            catch(std::exception& e){
                clear_terminal();
                std::cout << "Enter valid selection.\n";
                user_choice = Worker_selections ::WRONG;
            }
            switch(user_choice){
                default:{break;}
                case Worker_selections::DISPLAY_WORK_FORCE:{
                    clear_terminal();
                    break;
                }
                case Worker_selections::ADD_NEW_WORKER_TO_WORK_FORCE:{
                    clear_terminal();
                    break;
                }
                case Worker_selections::REMOVE_WORKER_FROM_WORK_FORCE:{
                    clear_terminal();
                    break;
                }
                case Worker_selections::GO_BACK:{
                    clear_terminal();
                    user_exit = true;
                    break;
                }
                case Worker_selections::WRONG:{ break; }
            }
        }
    }
}

namespace t_management{
    enum Task_selections{
        DISPLAY_TASKS,
        ADD_NEW_TASK,
        REMOVE_TASK,
        GO_BACK,
        WRONG
    };

    static const char* task_strings[] = {
            "Display tasks",
            "Add new task",
            "Remove task",
            "Go back"
    };

    void task_menu(){
        bool user_exit = false;
        while(!user_exit){
            int index = 0;
            std::cout << "You are here: [" << outer_strings[1] << "]\n";
            for(const char * string_selection : task_strings){
                std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
                index++;
            }
            std::cout << "Enter selection:" << std::endl;
            std::string selection;
            std::cin.clear();
            std::cin >> selection;
            Task_selections user_choice;
            try {
                int before_cast = std::stoi(selection);
                user_choice = static_cast<Task_selections >(before_cast);
            }
            catch(std::exception& e){
                clear_terminal();
                std::cout << "Enter valid selection.\n";
                user_choice = Task_selections ::WRONG;
            }
            switch(user_choice){
                default:{break;}
                case Task_selections::DISPLAY_TASKS :{
                    clear_terminal();
                    break;
                }
                case Task_selections::ADD_NEW_TASK :{
                    clear_terminal();
                    break;
                }
                case Task_selections::REMOVE_TASK :{
                    clear_terminal();
                    break;
                }
                case Task_selections::GO_BACK:{
                    clear_terminal();
                    user_exit = true;
                    break;
                }
                case Task_selections::WRONG:{ break; }
            }
        }
    }
}


namespace wd_management{
    enum Work_day_selections{
        DISPLAY_DAY_STATISTICS,
        CHANGE_RESOLUTION,
        ADD_WORKER_TO_THIS_DAY,
        REMOVE_WORKER_FROM_THIS_DAY,
        GO_BACK,
        WRONG
    };

    static const char* wd_strings[] = {
            "Display day statistics",
            "Change resolution",
            "Add worker to this day",
            "Remove worker from this day",
            "Go back"
    };

    std::string worker_printer(Worker & your_worker, Genders & available_genders, Positions & available_positions){
        std::string out_line;
        out_line += "name:" + your_worker.get_name() + ",";
        int worker_gender = your_worker.get_gender();
        out_line += "gender:" + available_genders.get_string(worker_gender) + ",";
        int worker_position = your_worker.get_position();
        out_line += "position:" + available_positions.get_string(worker_position) + ",";
        long worker_personal_number = your_worker.get_personal_number();
        out_line += "personal_number:" + std::to_string(worker_personal_number);
        return out_line;
    }

    Worker * worker_selector(std::vector<Worker> & worker_list, Genders & available_genders, Positions & available_positions){
        bool correct = false;
        int user_id = -1;
        while(!correct){
            std::cout << "=========================\n";

            int i = 0;
            for(Worker worker : worker_list){
                std::string out_line;
                out_line += std::to_string(i) + ":";
                out_line += worker_printer(worker,available_genders,available_positions);
                std::cout << out_line << std::endl;
                i++;
            }
            std::cout << "=========================\n";

            std::cout << "Please give index number to select worker, enter any non number to cancel.\n";
            std::string user_selection;

            std::cin.clear();
            std::cin >> user_selection;
            try{
                user_id = std::stoi(user_selection);
                if(user_id < 0 || user_id > worker_list.size() -1){
                    clear_terminal();
                    std::cout << "Try again, enter index corresponding to a worker.\n";
                }
                else{
                    Worker * worker_returner = &worker_list[user_id];
                    return worker_returner;
                }

            }
            catch(std::exception& e){ correct = true; }
        }
        return nullptr;
    }

    void display_day(Work_day & current_day, Genders & available_genders, Positions & available_positions){
        std::cout << "Workers for the day:\n";
        std::cout << "====================\n";
        int i = 0;

        for(auto it : current_day.get_all_workers()){
            std::string out_line;
            Worker your_worker = it.second;

            out_line += std::to_string(i) + ":";
            out_line += worker_printer(your_worker,available_genders,available_positions);
            std::cout << out_line << std::endl;
            i++;
        }
        std::cout << "====================\n";
        std::cout << "Resolution:" << current_day.get_resolution() << "\n";
        std::cout << "====================\n";
    }

    void add_worker(Work_day & current_day, std::vector<Worker> & worker_list, Genders & available_genders, Positions & available_positions){
        std::cout << "Adding a worker to the day.\n";
        std::cout << "Workers in the work pool:\n";
        Worker * selected_worker = worker_selector(worker_list,available_genders,available_positions);
        if(selected_worker != nullptr){
            if(current_day.is_worker_in_day(*selected_worker)){
                clear_terminal();
                std::cout << "Worker already in the day\n";
            }
            else{
                clear_terminal();
                current_day.add_worker(*selected_worker);
            }
        }
        else{ clear_terminal(); }
    }

    void remove_worker(Work_day & current_day, Genders & available_genders, Positions & available_positions){
        std::vector<Worker> worker_list;
        for(auto it : current_day.get_all_workers()){
            worker_list.push_back(it.second);
        }

        if(worker_list.empty()){ std::cout << "No workers in day, can't remove worker.\n"; }
        else{
            std::cout << "Removing a worker from the day.\n";
            std::cout << "Workers for the day:\n";

            Worker * selected_worker = worker_selector(worker_list,available_genders,available_positions);
            if(selected_worker != nullptr){
                std::string name = selected_worker->get_name();
                current_day.remove_worker(name);
            }
            clear_terminal();
        }
    }

    void change_resolution(Work_day & current_day){
        bool correct = false;

        std::cout << "Changing resolution will clear all reference columns because of compatibility issues, wish to continue? [Y/N]\n";
        std::string yes_no;
        std::cin.clear();
        std::cin >> yes_no;

        if(yes_no == "Y"){ current_day.clear_reference_matrix(); }
        else{ correct = true; }

        while(!correct){
            std::cout << "Enter new resolution, enter any other non digit to exit" << std::endl;
            std::string res_string;
            std::cin.clear();
            std::cin >> res_string;
            int res;

            try { res = std::stoi(res_string); }
            catch(std::exception& e){ break; }

            if((60*24)%res != 0){
                clear_terminal();
                std::cout << "Enter resolution that will divide 60*24\n";
            }
            else{
                current_day.change_resolution(res);
                correct = true;
            }
        }
        clear_terminal();
    }

    void wd_menu(Work_day & current_day, std::vector<Worker> & worker_list, Genders available_genders, Positions available_positions){
        bool user_exit = false;
        while(!user_exit){
            int index = 0;
            std::cout << "You are here: [" << outer_strings[2] << "]\n";
            for(const char * string_selection : wd_strings){
                std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
                index++;
            }
            std::cout << "Enter selection:" << std::endl;
            std::string selection;
            std::cin.clear();
            std::cin >> selection;
            Work_day_selections user_choice;
            try {
                int before_cast = std::stoi(selection);
                user_choice = static_cast<Work_day_selections>(before_cast);
            }
            catch(std::exception& e){
                clear_terminal();
                std::cout << "Enter valid selection.\n";
                user_choice = Work_day_selections ::WRONG;
            }
            switch(user_choice){
                default:{break;}
                case Work_day_selections::DISPLAY_DAY_STATISTICS :{
                    clear_terminal();
                    display_day(current_day,available_genders,available_positions);
                    break;
                }
                case Work_day_selections ::ADD_WORKER_TO_THIS_DAY :{
                    clear_terminal();
                    add_worker(current_day,worker_list,available_genders,available_positions);
                    break;
                }
                case Work_day_selections ::REMOVE_WORKER_FROM_THIS_DAY :{
                    clear_terminal();
                    remove_worker(current_day,available_genders,available_positions);
                    break;
                }
                case Work_day_selections ::CHANGE_RESOLUTION :{
                    clear_terminal();
                    change_resolution(current_day);
                    break;
                }
                case Work_day_selections ::GO_BACK:{
                    clear_terminal();
                    user_exit = true;
                    break;
                }
                case Work_day_selections ::WRONG:{ break; }
            }
        }
    }
}

namespace rf_management{
    enum Reference_selections{
        DISPLAY_REFERENCE_MATRIX,
        ADD_COLUMN,
        REMOVE_COLUMN,
        GO_BACK,
        WRONG
    };

    static const char* rf_strings[] = {
            "Display reference matrix",
            "Add column",
            "Remove column",
            "Go back"
    };

    void add_column(Work_day& current_day, Tasks& available_tasks){
        bool user_correct = false;
        int time_in_int[2];
        Task user_task;
        while(!user_correct){
            bool invalid = false;
            std::string time_slot[2];

            std::cout << "Enter start time\n";
            std::cin.clear();
            std::cin >> time_slot[0];
            try { time_in_int[0] = std::stoi(time_slot[0]); }
            catch(std::exception& e){
                invalid = true;
            }

            clear_terminal();
            std::cout << "Enter end time\n";
            std::cin.clear();
            std::cin >> time_slot[1];
            try { time_in_int[1] = std::stoi(time_slot[1]); }
            catch(std::exception& e){
                invalid = true;
            }

            clear_terminal();
            std::vector<std::string> task_names = available_tasks.get_all_task_names();
            int index = 0;
            std::string user_selection_name;
            int user_selection_int = 0;
            for(std::string &name : task_names){
                std::cout << std::to_string(index) << ": " << name << std::endl;
                index++;
            }
            std::cout << "Enter desired task\n";
            std::cin.clear();
            std::cin >> user_selection_name;
            try {
                user_selection_int = std::stoi(user_selection_name);
                if(user_selection_int > task_names.size() - 1  && user_selection_int < 0){ invalid = true; }
            }
            catch(std::exception& e){
                invalid = true;
            }
            if(!invalid){
                user_correct = true;
                user_task = available_tasks.get_all_tasks()[task_names[user_selection_int]];
            }
        }
        current_day.add_reference_column(user_task,time_in_int[0],time_in_int[1]);
        clear_terminal();
    }

    std::vector<unsigned long> padding_list(Work_day &current_day){
        std::vector<unsigned long> max_size;
        for(std::vector<Task> column : current_day.get_reference_matrix()){
            unsigned long best_guess = 0;
            for(Task task : column){
                if(task.name().length() > best_guess){ best_guess = task.name().length(); }
            }
            max_size.push_back(best_guess+1);
        }
        return max_size;
    }

    std::string add_padding(unsigned long padding , std::string task_name){
        unsigned long int number_of_spaces = padding - task_name.length();
        std::string line = task_name + std::string(number_of_spaces,' ');
        return line;
    }

    std::string flush_with_zeros(std::string input){
        if(input.length() != 1){ return input; }
        else{ return ("0" + input); }
    }

    std::vector<std::string> clock_column(int resolution){
        std::vector<std::string> column;

        int minutes_in_a_day = 24*60;
        int minutes_per_snippet = minutes_in_a_day/resolution;
        for(int snippet = 0; snippet < resolution; snippet++){
            int dummy_minutes = snippet*minutes_per_snippet;
            int dummy_hours = dummy_minutes/60;
            std::string hours = std::to_string(dummy_hours);
            int real_minutes = dummy_minutes%60;
            std::string minutes = std::to_string(real_minutes);
            std::string time_string = flush_with_zeros(hours) + ":" + flush_with_zeros(minutes) + "|";
            column.push_back(time_string);
        }
        return column;
    }

    void display_matrix(Work_day& current_day){
        // get padding lengths per column.
        std::vector<unsigned long> list_of_padding_lengths = padding_list(current_day);
        unsigned long total_row_length = 0;
        for(unsigned long piece : list_of_padding_lengths){
            total_row_length += piece;
        }
        total_row_length += 6; // for clock padding as well.

        // load matrix and get dimensions.
        std::vector<std::vector<Task>> matrix = current_day.get_reference_matrix();
        auto column_space = static_cast<int>(matrix.size());
        int row_space = current_day.get_resolution();

        // get time vector.
        std::vector<std::string> time_vector = clock_column(row_space);

        std::string delimiter = std::string(total_row_length, '=');
        std::string header;
        std::cout << delimiter << std::endl;
        header += "Time |";
        for(int i = 0; i < column_space; i++){
            std::string number = " Column " + std::to_string(i);
            header += add_padding(list_of_padding_lengths[i]-1, number);
            header += "|";
        }
        std::cout << header << std::endl;
        std::cout << delimiter << std::endl;

        for(int row = 0; row < row_space; row++){
            std::string row_string;

            row_string += time_vector[row];
            for(int column = 0; column < column_space; column++){
                std::string task_name = matrix[column][row].name();
                row_string += add_padding(list_of_padding_lengths[column]-1, task_name);
                row_string += "|";
            }
            std::cout << row_string << std::endl;
        }
        std::cout << delimiter << std::endl;
    }

    void remove_column(Work_day & current_day){
        display_matrix(current_day);
        std::cout << "Please choose column number to remove, enter any other key to abort.\n";
        std::string selection;
        std::cin.clear();
        std::cin >> selection;
        int column_index;
        try {
            column_index = std::stoi(selection);
            if(column_index < current_day.get_reference_matrix().size() && column_index >= 0){ current_day.remove_reference_column(column_index); }
        }
        catch(std::exception& e){
            // do nothing.
            clear_terminal();
        }
        clear_terminal();
    }

    void rf_menu(Work_day & current_day, Tasks& available_tasks){
        bool user_exit = false;
        while(!user_exit){
            int index = 0;
            std::cout << "You are here: [" << outer_strings[3] << "]\n";
            for(const char * string_selection : rf_strings){
                std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
                index++;
            }
            std::cout << "Enter selection:" << std::endl;
            std::string selection;
            std::cin.clear();
            std::cin >> selection;
            Reference_selections user_choice;
            try {
                int before_cast = std::stoi(selection);
                user_choice = static_cast<Reference_selections>(before_cast);
            }
            catch(std::exception& e){
                std::cout << "Enter valid selection.\n";
                user_choice = Reference_selections::WRONG;
            }
            switch(user_choice){
                default:{break;}
                case Reference_selections::DISPLAY_REFERENCE_MATRIX :{
                    clear_terminal();
                    display_matrix(current_day);
                    break;
                }
                case Reference_selections ::ADD_COLUMN :{
                    clear_terminal();
                    add_column(current_day,available_tasks);
                    break;
                }
                case Reference_selections ::REMOVE_COLUMN :{
                    clear_terminal();
                    remove_column(current_day);
                    break;
                }
                case Reference_selections ::GO_BACK:{
                    clear_terminal();
                    user_exit = true;
                    break;
                }
                case Reference_selections ::WRONG:{break;}
            }
        }
    }

}


void menu(){
    clear_terminal();
    Tasks available_tasks = Tasks();
    Workers available_workers = Workers();
    Genders gender_util = Genders();
    Positions positions_util = Positions();

    Work_day current_day = Work_day();
    current_day.change_resolution(48); // minutes in a day must be divisible by resolution.

    // load into a vector for easier selection.
    std::vector<Worker> worker_list;
    for(auto it : available_workers.get_all_workers()){
        Worker your_worker = it.second;
        worker_list.push_back(your_worker);
    }

    bool user_exit = false;
    while(!user_exit){
        int index = 0;
        std::cout << "You are here: [Main menu]\n";
        for(const char * string_selection : outer_strings){
            std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
            index++;
        }
        std::cout << "Enter selection:" << std::endl;
        std::string selection;
        std::cin.clear();
        std::cin >> selection;
        Outer_selections user_choice;
        try {
            int before_cast = std::stoi(selection);
            user_choice = static_cast<Outer_selections>(before_cast);
        }
        catch(std::exception& e){
            clear_terminal();
            std::cout << "Enter valid selection.\n";
            user_choice = Outer_selections::WRONG;
        }
        switch(user_choice){
            default:{break;}
            case Outer_selections::WORKER_MANAGEMENT :{
                clear_terminal();
                w_management::work_menu();
                break;
            }
            case Outer_selections::TASK_MANAGEMENT :{
                clear_terminal();
                t_management::task_menu();
                break;
            }
            case Outer_selections::WORK_DAY_MANAGEMENT :{
                clear_terminal();
                wd_management::wd_menu(current_day,worker_list,gender_util,positions_util);
                break;
            }
            case Outer_selections::REFERENCE_MANAGEMENT :{
                clear_terminal();
                rf_management::rf_menu(current_day,available_tasks);
                break;
            }
            case Outer_selections::EXIT :{
                clear_terminal();
                user_exit = true;
                break;
            }
            case Outer_selections::WRONG :{ break; }
        }
    }
}