#include "json.h"

JSON_NAMESPACE_OPEN

std::string get_indent(int indent)
{
    return std::string(" ", 0, indent * 4);
}

// Null
std::string null_value::format(int indent = 0)
{
    return get_indent(indent) + "Null";
}

// Bool
bool bool_value::value()
{
    return m_value;
}

std::string bool_value::format(int indent = 0)
{
    std::string bool_string(m_value == true ? std::string("true") : std::string("false"));
    return get_indent(indent) + bool_string;
}

// Int
int int_value::value()
{
    return m_value;
}

std::string int_value::format(int indent = 0)
{
    return get_indent(indent) + std::to_string(m_value);
}

// Double
double double_value::value()
{
    return m_value;
}

std::string double_value::format(int indent = 0)
{
    return get_indent(indent) + std::to_string(m_value);
}

// String
std::string string_value::value()
{
    return m_value;
}

std::string string_value::format(int indent = 0)
{
    return get_indent(indent) + m_value;
}

// Array
array_value::array_value(const array_t value)
{
    m_value.clear();
    for (const json& v : value)
    {
        m_value.emplace_back(v);
    }
};

array_t array_value::value()
{
    return m_value;
}

std::string array_value::format(int indent = 0)
{
    std::string arrayString = get_indent(indent) + "[";
    int count = 0;
    for (auto& v : m_value)
    {
        arrayString += v.format(indent);
        count++;

        if (count != m_value.size())
        {
            arrayString += ",";
        }
        else
        {
            arrayString += "]";
        }
    }
    return arrayString;
}

// Dictionary
dict_value::dict_value(const dict_t value)
{
    for (const auto& [k, v] : value)
    {
        m_value[k] = v;
    }
};

dict_t dict_value::value()
{
    return m_value;
}

std::string dict_value::format(int indent = 0)
{
    std::string dictString = get_indent(indent) + "{";
    int count = 0;
    for (auto& [k, v] : m_value)
    {
        dictString += k + ": " + v.format(indent);
        count++;

        if (count != m_value.size())
        {
            dictString += ",";
        }
        else
        {
            dictString += "}";
        }
    }
    return dictString;
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
json::json(const array_t& value)
{

    m_value = std::make_unique<array_value>(value);
    m_type = Array;
}
json::json(const dict_t& value)
{

    m_value = std::make_unique<dict_value>(value);
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

array_t json::get_array() const
{
    return static_cast<array_value*>(m_value.get())->value();
}

dict_t json::get_dict() const
{
    return static_cast<dict_value*>(m_value.get())->value();
}

std::string json::format(int indent = 0)
{
    if (m_value.get() != nullptr)
    {
        return m_value.get()->format(indent);
    }

    return "NULL";
}

JSON_NAMESPACE_CLOSE