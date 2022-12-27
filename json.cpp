#include "json.h"

// Null
std::string null_value::format()
{
    return "Null";
}

// Bool
bool bool_value::value()
{
    return m_value;
}

std::string bool_value::format()
{
    return (m_value == true ? std::string("true") : std::string("false"));
}

// Int
int int_value::value()
{
    return m_value;
}

std::string int_value::format()
{
    return std::to_string(m_value);
}

// Double
double double_value::value()
{
    return m_value;
}

std::string double_value::format()
{
    return std::to_string(m_value);
}

// String
std::string string_value::value()
{
    return m_value;
}

std::string string_value::format()
{
    return m_value;
}

// Array
array_value::array_value(const std::vector<json> value)
{
    m_value.clear();
    for (const json& v : value)
    {
        m_value.emplace_back(v);
    }
};

std::vector<json> array_value::value()
{
    return m_value;
}

std::string array_value::format()
{
    std::string arrayString = "[\n";
    for (auto& v : m_value)
    {
        arrayString += v.format() + ",";
    }
    arrayString += "\n]";
    return arrayString;
}

// JSON Object
json::json()
{
    m_value = nullptr;
    m_type = Null;
}
json::json(const json& other)
{
    *this = other;
}
json::json(bool value)
{
    m_value = std::make_unique<bool_value>(value);
    m_type = Bool;
}
json::json(int value)
{
    m_value = std::make_unique<int_value>(value);
    m_type = Int;
}
json::json(double value)
{
    m_value = std::make_unique<double_value>(value);
    m_type = Double;
}
json::json(const std::string& value)
{
    m_value = std::make_unique<string_value>(value);
    m_type = String;
}
json::json(const std::vector<json>& value)
{

    m_value = std::make_unique<array_value>(value);
    m_type = Array;
}

bool json::get_bool() const
{
    return static_cast<bool_value*>(m_value.get())->value();
}

int json::get_int() const
{
    return static_cast<int_value*>(m_value.get())->value();
}

double json::get_double() const
{
    return static_cast<double_value*>(m_value.get())->value();
}

std::string json::get_string() const
{
    return static_cast<string_value*>(m_value.get())->value();
}

std::vector<json> json::get_array() const
{
    return static_cast<array_value*>(m_value.get())->value();
}

std::string json::format()
{
    if (m_value.get() != nullptr)
    {
        return m_value.get()->format();
    }

    return "NULL";
}