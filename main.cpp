#include "src/json.h"

using namespace JSON;

int main()
{
    std::string filename("..\\examples\\gltf.json");
    JsonObject jsonFile = loadFile(filename);
    std::cout << jsonFile << std::endl;

    std::string string("{\"test\":5}");
    JsonObject jsonString = loadString(string);
    std::cout << jsonString << std::endl;
    return 0;
}
