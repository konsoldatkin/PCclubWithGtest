#pragma

#include <ctime>
#include <istream>

namespace PC_club::core {

struct TmParseResult {
  std::tm time;
  bool fail;
};

TmParseResult readTime(std::istream &input);

void writeTime(std::ostream &output, std::tm time);

} // namespace PC_club::core
