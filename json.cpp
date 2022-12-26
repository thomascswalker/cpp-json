#include "json.h"

// Null
std::string NullValue::format()
{
    return "Null";
}

// Bool
bool BoolValue::value()
{
    return m_value;
}

std::string BoolValue::format()
{
    return (m_value == true ? std::string("true") : std::string("false"));
}

// Int
int IntValue::value()
{
    return m_value;
}

std::string IntValue::format()
{
    return std::to_string(m_value);
}

// Double
double DoubleValue::value()
{
    return m_value;
}

std::string DoubleValue::format()
{
    return std::to_string(m_value);
}

// String
std::string StringValue::value()
{
    return m_value;
}

std::string StringValue::format()
{
    return m_value;
}

// Array
ArrayValue::ArrayValue(const std::vector<JsonObject>& value)
{
    for (const JsonObject& v : value)
    {
        m_value.push_back(&v);
    }
};

std::vector<JsonObject> ArrayValue::value()
{
    return m_value;
}

std::string ArrayValue::format()
{
    std::string arrayString = "[]";
    return arrayString;
}

// JSON Object
JsonObject::JsonObject()
{
    m_value = nullptr;
    m_type = Null;
}
JsonObject::JsonObject(bool value)
{
    m_value = std::make_unique<BoolValue>(value);
    m_type = Bool;
}
JsonObject::JsonObject(int value)
{
    m_value = std::make_unique<IntValue>(value);
    m_type = Int;
}
JsonObject::JsonObject(double value)
{
    m_value = std::make_unique<DoubleValue>(value);
    m_type = Double;
}
JsonObject::JsonObject(const std::string& value)
{
    m_value = std::make_unique<StringValue>(value);
    m_type = String;
}
JsonObject::JsonObject(const std::vector<JsonObject>& value)
{
    m_value = std::make_unique<ArrayValue>(value);
    m_type = Array;
}

bool JsonObject::getBool()
{
    return static_cast<BoolValue*>(m_value.get())->value();
}

int JsonObject::getInt()
{
    return static_cast<IntValue*>(m_value.get())->value();
}

double JsonObject::getDouble()
{
    return static_cast<DoubleValue*>(m_value.get())->value();
}

std::string JsonObject::getString()
{
    return static_cast<StringValue*>(m_value.get())->value();
}

std::string JsonObject::format()
{
    return m_value.get()->format();
}