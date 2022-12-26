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
    std::string format();
};

class BoolValue
    : public Value
{
    bool m_value;
public:
    BoolValue(bool value)
        : m_value(value) { };
    bool value();
    std::string format();
};

class IntValue
    : public Value
{
    int m_value;
public:
    IntValue(int value)
        : m_value(value) { };
    int value();
    std::string format();
};

class DoubleValue
    : public Value
{
    double m_value;
public:
    DoubleValue(double value)
        : m_value(value) { };
    double value();
    std::string format();
};

class StringValue
    : public Value
{
    std::string m_value;
public:
    StringValue(std::string value)
        : m_value(value) { };
    std::string value();
    std::string format();
};

class ArrayValue
    : public Value
{
    std::vector<JsonObject> m_value;
public:
    ArrayValue(const std::vector<JsonObject>& value);
    std::vector<JsonObject> value();
    std::string format();
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
    JsonObject();
    JsonObject(bool value);
    JsonObject(int value);
    JsonObject(double value);
    JsonObject(const std::string& value);
    JsonObject(const std::vector<JsonObject>& value);
    ~JsonObject() { };

    // Methods
    ValueType type() { return m_type; }

    bool getBool();
    int getInt();
    double getDouble();
    std::string getString();
    std::string format();

    // Operators
    friend std::ostream& operator << (std::ostream& o, JsonObject& v);
};

inline std::ostream& operator << (std::ostream& o, JsonObject& j)
{
    return o << j.format();
}

#endif