#include "Message.hpp"

#include <ctime>

namespace EngineG::Network{

void Message::SetTimeNow() {
  ctime(&mTime);
}

}
