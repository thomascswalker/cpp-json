#include "json.hpp"

int main()
{
    JSON::JsonObject simpleJson = JSON::loadFile("simple.json");
    std::cout << "Simple JSON:\n" << simpleJson << std::endl << std::endl;

    JSON::JsonObject complexJson = JSON::loadFile("complex.json");
    std::cout << "Complex JSON:\n" << complexJson << std::endl << std::endl;

    JSON::JsonObject largeJson = JSON::loadFile("large.json");
    std::cout << "Large JSON:\n" << largeJson << std::endl << std::endl;

    std::string string("{\"fruits\":[\"apple\",\"orange\",\"pear\"]}");
    JSON::JsonObject stringJson = JSON::loadString(string);
    std::cout << "Raw String: " << string << std::endl;
    std::cout << "String JSON:\n" << stringJson << std::endl << std::endl;

    return 0;
}