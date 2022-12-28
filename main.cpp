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
    array_t arr1 { true, false, true, false, true };
    json jarr1(arr1);

    array_t arr2 { 1, 2, 3 };
    json jarr2(arr2);

    array_t arr3 { jarr1, jarr2, j1, j2, j3};
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

    // Accessors
    std::cout << "Accessors" << std::endl;
    std::cout << "d1[\"test\"] = " << d1["test"] << std::endl;

    std::cout << arr2[0] << std::endl;
    std::cout << jarr2[0] << std::endl;
    std::cout << d2["test"] << std::endl;
    std::cout << jd2["test"] << std::endl;

    jd2["test"] = 1539834;
    std::cout << jd2["test"] << std::endl;

    // Other
    dict_t my_json;
    my_json["test!"] = 5;
    std::cout << my_json << std::endl;
    dict_value my_json_value(my_json);
    std::cout << my_json_value << std::endl;
    json my_json_object(my_json);

    my_json["test!"] = 128382812;

    for (const auto& [k, v] : my_json)
    {
        std::cout << k << " = " << v << std::endl;
    }

    for (const auto& [k, v] : my_json_object.get_dict())
    {
        std::cout << k << " = " << v << std::endl;
    }

    return 0;
}