

#include <ctime>
#include <string>

namespace PC_club::core {

const int EventTypeClientCame = 1;
const int EventTypeTakeSeat = 2;
const int EventTypeWaiting = 3;
const int EventTypeLeft = 4;

const int OutgoingEventTypeLeft = 11;
const int OutgoingEventTypeTakeSeat = 12;
const int OutgoingEventTypeError = 13;

struct Event {
  
  Event(const std::string &line);

  std::tm time;
  int type;
  std::string client_name;
  int table_number;
};

} // namespace PC_club::core
