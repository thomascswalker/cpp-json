#include <format>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "json.h"

int main()
{
    JsonObject j1 = JsonObject(false);
    std::cout << j1 << std::endl;

    JsonObject j2 = JsonObject(15382);
    std::cout << j2 << std::endl;

    JsonObject j3 = JsonObject(3.14159);
    std::cout << j3 << std::endl;

    std::string string = std::string("This is a sentence.");
    JsonObject j4 = JsonObject(string);
    std::cout << j4 << std::endl;

    //std::vector<JsonObject> arr1;
    //arr1.emplace_back(true);
    //arr1.emplace_back(false);
    //JsonObject jarr1(arr1);
    //std::cout << jarr1 << std::endl;

    //std::vector<double> arr2{ 3.14159, 0.00001, 99.9999 };
    //JsonObject jarr2(arr2);
    //std::cout << jarr2 << std::endl;

    return 0;
}