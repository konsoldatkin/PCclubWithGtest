#include "club.hpp"
#include <cctype>
#include <fstream>
#include <iostream>
#include <istream>
#include <set>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <cmath>

namespace PC_club::core
{
namespace
{
    bool valid_time_form(const std::string& timeStr) {
		std::regex pattern("^([0-1][0-9]|2[0-3]):([0-5][0-9])$");
		return std::regex_match(timeStr, pattern);
	}
    bool non_space_char(const std::string& str) {
		return std::find_if(str.begin(), str.end(), [](char c) {
			return !std::isspace(static_cast<unsigned char>(c));
		}) != str.end();
	}
    void rm_str_from_list(std::list<std::string>& myList, const std::string& target) {
		
		for (auto it = myList.begin(); it != myList.end(); ) {
			
			if (*it == target) {
				it = myList.erase(it);
			}
			else {
				++it;
			}
		}
	}
    bool is_element_in_list(const std::list<std::string>& myList, const std::string& target) {
        for (const auto& element : myList) {
            if (element == target) {
                return true;
            }
        }
        return false;
    }

    bool compare_time(const std::string &time1, const std::string &time2) {
        int hour1 = std::stoi(time1.substr(0, 2));
        int minute1 = std::stoi(time1.substr(3, 2));

        int hour2 = std::stoi(time2.substr(0, 2));
        int minute2 = std::stoi(time2.substr(3, 2));
        if (hour1 < hour2) {
            return true;
        }
        else if (hour1 > hour2) {
            return false;
        }
        else {
            return minute1 <= minute2;
        }
    }
    void calc_of_time(const std::string &time,
        std::pair<int, std::string> &time_tables, std::pair<int, bool> &busy_tables, const int &price) {

        int hour1 = std::stoi((time_tables.second).substr(0, 2));
        int minute1 = std::stoi((time_tables.second).substr(3, 2));

        int hour2 = std::stoi(time.substr(0, 2));
        int minute2 = std::stoi(time.substr(3, 2));

        int difference = (hour2 * 60 + minute2) - (hour1 * 60 + minute1);
        time_tables.first += difference;

        busy_tables.first += std::ceil(double(difference) / 60.0) * price;
        busy_tables.second = 0;
    }
    void correct(const std::string &str, const int &number, std::string &sequence_in_time, const std::string &closing_time) {

        std::string str_hp = str.substr(0, 5);
        if (!compare_time(str_hp, closing_time)) {
            auto error_message = std::string{ "Event time is greater than closing time" } + str;
            throw std::invalid_argument(error_message);
        }
        if (!valid_time_form(str_hp)) {
            auto error_message = std::string{ "Incorrect event time" } + str;
            throw std::invalid_argument(error_message);
        }
        if (!compare_time(sequence_in_time, str_hp)) {
            auto error_message = std::string{ "The time of the previous event is greater" } + str;
            throw std::invalid_argument(error_message);
        }
        sequence_in_time = str_hp;

        if (str.substr(6, 1) == "2") {
            std::regex pattern("[1-" + std::to_string(number) + "]");
            str_hp = str.substr(str.size() - 1);

            if (!std::regex_match(str_hp, pattern)) {
                auto error_message = std::string{ "Incorrect number table" } + str;
                throw std::invalid_argument(error_message);
            }

            std::regex pattern1(" [1-4] [a-z0-9_-]+");
            str_hp = str.substr(5, str.size() - 10);

            if (!std::regex_match(str_hp, pattern1)) {
                auto error_message = std::string{ "Invalid number or name" } + str;
                throw std::invalid_argument(error_message);
            }
            return;

        }
        std::regex pattern(" [1-4] [a-z0-9_-]+");
        str_hp = str.substr(5);

        if (!std::regex_match(str_hp, pattern)) {
            auto error_message = std::string{ "Invalid number or name" } + str;
            throw std::invalid_argument(error_message);
        }

    }
}

PCclub::PCclub(std::string &file_name)
{   

    std::ifstream input;
    input.open(file_name);
    if (!input.is_open())
        throw std::logic_error("");
    std::string received_line;

    std::regex pattern("^[1-9][0-9]*$");

    std::getline(input, received_line);
    if(std::regex_match(received_line, pattern)){
        number_tables = std::stoi(received_line);
    }else{
        auto error_message = std::string{"Incorrect table"} + received_line;
        throw std::invalid_argument(error_message);
    }   

    std::getline(input, received_line);
    if (valid_time_form(received_line.substr(0, 5)) &&
        valid_time_form(received_line.substr(6, 5))) {
        opening_time = received_line.substr(0, 5);
        closing_time = received_line.substr(6, 5);
        if (opening_time == closing_time) {
            auto error_message = std::string{ "Incorrect time" } + received_line;
            throw std::invalid_argument(error_message);
        }
    }else{
        auto error_message = std::string{ "Incorrect time" } + received_line;
        throw std::invalid_argument(error_message);
    }

    std::getline(input, received_line);
    if (std::regex_match(received_line, pattern)) {
        price = std::stoi(received_line);
    }
    else {
        auto error_message = std::string{ "Invalid string" } + received_line;
        throw std::invalid_argument(error_message);
    }

    for (int i = 0; i < number_tables; i++) {
        this->time_tables.push_back({ 0, "00:00" });
        this->busy_tables.push_back({ 0.0, 0 });
    }

    std::cout << opening_time << '\n';

    std::string sequence_in_time = "00:00";
    while (!input.eof()){
        std::getline(input, received_line);
        if (non_space_char(received_line)) {
            correct(received_line, number_tables, sequence_in_time, closing_time);
            query_proces(received_line);
        }
    }

    exit_print();

}

void PCclub::query_proces(std::string str) {

    switch (std::stoi(str.substr(6, 1))) {
    case 1:
        ID_1(str.substr(0, 5), str.substr(8));
        break;
    case 2:
        ID_2(str.substr(0, 5), str.substr(8, str.size() - 10), str.substr(str.size() - 1));
        break;
    case 3:
        ID_3(str.substr(0, 5), str.substr(8));
        break;
    case 4:
        ID_4(str.substr(0, 5), str.substr(8));
        break;
    }
}

void PCclub::ID_1(std::string time, std::string name){
    std::cout << time << " 1 " << name << '\n';
    if (compare_time(time, opening_time) && (time != opening_time)) {
        std::cout << time << " 13 " << "NotOpenYet" << '\n';
        return;
    }
    if (clients.count(name)) {
        std::cout << time  << " 13 " << "YouShallNotPass" << '\n';
        return;
    }
    clients.insert({ name, -1 });
}
void PCclub::ID_2(std::string time, std::string name, std::string num, std::string id) {
    std::cout << time << " " << id << " " << name << " " << num << '\n';
    if (clients.count(name) == 0  ) {
        std::cout << time << " 13 " << "ClientUnknown" << '\n';
        return;
    }
    if (busy_tables[std::stoi(num)-1].second) {
        std::cout << time << " 13 " << "PlaceIsBusy" << '\n';
        return;
    }
    if (clients[name] != -1){
        int num_table = clients[name];
        calc_of_time(time, time_tables[num_table], busy_tables[num_table], price);
        time_tables[std::stoi(num) - 1].second = time;
        busy_tables[std::stoi(num) - 1].second = 1;
        clients[name] = std::stoi(num) - 1;

    }else{
        time_tables[std::stoi(num) - 1].second = time;
        busy_tables[std::stoi(num) - 1].second = 1;
        clients[name] = std::stoi(num) - 1;
        rm_str_from_list(queue, name);
    }


}
void PCclub::ID_3(std::string time, std::string name) {

    if (!clients.count(name)) {
        std::cout << time << " 13 " << "ClientUnknown" << '\n';
        return;
    }
    if (queue.size() < number_tables){
        std::cout << time << " " << "3" << " " << name << '\n';
        if (!is_element_in_list(queue, name)) {
            queue.push_back(name);
        }
        for (int i = 0; i < number_tables; i++) {
            if (busy_tables[i].second == 0) {
                std::cout << time << " 13 " << "ICanWaitNoLonger!" << '\n';
                break;
            }
        }
    }else {
        rm_str_from_list(queue, name);
        ID_4(time, name, "11");
    }
}
void PCclub::ID_4(std::string time, std::string name, std::string id) {
    if (!clients.count(name)) {
        std::cout << time << " 13 " << "ClientUnknown" << '\n';
        return;
    }
    if (clients[name] == -1 && id == "11") {
        clients.erase(name);
        std::cout << time << " " << id << " " << name << '\n';
        return;
    }
    if (clients[name] == -1) {
        clients.erase(name);
        rm_str_from_list(queue, name);
        std::cout << time << " 4 " << name << '\n';
        return;
    }
    std::cout << time << " " << id << " " << name << '\n';
    int num_table = clients[name];
    calc_of_time(time, time_tables[num_table], busy_tables[num_table], price);
    clients.erase(name);
    if (!queue.empty() && time != closing_time) {
        std::string fst_from_queue = queue.front();
        queue.pop_front();
        ID_2(time, fst_from_queue, std::to_string(num_table + 1), "12");
    }
}

void PCclub::exit_print() {

    while (!clients.empty()) {
        std::string key = clients.begin()->first;
        ID_4(closing_time, key, "11");
    }
    std::cout << closing_time << '\n';
    std::string hours;
    std::string mins;
    for (int i = 0; i < number_tables; i++) {

        int minut = time_tables[i].first;
        int hour = minut / 60;
        int min = minut % 60;
        if (hour / 10 != 0) {
            hours = std::to_string(hour / 10) + std::to_string(hour % 10);
        }else {
            hours = "0" + std::to_string(hour % 10);
        }
        if (min / 10 != 0) {
            mins = std::to_string(min / 10) + std::to_string(min % 10);
        }
        else {
            mins = "0" + std::to_string(min % 10);
        }
        std::cout << i + 1 << " " << busy_tables[i].first << " " << hours << ":" << mins << '\n';
    }
    std::exit(EXIT_FAILURE);
}
} // namespace pcclub::core