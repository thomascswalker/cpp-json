//#ifndef PARSER_H
//#define PARSER_H
//
//#include <map>
//#include <string>
//#include <vector>
//

//
//JSON_NAMESPACE_OPEN
//
//// Values
//enum ValueType
//{
//    Null,           // nullptr
//    Bool,           // true, false
//    Int,            // 1, 2, 3
//    Double,         // 3.14, 7.62, 50.50
//    String,         // "This is a string."
//    Array,          // { 1, 2, 3, 4, 5 }
//    Dictionary      // { {"Key 1", 5}, {"Key 2", 10} }
//};
//
//enum TokenType
//{
//    ArrayStart,
//    ArrayEnd,
//    ObjectStart,
//    ObjectEnd,
//    Colon,
//    Comma
//};
//
//std::map<std::string, TokenType> TOKEN_MAP
//{
//    {"[", ArrayStart},
//    {"]", ArrayEnd},
//    {"{", ObjectStart},
//    {"}", ObjectEnd},
//    {":", Colon},
//    {",", Comma}
//};
//
//struct Token
//{
//    TokenType tokenType;
//    ValueType valueType;
//    std::string value;
//    int start;
//    int end;
//};
//
//class Parser
//{
//    const char* current = "";
//    std::string text = "";
//    std::vector<Token> tokens;
//    int offset = 0;
//    int depth = 0;
//
//    std::string sanitize(std::string& input)
//    {
//        std::string output;
//        bool inString = false;
//
//        // For every character in the input string...
//        for (auto& c : input)
//        {
//            // Remove new lines and end of lines
//            if (c == '\n' || c == '\0')
//            {
//                continue;
//            }
//
//            // Remove spaces ONLY when outside of a string
//            if (c == ' ' && inString == false)
//            {
//                continue;
//            }
//
//            // Flip-flop context of in or outside of a string
//            if (c == '"')
//            {
//                inString = !inString;
//            }
//
//            // Append current character to sanitized output
//            output += c;
//        }
//
//        return output;
//    }
//
//    bool isKeyword(const char* c)
//    {
//        std::string keywords("[]{}:,");
//        if (keywords.find(*c) != std::string::npos)
//        {
//            std::cout << "Found keyword" << std::endl;
//            return true;
//        }
//
//        // true
//        if (c == "t" && c + 1 == "r" && c + 2 == "u" && c + 3 == "e")
//        {
//            return true;
//        }
//        // false
//        if (c == "f" && c + 1 == "a" && c + 2 == "l" && c + 3 == "s" && c + 4 == "e")
//        {
//            return true;
//        }
//        // false
//        if (c == "n" && c + 1 == "u" && c + 2 == "l" && c + 3 == "l")
//        {
//            return true;
//        }
//
//        return false;
//    }
//
//    Token createToken(TokenType type)
//    {
//        std::cout << "Creating token at: " << offset << std::endl;
//        Token token;
//        token.tokenType = type;
//        token.value = current;
//        token.start = offset;
//        token.end = text.size();
//        return token;
//    }
//
//public:
//    void tokenize(std::string& _text)
//    {
//        text = sanitize(_text);
//        int offset = 0;
//        while (offset < text.size())
//        {
//            // Get current char
//            current = &text[offset];
//            std::cout << current << std::endl;
//
//            if (current == "\0")
//            {
//                break;
//            }
//            else if (isKeyword(current))
//            {
//                Token t = createToken(TOKEN_MAP[current]);
//                tokens.push_back(t);
//                offset++;
//            }
//            else
//            {
//                std::cout << "Unexpected error" << std::endl;
//                offset++;
//            }
//
//            // Increment offset
//        }
//    }
//
//    std::vector<Token> getTokens() { return tokens; }
//};
//
//JSON_NAMESPACE_CLOSE
//
//#endif // !PARSER_H
