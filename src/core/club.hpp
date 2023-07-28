#pragma once

#include <string>
#include <map>
#include <vector>
#include <list>

namespace PC_club::core
{
	
class PCclub
{
  public:
	PCclub(std::string &file_name);
	void query_proces(std::string str);
	void ID_1(std::string time, std::string name);
	void ID_2(std::string time, std::string name, std::string num, std::string id = "2");
	void ID_3(std::string time, std::string name);
	void ID_4(std::string time, std::string name, std::string id = "4");
	void exit_print();

  private:
	int number_tables;
	std::string opening_time;
	std::string closing_time;
	int price;

	std::vector<std::pair<int, std::string>> time_tables;
	std::vector<std::pair<int, bool>> busy_tables;

	std::map<std::string, int> clients;
	std::list<std::string> queue;


};

} // namespace csv_reader::core
