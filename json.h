#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

// Forward declaration
class JsonObject;

template<typename Base, typename T>
inline bool isInstance(const T obj)
{
    auto ptr = &obj;
    return static_cast<const Base*>(ptr) != nullptr;
}

// Values
enum ValueType
{
    Null,           // nullptr
    Bool,           // true, false
    Int,            // 1, 2, 3
    Double,         // 3.14, 7.62, 50.50
    String,         // "This is a string."
    Array,          // { 1, 2, 3, 4, 5 }
    Dictionary      // { {"Key 1", 5}, {"Key 2", 10} }
};

/// <summary>
/// Base class for all JSON value types.
/// </summary>
class Value
{
public:
    /// <summary>
    /// Format the current value to a string.
    /// </summary>
    /// <returns>The string-formatted value.</returns>
    virtual std::string format() = 0;
};

class NullValue
    : public Value
{
    void* m_value = nullptr;
public:
    NullValue() { };
    std::string format()
    {
        return "Null";
    }
};

class BoolValue
    : public Value
{
    bool m_value;
public:
    BoolValue(bool value)
        : m_value(value) { };
    bool value()
    {
        return m_value;
    }

    std::string format()
    {
        return (m_value == true ? std::string("true") : std::string("false"));
    }
};

class IntValue
    : public Value
{
    int m_value;
public:
    IntValue(int value)
        : m_value(value) { };
    int value()
    {
        return m_value;
    }

    std::string format()
    {
        return std::to_string(m_value);
    }
};

class DoubleValue
    : public Value
{
    double m_value;
public:
    DoubleValue(double value)
        : m_value(value) { };
    double value()
    {
        return m_value;
    }

    std::string format()
    {
        return std::to_string(m_value);
    }
};

class StringValue
    : public Value
{
    std::string m_value;
public:
    StringValue(std::string value)
        : m_value(value) { };
    std::string value()
    {
        return m_value;
    }

    std::string format()
    {
        return m_value;
    }
};

class ArrayValue
    : public Value
{
    std::vector<JsonObject> m_value;
public:
    ArrayValue(const std::vector<JsonObject> value)
    {
        for (int i = 0; i < value.size(); i++)
        {
            m_value.push_back(value[i]);
        }
    };

    std::vector<JsonObject> value()
    {
        return m_value;
    }

    std::string format()
    {
        std::string arrayString = "[]";
        return arrayString;
    }
};

/// <summary>
/// Base JSON object. Contains a wrapper for each possible value type, with constructors and accessors for each.
/// </summary>
class JsonObject
{
    std::unique_ptr<Value> m_value;
    ValueType m_type;

public:
    // Constructors
    JsonObject()
    {
        m_value = nullptr;
        m_type = Null;
    }
    JsonObject(bool value)
    {
        m_value = std::make_unique<BoolValue>(value);
        m_type = Bool;
    }
    JsonObject(int value)
    {
        m_value = std::make_unique<IntValue>(value);
        m_type = Int;
    }
    JsonObject(double value)
    {
        m_value = std::make_unique<DoubleValue>(value);
        m_type = Double;
    }
    JsonObject(const std::string& value)
    {
        m_value = std::make_unique<StringValue>(value);
        m_type = String;
    }
    JsonObject(const std::vector<JsonObject>& value)
    {
        m_value = std::make_unique<ArrayValue>(value);
        m_type = Array;
    }
    ~JsonObject() { };

    // Methods
    ValueType type() { return m_type; }

    bool getBool()
    {
        return static_cast<BoolValue*>(m_value.get())->value();
    }

    int getInt()
    {
        return static_cast<IntValue*>(m_value.get())->value();
    }

    double getDouble()
    {
        return static_cast<DoubleValue*>(m_value.get())->value();
    }

    std::string getString()
    {
        return static_cast<StringValue*>(m_value.get())->value();
    }

    std::string format()
    {
        return m_value.get()->format();
    }

    // Operators
    friend std::ostream& operator << (std::ostream& o, JsonObject& v);
};

inline std::ostream& operator << (std::ostream& o, JsonObject& j)
{
    return o << j.format();
}

#endif