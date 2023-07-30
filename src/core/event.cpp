#include "event.hpp"

#include "utils.hpp"
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <regex>

namespace PC_club::core {

Event::Event(const std::string &line) {
  std::stringstream input(line);
  auto parseTime = readTime(input);
  if (parseTime.fail) {
      throw std::invalid_argument{line};
  }
  time = parseTime.time;

  input >> type;
  if (input.fail()) {
      throw std::invalid_argument{line};
  }
  std::vector<int> allowedValues{1, 2, 3, 4};
  if (std::find(allowedValues.begin(), allowedValues.end(), type) == allowedValues.end()){
      throw std::invalid_argument{line};
  }

  input >> client_name;
  if (input.fail()) {
      throw std::invalid_argument{line};
  }
  std::regex pattern("[a-z0-9_-]+");
  if (!std::regex_match(client_name, pattern)) {
      throw std::invalid_argument{line};
  }

    
  if (type == EventTypeTakeSeat) {
      input >> table_number;
      if (input.fail()) {
          throw std::invalid_argument{line};
      }
  }
}

} // namespace PC_club::core
