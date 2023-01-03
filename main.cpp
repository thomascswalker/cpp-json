#include "json.h"
using namespace JSON;

int main()
{
    JsonObject simpleJson = loadFile("simple.json");
    std::cout << "Simple JSON:\n" << simpleJson << std::endl << std::endl;

    JsonObject complexJson = loadFile("complex.json");
    std::cout << "Complex JSON:\n" << complexJson << std::endl << std::endl;

    JsonObject largeJson = loadFile("large.json");
    std::cout << "Large JSON:\n" << largeJson << std::endl << std::endl;

    JsonObject stringJson = loadString("{\"fruits\":[\"apple\",\"orange\",\"pear\"]}");
    std::cout << "String JSON:\n" << stringJson << std::endl << std::endl;

    return 0;
}