#include "json.h"
using namespace JSON;

int main()
{
    JsonObject j = loadFile("file.json");
    std::cout << j << std::endl;
    return 0;
}