#include <format>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "json.h"

JSON_NAMESPACE_USING

int main()
{
    // Boolean
    json_t j1 = json_t(false);
    std::cout << "Boolean" << std::endl;
    std::cout << j1 << std::endl << std::endl;

    // Integer
    json_t j2 = json_t(15382);
    std::cout << "Integer" << std::endl;
    std::cout << j2 << std::endl << std::endl;

    // Double
    json_t j3 = json_t(3.14159);
    std::cout << "Double" << std::endl;
    std::cout << j3 << std::endl << std::endl;

    // String
    std::string string = std::string("this is a sentence.");
    json_t j4 = json_t(string);
    std::cout << "String" << std::endl;
    std::cout << j4 << std::endl << std::endl;

    // Array
    array_t arr1;
    arr1.emplace_back(true);
    arr1.emplace_back(true);
    arr1.emplace_back(false);
    json jarr1(arr1);

    array_t arr2;
    arr2.emplace_back(1);
    arr2.emplace_back(2);
    arr2.emplace_back(std::string("three"));
    json jarr2(arr2);

    array_t arr3;
    arr3.emplace_back(jarr1);
    arr3.emplace_back(jarr2);
    json jarr3(arr3);

    std::cout << "Array" << std::endl;
    std::cout << jarr3 << std::endl << std::endl;

    // Dictionary
    dict_t d1;
    d1["test"] = 5;
    d1["test2"] = jarr3;
    d1["test3"] = 15;
    json jd1(d1);

    dict_t d2;
    d2["test"] = 5;
    d2["test2"] = 10;
    d2["test3"] = jd1;
    json jd2(d2);

    std::cout << "Dictionary" << std::endl;
    std::cout << jd2 << std::endl << std::endl;

    return 0;
}