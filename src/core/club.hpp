#pragma once

#include "event.hpp"

#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace PC_club::core {

	struct TableInfo {
		std::tm current_occupation_time;
		int occupied_minutes;
		int earned;
		bool occupied;
	};

	struct WorkingHours {
		std::tm open;
		std::tm close;
	};

	class PCclub {
	public:
		PCclub(std::string& file_name);
		void handleEvent(const Event& event);
		void ID_1(const Event& event);
		void ID_2(const Event& event, std::string id = "2");
		void ID_3(const Event& event);
		void ID_4(const Event& event, std::string id = "4");
		void exit_print();

	private:
		int price;
		WorkingHours working_hours;
		int number_tables;
		std::vector<TableInfo> tables;

		std::map<std::string, int> clients;
		std::list<std::string> queue;
	};

} // namespace PC_club::core
