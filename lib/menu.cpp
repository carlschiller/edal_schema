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
    EXIT
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
        GO_BACK
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
            for(const char * string_selection : worker_strings){
                std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
                index++;
            }
            std::cout << "Enter selection:" << std::endl;
            std::string selection;
            std::cin >> selection;
            Worker_selections user_choice;
            try {
                int before_cast = std::stoi(selection);
                user_choice = static_cast<Worker_selections >(before_cast);
            }
            catch(std::exception& e){
                std::cout << "Enter valid selection.\n";
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
            }
        }
    }
}

namespace t_management{
    enum Task_selections{
        DISPLAY_TASKS,
        ADD_NEW_TASK,
        REMOVE_TASK,
        GO_BACK
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
            for(const char * string_selection : task_strings){
                std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
                index++;
            }
            std::cout << "Enter selection:" << std::endl;
            std::string selection;
            std::cin >> selection;
            Task_selections user_choice;
            try {
                int before_cast = std::stoi(selection);
                user_choice = static_cast<Task_selections >(before_cast);
            }
            catch(std::exception& e){
                std::cout << "Enter valid selection.\n";
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
        GO_BACK
    };

    static const char* wd_strings[] = {
            "Display day statistics",
            "Change resolution",
            "Add worker to this day",
            "Remove worker from this day",
            "Go back"
    };

    void wd_menu(){
        bool user_exit = false;
        while(!user_exit){
            int index = 0;
            for(const char * string_selection : wd_strings){
                std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
                index++;
            }
            std::cout << "Enter selection:" << std::endl;
            std::string selection;
            std::cin >> selection;
            Work_day_selections user_choice;
            try {
                int before_cast = std::stoi(selection);
                user_choice = static_cast<Work_day_selections>(before_cast);
            }
            catch(std::exception& e){
                std::cout << "Enter valid selection.\n";
            }
            switch(user_choice){
                default:{break;}
                case Work_day_selections::DISPLAY_DAY_STATISTICS :{
                    clear_terminal();
                    break;
                }
                case Work_day_selections ::ADD_WORKER_TO_THIS_DAY :{
                    clear_terminal();
                    break;
                }
                case Work_day_selections ::REMOVE_WORKER_FROM_THIS_DAY :{
                    clear_terminal();
                    break;
                }
                case Work_day_selections ::CHANGE_RESOLUTION :{
                    clear_terminal();
                    break;
                }
                case Work_day_selections ::GO_BACK:{
                    clear_terminal();
                    user_exit = true;
                    break;
                }
            }
        }
    }
}

namespace rf_management{
    enum Reference_selections{
        DISPLAY_REFERENCE_MATRIX,
        ADD_COLUMN,
        REMOVE_COLUMN,
        GO_BACK
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
            std::cin >> time_slot[0];
            try { time_in_int[0] = std::stoi(time_slot[0]); }
            catch(std::exception& e){
                invalid = true;
            }

            std::cout << "Enter end time\n";
            std::cin >> time_slot[1];
            try { time_in_int[1] = std::stoi(time_slot[1]); }
            catch(std::exception& e){
                invalid = true;
            }

            std::vector<std::string> task_names = available_tasks.get_all_task_names();
            int index = 0;
            std::string user_selection_name;
            int user_selection_int = 0;
            for(std::string &name : task_names){
                std::cout << std::to_string(index) << ": " << name << std::endl;
                index++;
            }
            std::cout << "Enter desired task\n";
            std::cin >> user_selection_name;
            try {
                user_selection_int = std::stoi(user_selection_name);
                if(user_selection_int > task_names.size() - 1 && user_selection_int < 0){ invalid = true; }
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

    void display_matrix(Work_day& current_day){
        std::vector<std::vector<Task>> matrix = current_day.get_reference_matrix();
        auto column_space = static_cast<int>(matrix.size());
        int row_space = current_day.get_resolution();

        for(int row = 0; row < row_space; row++){
            std::string row_string;
            for(int column = 0; column < column_space; column++){
                row_string += matrix[column][row].name() + " ";
            }
            std::cout << row_string << std::endl;
        }
    }

    void rf_menu(Work_day & current_day, Tasks& available_tasks){
        bool user_exit = false;
        while(!user_exit){
            int index = 0;
            for(const char * string_selection : rf_strings){
                std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
                index++;
            }
            std::cout << "Enter selection:" << std::endl;
            std::string selection;
            std::cin >> selection;
            Reference_selections user_choice;
            try {
                int before_cast = std::stoi(selection);
                user_choice = static_cast<Reference_selections>(before_cast);
            }
            catch(std::exception& e){
                std::cout << "Enter valid selection.\n";
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
                    break;
                }
                case Reference_selections ::GO_BACK:{
                    clear_terminal();
                    user_exit = true;
                    break;
                }
            }
        }
    }

}


void menu(){
    Tasks available_tasks = Tasks();
    Workers available_workers = Workers();
    Genders gender_util = Genders();
    Positions positions_util = Positions();

    Work_day current_day = Work_day();
    current_day.change_resolution(48);

    bool user_exit = false;
    while(!user_exit){
        int index = 0;
        for(const char * string_selection : outer_strings){
            std::cout << std::to_string(index) + ": "<< string_selection << std::endl;
            index++;
        }
        std::cout << "Enter selection:" << std::endl;
        std::string selection;
        std::cin >> selection;
        Outer_selections user_choice;
        try {
            int before_cast = std::stoi(selection);
            user_choice = static_cast<Outer_selections>(before_cast);
        }
        catch(std::exception& e){
            std::cout << "Enter valid selection.\n";
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
                wd_management::wd_menu();
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
        }
    }
}