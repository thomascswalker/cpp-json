#include <iostream>
#include "parser.h"

int main()
{
    auto j = JSON::JsonObject(true);
    std::cout << j << std::endl;
    return 0;
}