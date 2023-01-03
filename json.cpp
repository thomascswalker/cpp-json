#include "json.h"

JSON_NAMESPACE_OPEN

std::string getIndent(int indent = 0)
{
    auto size = indent * 4;
    return std::string(size, ' ');
}

std::string formatLine(const std::string& value, int indent, bool end)
{
    std::string line;
    line += getIndent(indent);
    line += value;
    if (!end)
    {
        line += ",";
    }
    line += "\n";
    return line;
}

std::string formatLine(const std::string& key, const std::string& value, int indent, bool end)
{
    std::string line;
    line += getIndent(indent);
    line += "\"" + key + "\"" + ": " + value;
    if (!end)
    {
        line += ",";
    }
    line += "\n";
    return line;
}

// General operators
std::ostream& operator << (std::ostream& o, JsonArray& a)
{
    return o << JsonObject(a).format();
}

std::ostream& operator << (std::ostream& o, JsonDict& d)
{
    return o << JsonObject(d).format();
}

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
    std::string boolString(m_value == true ? std::string("true") : std::string("false"));
#if DEBUG_TYPE == true
    boolString += " (bool)";
#endif
    return boolString;
}

// Int
int IntValue::value()
{
    return m_value;
}

std::string IntValue::format()
{
    std::string intString = std::to_string(m_value);
#if DEBUG_TYPE == true
    intString += " (int)";
#endif
    return intString;
}

// Double
double DoubleValue::value()
{
    return m_value;
}

std::string DoubleValue::format()
{
    std::string doubleString = std::to_string(m_value);
#if DEBUG_TYPE == true
    doubleString += " (double)";
#endif
    return doubleString;
}

// String
std::string StringValue::value()
{
    return m_value;
}

std::string StringValue::format()
{

    std::string string = "\"" + m_value + "\"";
#if DEBUG_TYPE == true
    string += " (string)";
#endif
    return string;
}

// Array
ArrayValue::ArrayValue(const JsonArray value)
{
    m_value.clear();
    for (const JsonObject& v : value)
    {
        m_value.emplace_back(v);
    }
};

JsonArray ArrayValue::value()
{
    return m_value;
}

std::string ArrayValue::format()
{
    std::string arrayString = "[\n";
    CURRENT_INDENT++;
    int count = 1;
    for (auto& v : m_value)
    {
        bool at_end = (count == m_value.size());
        arrayString += formatLine(v.format(), CURRENT_INDENT, at_end);
        count++;
    }
    CURRENT_INDENT--;
    arrayString += getIndent(CURRENT_INDENT) + "]";
    return arrayString;
}

// Dictionary
DictValue::DictValue(const JsonDict value)
{
    for (const auto& [k, v] : value)
    {
        m_value[k] = v;
    }
};

JsonDict DictValue::value()
{
    return m_value;
}

std::string DictValue::format()
{
    std::string dictString = "{\n";
    CURRENT_INDENT++;
    int count = 1;
    for (auto& [k, v] : m_value)
    {
        bool at_end = (count == m_value.size());
        std::string new_line = (v.type() == Dictionary || v.type() == Array) ? ("\n" + getIndent(CURRENT_INDENT)) : "";
        dictString += formatLine(k, new_line + v.format(), CURRENT_INDENT, at_end);
        count++;
    }
    CURRENT_INDENT--;
    dictString += getIndent(CURRENT_INDENT) + "}";
    return dictString;
}

// JSON Object
JsonObject::JsonObject()
{
    m_value = nullptr;
    m_type = Null;
}
JsonObject::JsonObject(const JsonObject& other)
{
    *this = other;
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
JsonObject::JsonObject(const JsonArray& value)
{

    m_value = std::make_unique<ArrayValue>(value);
    m_type = Array;
}
JsonObject::JsonObject(const JsonDict& value)
{
    m_value = std::make_unique<DictValue>(value);
    m_type = Dictionary;
}

BoolValue& JsonObject::asBool() const
{
    return *static_cast<BoolValue*>(m_value.get());
}

IntValue& JsonObject::asInt() const
{
    return *static_cast<IntValue*>(m_value.get());
}

DoubleValue& JsonObject::asDouble() const
{
    return *static_cast<DoubleValue*>(m_value.get());
}

StringValue& JsonObject::asString() const
{
    return *static_cast<StringValue*>(m_value.get());
}

ArrayValue& JsonObject::asArray() const
{
    return *static_cast<ArrayValue*>(m_value.get());
}

DictValue& JsonObject::asDict() const
{
    return *static_cast<DictValue*>(m_value.get());
}

bool JsonObject::getBool() const
{
    return asBool().value();
}

int JsonObject::getInt() const
{
    return asInt().value();
}

double JsonObject::getDouble() const
{
    return asDouble().value();
}

std::string JsonObject::getString() const
{
    return asString().value();
}

JsonArray JsonObject::getArray() const
{
    return asArray().value();
}

JsonDict JsonObject::getDict() const
{
    return asDict().value();
}

std::string JsonObject::format() const
{
    if (m_value.get() == nullptr)
    {
        return "NULL";
    }

    return m_value.get()->format();
}

const JsonObject& JsonObject::operator = (const JsonObject& other)
{
    switch (other.m_type)
    {
        case (Bool):
        {
            m_value = std::make_unique<BoolValue>(other.getBool());
            break;
        }
        case (Int):
        {
            m_value = std::make_unique<IntValue>(other.getInt());
            break;
        }
        case (Double):
        {
            m_value = std::make_unique<DoubleValue>(other.getDouble());
            break;
        }
        case (String):
        {
            m_value = std::make_unique<StringValue>(other.getString());
            break;
        }
        case (Array):
        {
            m_value = std::make_unique<ArrayValue>(other.getArray());
            break;
        }
        case (Dictionary):
        {
            m_value = std::make_unique<DictValue>(other.getDict());
            break;
        }
        case (Null):
            break;
    }
    m_type = other.m_type;
    return *this;
}

JsonObject& JsonObject::operator[](const std::string& key)
{
    if (m_type != Dictionary && m_value == nullptr)
    {
        throw std::runtime_error("Invalid type, wanted Dictionary");
    }
    return asDict()[key];
}

JsonObject& JsonObject::operator[](int index)
{
    std::cout << m_type << std::endl;
    if (m_type != Array)
    {
        throw std::runtime_error("Invalid type, wanted Array");
    }
    return asArray()[index];
}

std::ostream& operator<<(std::ostream& o, ArrayValue& a)
{
    return o << a.format();
}

std::ostream& operator << (std::ostream& o, DictValue& d)
{
    return o << d.format();
}

std::ostream& operator << (std::ostream& o, JsonObject& j)
{
    return o << j.format();
}

std::ostream& operator << (std::ostream& o, const JsonObject& j)
{
    return o << j.format();
}

JsonObject loadFile(std::string filename)
{
    // Read file contents
    std::ifstream file(filename);    //taking file as inputstream
    std::string data;
    if (file)
    {
        std::ostringstream stream;      // New stream
        stream << file.rdbuf();         // Reading data
        data = stream.str();            // Put stream to data string
    }
    else
    {
        std::cout << "File " << filename << " not found." << std::endl;
        return JsonObject();
    }

    // Tokenize string
    Lexer* l = new Lexer(data);

    // Parse string into JSON object
    Parser* p = new Parser(l);
    return p->get();
}

JsonObject loadString(std::string string)
{
    // Tokenize string
    Lexer* l = new Lexer(string);

    // Parse string into JSON object
    Parser* p = new Parser(l);
    return p->get();
}

JSON_NAMESPACE_CLOSE
