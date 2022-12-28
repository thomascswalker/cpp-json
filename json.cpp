#include "json.h"

JSON_NAMESPACE_OPEN

std::string get_indent(int indent = 0)
{
    auto size = indent * 4;
    return std::string(size, ' ');
}

std::string format_line(const std::string& value, int indent, bool end)
{
    std::string line;
    line += get_indent(indent);
    line += value;
    if (!end)
    {
        line += ",";
    }
    line += "\n";
    return line;
}

std::string format_dict(const std::string& key, const std::string& value, int indent, bool end)
{
    std::string line;
    line += get_indent(indent);
    line += "\"" + key + "\"" + ": " + value;
    if (!end)
    {
        line += ",";
    }
    line += "\n";
    return line;
}

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
    std::string bool_string(m_value == true ? std::string("true") : std::string("false"));
    return bool_string;
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
    return "\"" + m_value + "\"";
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

std::string array_value::format()
{
    std::string array_string = "[\n";
    CURRENT_INDENT++;
    int count = 1;
    for (auto& v : m_value)
    {
        bool at_end = (count == m_value.size());
        array_string += format_line(v.format(), CURRENT_INDENT, at_end);
        count++;
    }
    CURRENT_INDENT--;
    array_string += get_indent(CURRENT_INDENT) + "]";
    return array_string;
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

std::string dict_value::format()
{
    std::string dict_string = "{\n";
    CURRENT_INDENT++;
    int count = 1;
    for (auto& [k, v] : m_value)
    {
        bool at_end = (count == m_value.size());
        std::string new_line = (v.type() == Dictionary || v.type() == Array) ? ("\n" + get_indent(CURRENT_INDENT)) : "";
        dict_string += format_dict(k, new_line + v.format(), CURRENT_INDENT, at_end);
        count++;
    }
    CURRENT_INDENT--;
    dict_string += get_indent(CURRENT_INDENT) + "}";
    return dict_string;
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
    m_type = Dictionary;
}

bool json::get_bool() const
{
    return as_bool().value();
}

int json::get_int() const
{
    return as_int().value();
}

double json::get_double() const
{
    return as_double().value();
}

std::string json::get_string() const
{
    return as_string().value();
}

array_t json::get_array() const
{
    return as_array().value();
}

dict_t json::get_dict() const
{
    return as_dict().value();
}

std::string json::format() const
{
    if (m_value.get() == nullptr)
    {
        return "NULL";
    }

    return m_value.get()->format();
}

bool_value& json::as_bool() const
{
    return *static_cast<bool_value*>(m_value.get());
}

int_value& json::as_int() const
{
    return *static_cast<int_value*>(m_value.get());
}

double_value& json::as_double() const
{
    return *static_cast<double_value*>(m_value.get());
}

string_value& json::as_string() const
{
    return *static_cast<string_value*>(m_value.get());
}

array_value& json::as_array() const
{
    return *static_cast<array_value*>(m_value.get());
}

dict_value& json::as_dict() const
{
    return *static_cast<dict_value*>(m_value.get());
}

const json& json::operator = (const json& other)
{
    switch (other.m_type)
    {
        case (Bool):
        {
            m_value = std::make_unique<bool_value>(other.get_bool());
            break;
        }
        case (Int):
        {
            m_value = std::make_unique<int_value>(other.get_int());
            break;
        }
        case (Double):
        {
            m_value = std::make_unique<double_value>(other.get_double());
            break;
        }
        case (String):
        {
            m_value = std::make_unique<string_value>(other.get_string());
            break;
        }
        case (Array):
        {
            m_value = std::make_unique<array_value>(other.get_array());
            break;
        }
        case (Dictionary):
        {
            m_value = std::make_unique<dict_value>(other.get_dict());
            break;
        }
    }
    m_type = other.m_type;
    return *this;
}

json& json::operator[](const std::string& key)
{
    if (m_type != Dictionary)
    {
        throw std::runtime_error("Invalid type, wanted Dictionary");
    }
    return as_dict().value()[key];
}

json& json::operator[](int index)
{
    std::cout << m_type << std::endl;
    if (m_type != Array)
    {
        throw std::runtime_error("Invalid type, wanted Array");
    }
    return as_array().value()[index];
}

std::ostream& operator << (std::ostream& o, json& j)
{
    switch (j.type())
    {
        case (Bool):
        {
            return o << j.as_bool().format();
        }
        case (Int):
        {
            return o << j.as_int().format();
        }
        case (Double):
        {
            return o << j.as_double().format();
        }
        case (String):
        {
            return o << j.as_string().format();
        }
        case (Array):
        {
            return o << j.as_array().format();
        }
        case (Dictionary):
        {
            return o << j.as_dict().format();
        }
        default:
        {
            return o << "UNKNOWN TYPE: " << j.type();
        }
    }
}


JSON_NAMESPACE_CLOSE