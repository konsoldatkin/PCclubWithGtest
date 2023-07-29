#include "event.hpp"

#include "utils.hpp"

#include <sstream>
#include <stdexcept>
#include <iostream>

namespace PC_club::core {

Event::Event(const std::string &line) {
  std::stringstream input(line);
  auto parseTime = readTime(input);
  if (parseTime.fail) {
    std::cout << "+++++++++++++++++++++++++++++++++1" << '\n';
    throw std::invalid_argument{line};
  }
  time = parseTime.time;

  input >> type;
  if (input.fail()) {
    std::cout << "+++++++++++++++++++++++++++++++++2" << '\n';
    throw std::invalid_argument{line};
  }
  input >> client_name;
  if (input.fail()) {
    std::cout << "+++++++++++++++++++++++++++++++++3" << '\n';
    throw std::invalid_argument{line};
  }
  if (type == EventTypeTakeSeat) {
    input >> table_number;
    if (input.fail()) {
      std::cout << "+++++++++++++++++++++++++++++++++4" << '\n';
      throw std::invalid_argument{line};
    }
  }
}

} // namespace PC_club::core
