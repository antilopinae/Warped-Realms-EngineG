#include <cereal/archives/json.hpp>

#include <sstream>

std::string serializeSomething() {
    std::ostringstream os;
    cereal::JSONOutputArchive archive(os);
    int x = 42;
    archive(x);
    return os.str();
}
