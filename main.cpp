#include <format>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "json.h"

int main()
{
    json_t j1 = json_t(false);
    std::cout << j1 << std::endl;

    json_t j2 = json_t(15382);
    std::cout << j2 << std::endl;

    json_t j3 = json_t(3.14159);
    std::cout << j3 << std::endl;

    std::string string = std::string("this is a sentence.");
    json_t j4 = json_t(string);
    std::cout << j4 << std::endl;

    std::vector<json> arr1;
    arr1.emplace_back(false);
    arr1.emplace_back(false);
    json jarr1(arr1);
    std::cout << jarr1 << std::endl;

    //std::vector<double> arr2{ 3.14159, 0.00001, 99.9999 };
    //json jarr2(arr2);
    //std::cout << jarr2 << std::endl;

    return 0;
}