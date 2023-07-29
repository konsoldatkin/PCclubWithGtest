#include "club.hpp"
#include "utils.hpp"

#include <cctype>
#include <cmath>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace PC_club::core {
    namespace {
        bool valid_time_form(const std::string& timeStr) {
            std::regex pattern("^([0-1][0-9]|2[0-3]):([0-5][0-9])$");
            return std::regex_match(timeStr, pattern);
        }
        bool non_space_char(const std::string& str) {
            return std::find_if(str.begin(), str.end(), [](char c) {
                return !std::isspace(static_cast<unsigned char>(c));
                }) != str.end();
        }
        void rm_str_from_list(std::list<std::string>& myList,
            const std::string& target) {

            for (auto it = myList.begin(); it != myList.end();) {

                if (*it == target) {
                    it = myList.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        bool is_element_in_list(const std::list<std::string>& myList,
            const std::string& target) {
            for (const auto& element : myList) {
                if (element == target) {
                    return true;
                }
            }
            return false;
        }

        bool compare_time(const std::tm& time1, const std::tm& time2) {
            if (time1.tm_hour < time2.tm_hour) {
                return true;
            }
            else if (time1.tm_hour > time2.tm_hour) {
                return false;
            }
            else {
                return time1.tm_min <= time2.tm_min;
            }
        }

        void calc_of_time(const std::tm& time, TableInfo& tables, const int& price) {
            int difference = (time.tm_hour * 60 + time.tm_min) - (tables.current_occupation_time.tm_hour * 60 + tables.current_occupation_time.tm_min);
            tables.occupied_minutes += difference;
            tables.earned += std::ceil(double(difference) / 60.0) * price;
            tables.occupied = 0;
        }

    } // namespace

    // погугли что такое enum и enum class


    int readInteger(std::istream& input) {
        std::string line;
        if (std::getline(input, line).fail()) {
            throw std::invalid_argument(line);
        }
        std::size_t pos;
        int result = std::stoi(line, &pos);
        if (pos != line.size() || result == 0) {
            throw std::invalid_argument(line);
            //std::exit(EXIT_FAILURE);
        }
        return result;
    }

    WorkingHours readWorkingHours(std::istream& input) {
        std::string line;
        if (std::getline(input, line).fail()) {
            throw std::invalid_argument{line};
        }
        auto line_stream = std::stringstream{ line };
        auto openTimeResult = readTime(line_stream);
        if (openTimeResult.fail) {
            throw std::invalid_argument{line};
        }

        //line_stream = std::stringstream{ line };
        auto closeTimeResult = readTime(line_stream);
        if (closeTimeResult.fail) {
            throw std::invalid_argument{line};
        }

        return { .open = openTimeResult.time, .close = closeTimeResult.time };
    }

    std::vector<Event> readEvents(std::ifstream& input) {

        std::vector<Event> events;
        while (!input.eof()) {
            std::string line;
            if (std::getline(input, line).fail()) {
                // TODO: look at me
                throw std::invalid_argument{""};
            }
            try {

                events.push_back(Event{ line });
            }
            catch (...) {
                throw std::invalid_argument{line};
            }
        }
        return events;
    }

    PCclub::PCclub(std::string& file_name) {
        std::ifstream input;
        input.open(file_name);
        if (!input.is_open())
            throw std::logic_error("");

        number_tables = readInteger(input);
        working_hours = readWorkingHours(input);

        price = readInteger(input);

        for (int i = 0; i < number_tables; i++) {
            tables.push_back(TableInfo{
                .current_occupation_time = std::tm{.tm_min = 0, .tm_hour = 0},
                .occupied_minutes = 0,
                .earned = 0,
                .occupied = false,
                });
        }

        std::string sequence_in_time = "00:00";
        std::vector<Event> events = readEvents(input);
        // write opening hour
        writeTime(std::cout, working_hours.open);
        std::cout << '\n';
        for (const Event& event : events) {
            handleEvent(event);
        }

        exit_print();
    }

    void PCclub::handleEvent(const Event& event) {

        switch (event.type) {
        case EventTypeClientCame:
            ID_1(event);
            break;
        case EventTypeTakeSeat:
            ID_2(event);
            break;
        case EventTypeWaiting:
            ID_3(event);
            break;
        case EventTypeLeft:
          ID_4(event);
            break;
        }
    }

    bool EqualTimes(const std::tm& a, const std::tm& b) {
        return a.tm_hour == b.tm_hour && a.tm_min == b.tm_min;
    }

    void PCclub::ID_1(const Event& event) {
        writeTime(std::cout, event.time);
        std::cout << ' ' << event.type << ' ' << event.client_name<<'\n';

        if (compare_time(event.time, working_hours.open) &&
            !EqualTimes(event.time, working_hours.open)) {
            writeTime(std::cout, event.time);
            std::cout << ' ' << OutgoingEventTypeError << ' ' << "NotOpenYet" << '\n';
            return;
        }
        if (clients.count(event.client_name)) {
            writeTime(std::cout, event.time);
            std::cout << ' ' << OutgoingEventTypeError << ' ' << "YouShallNotPass" << '\n';
            return;
        }
        clients.insert({ event.client_name, -1 });
    }

    void PCclub::ID_2(const Event& event, std::string id) {
        writeTime(std::cout, event.time);
        std::cout << ' ' << id << ' ' << event.client_name << " " << event.table_number << '\n';

        if (clients.count(event.client_name) == 0) {
            writeTime(std::cout, event.time);
            std::cout << ' ' << OutgoingEventTypeError << ' ' << "ClientUnknown" << '\n';
            return;
        }
        if (tables[event.table_number - 1].occupied) {
            writeTime(std::cout, event.time);
            std::cout << ' ' << OutgoingEventTypeError << ' ' << "PlaceIsBusy" << '\n';
            return;
        }
        if (clients[event.client_name] != -1) {
            int num_table = clients[event.client_name];

            calc_of_time(event.time , tables[num_table], price);
            tables[event.table_number - 1].current_occupation_time = event.time;
            tables[event.table_number - 1].occupied = 1;
            clients[event.client_name] = event.table_number - 1;

        }
        else {
            tables[event.table_number - 1].current_occupation_time = event.time;
            tables[event.table_number - 1].occupied = 1;
            clients[event.client_name] = event.table_number - 1;
            rm_str_from_list(queue, event.client_name);
        }
    }

    void PCclub::ID_3(const Event& event) {
        if (!clients.count(event.client_name)) {
            writeTime(std::cout, event.time);
            std::cout << ' ' << OutgoingEventTypeError << ' ' << "ClientUnknown" << '\n';
            return;
        }
        if (queue.size() < number_tables) {
            writeTime(std::cout, event.time);
            std::cout << ' ' << event.type << ' ' << event.client_name << '\n';
            if (!is_element_in_list(queue, event.client_name)) {
                queue.push_back(event.client_name);
            }
            for (int i = 0; i < number_tables; i++) {
                if (tables[i].occupied == 0) {
                    writeTime(std::cout, event.time);
                    std::cout << ' ' << OutgoingEventTypeError << ' ' << "ICanWaitNoLonger!" << '\n';
                    break;
                }
            }
        }
        else {
            rm_str_from_list(queue, event.client_name);
            ID_4(event, "11");
        }

    }

    void PCclub::ID_4(const Event& event, std::string id) {
        if (!clients.count(event.client_name)) {
          writeTime(std::cout, event.time);
          std::cout << ' ' << OutgoingEventTypeError << ' ' << "ClientUnknown" << '\n';
          return;
        }
        if (clients[event.client_name] == -1 && id == "11") {
          clients.erase(event.client_name);
          writeTime(std::cout, event.time);
          std::cout << ' ' << id << ' ' << event.client_name << '\n';
          return;
        }
        if (clients[event.client_name] == -1) {
          clients.erase(event.client_name);
          rm_str_from_list(queue, event.client_name);
          writeTime(std::cout, event.time);
          std::cout << ' ' << event.type << ' ' << event.client_name << '\n';
          return;
        }
        writeTime(std::cout, event.time);
        std::cout << ' ' << id << ' ' << event.client_name << '\n';
        int num_table = clients[event.client_name];
        calc_of_time(event.time, tables[num_table], price);
        clients.erase(event.client_name);
        if (!queue.empty() && !EqualTimes(event.time, working_hours.close)) {

          std::string name_first_from_queue = queue.front();


          std::ostringstream oss;
          writeTime(oss, event.time);
          oss << ' ' << 2 << ' ' << queue.front()<< ' ' << num_table + 1;
          auto str = oss.str();
          Event first_from_queue(str);


          queue.pop_front();

          ID_2(first_from_queue, "12");
        }
    }

    void PCclub::exit_print() {

        std::ostringstream oss;

        while (!clients.empty()) {
          std::string key = clients.begin()->first;
          writeTime(oss, working_hours.close);
          oss << ' ' << 4 << ' ' << key;
          auto str = oss.str();
          Event closing_event(str);

          ID_4(closing_event, "11");
        }
        writeTime(std::cout, working_hours.close);
        std::cout << '\n';
        std::string hours;
        std::string mins;
        for (int i = 0; i < number_tables; i++) {

          int hour = tables[i].occupied_minutes / 60;
          int min = tables[i].occupied_minutes % 60;
          if (hour / 10 != 0) {
            hours = std::to_string(hour / 10) + std::to_string(hour % 10);
          } else {
            hours = "0" + std::to_string(hour % 10);
          }
          if (min / 10 != 0) {
            mins = std::to_string(min / 10) + std::to_string(min % 10);
          } else {
            mins = "0" + std::to_string(min % 10);
          }
          std::cout << i + 1 << " " << tables[i].earned << " " << hours << ":"
                    << mins << '\n';
        }
        std::exit(EXIT_SUCCESS);
    }

} // namespace PC_club::core
