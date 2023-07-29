#include "utils.hpp"

#include <iomanip>
#include <iostream>

namespace PC_club::core {

namespace {

const auto TimeFormat = "%H:%M";

}

TmParseResult readTime(std::istream &input) {
  TmParseResult result;
  input >> std::get_time(&result.time, "%H:%M");
  result.fail = input.fail();
  return result;
}

void writeTime(std::ostream &output, std::tm time) {
  output << std::put_time(&time, TimeFormat);
}

} // namespace PC_club::core
