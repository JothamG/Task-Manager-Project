#include <iostream>
#include <string>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

struct Tasks
{
    int id;
    std::string name;
    bool completed;
}
Task;

json loadfromfile()
{
    std::ifstream infile("tasklist.json");

    if (!infile.is_open())
    {
        std::cout << "Error: Could not open file. \n";
        return json::array();
    }
    json tasks_json;
    infile >> tasks_json;
    infile.close();

    if (!tasks_json.is_array())
    {
        std::cout << "Error: json file invalid format. \n";
        return json::array();
    }

    return tasks_json;

};

void savetofile(const json& tasks_json)
{
    std::ofstream outfile("tasklist.json");
    outfile << tasks_json.dump(4);
    outfile.close();
}

int getnextID(json tasks_json)
{
    int maxID = 0;
    for (const auto& task : tasks_json)
    {
        if(task["id"] > maxID)
        {
            maxID = task["id"];
        }
    }
    return maxID;
}

void AddTask(json& tasks_json)
{
    Tasks t;

    std::cout << "Enter the Task Name: ";
    std::cin.ignore();
    std::getline(std::cin, t.name);
    t.id = getnextID(tasks_json) + 1;
    t.completed = false;

    json obj = 
    {
        {"id", t.id},
        {"name", t.name},
        {"completed", t.completed}
    };
    tasks_json.push_back(obj);
    std::cout << "The task has been added.\n";
}

void PrintTasks(const json& tasks_json)
{

    for (const auto&task : tasks_json)
    {
        std::cout << "ID: " << task["id"] << " | Name: " << task["name"] << " | Completed: " << (task["completed"] ? "Yes" : "No") << "\n";
    }
}

void DeleteTask(json& tasks_json)
{
    int id;
    std::cout << "Enter the ID of the task to delete: ";
    std::cin >> id;
    for (auto it = tasks_json.begin(); it != tasks_json.end(); ++it)
    {
        if ((*it)["id"] == id)
        {
            tasks_json.erase(it);
            break;
        }
    }
}

bool EditTask(json& tasks_json)
{
    std::cout << "Would you edit the name of the task or its completion status? (1/2): ";
    int choice;
    std::cin >> choice;
    int id;
    std::cout << "Enter the ID of the task to edit: ";
    std::cin >> id;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for(auto& task: tasks_json)
    {
        if (task["id"] == id)
        {
            if (choice == 1)
            {
                std::string newName;
                std::cout << "Enter the new name: ";
                std::getline(std::cin, newName);

                task["name"] = newName;
                return true;
            }
            else if (choice == 2)
            {
                bool newStatus;
                std::cout << "Enter the new completion status (0 for incomplete, 1 for complete): ";
                std::cin >> newStatus;
                task["completed"] = newStatus;
                return true;
            }
        }
    }
    return false;
}

int main()
{
    int choice;
    json tasks_json = loadfromfile();
    int maxID = getnextID(tasks_json);
    std::cout << "Welcome to your Task Manager, what would you like to do today?\n";
    do
    {
        std::cout << "\n1. Add a Task\n2. Edit a Task\n3. Delete a Task\n4. See all Tasks\n5. Exit Program\n\n";
        std::cout << "Your Choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                AddTask(tasks_json);
            break;
            case 2:
                if (!EditTask(tasks_json))
                {
                    std::cout << "Task not found.\n";
                }
            break;
            case 3:
                DeleteTask(tasks_json);
            break;
            case 4:
                PrintTasks(tasks_json);
            break;
            case 5:
                std::cout << "Exiting Task Manager. Goodbye!\n";
            break;
            default:
                std::cout << "Invalid choice, please try again.\n";
            }
    }
    while(choice != 5);
    

    savetofile(tasks_json);
    return 0;
}