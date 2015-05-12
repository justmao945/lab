#ifndef JSON_HH
#define JSON_HH

#include <cassert>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

namespace json {

class Value;

typedef std::string             String;
typedef double                  Number;
typedef std::map<String,Value>  Object;
typedef std::vector<Value>      Array;
// lack of True, False and Null type

class Value {
public:
    Value()  { _init(NullType); }
    ~Value() { _destroy(); }
    Value(const String& v) { _init(StringType); _str = new String(v); }
    Value(const Number  v) { _init(NumberType); _num = v; }
    Value(const Object& v) { _init(ObjectType); _obj = new Object(v); }
    Value(const Array&  v) { _init(ArrayType);  _arr = new Array(v); }
    Value(const bool    v) { _init(v ? TrueType : FalseType); }
    Value(const Value&  v) { _copy(v); }
    Value& operator=(const String& v) { _destroy(); _init(StringType); _str = new String(v); return *this; }
    Value& operator=(const Number v)  { _destroy(); _init(NumberType); _num = v; return *this; }
    Value& operator=(const Object& v) { _destroy(); _init(ObjectType); _obj = new Object(v); return *this; }
    Value& operator=(const Array& v)  { _destroy(); _init(ArrayType); _arr = new Array(v); return *this; }
    Value& operator=(const bool v)    { _destroy(); _init(v ? TrueType : FalseType); return *this; }
    Value& operator=(const Value& v)  { if(this != &v) { _destroy(); _copy(v); } return *this; }

    bool isString() const { return _type == StringType; }
    bool isNumber() const { return _type == NumberType; }
    bool isObject() const { return _type == ObjectType; }
    bool isArray()  const { return _type == ArrayType;  }
    bool isTrue()   const { return _type == TrueType;   }
    bool isFalse()  const { return _type == FalseType;  }
    bool isNull()   const { return _type == NullType;   }

    const String& toString() const { assert(isString()); return *_str; }
    const Number  toNumber() const { assert(isNumber()); return _num;  }
    const Object& toObject() const { assert(isObject()); return *_obj; }
    const Array&  toArray()  const { assert(isArray());  return *_arr; }
    const bool    toBool()   const { assert(isTrue() || isFalse()); return isTrue(); }
  
    bool save(std::ostream& os) const {
        switch(_type) {
        case StringType:
            assert(_str);
            os << '"';
            for(String::const_iterator it = _str->begin(); it != _str->end(); ++it) {
                switch(*it) {
                case '"' : os << "\\\""; break;
                case '\\': os << "\\\\"; break;
                case '\b': os << "\\b"; break;
                case '\f': os << "\\f"; break;
                case '\n': os << "\\n"; break;
                case '\r': os << "\\r"; break;
                case '\t': os << "\\t"; break;
                default:   os << *it;    break;
                }
            }
            os << '"';
            break;
        case NumberType:
            os << _num;
            break;
        case ObjectType:
            os << "{";
            assert(_obj);
            for(Object::const_iterator it = _obj->begin(); it != _obj->end(); ++it) {
                if(it != _obj->begin()) {
                    os << ",";
                }
                os << '"' << it->first << '"' << ':';
                it->second.save(os);
            }
            os << "}";
            break;
        case ArrayType:
            os << "[";
            assert(_arr);
            for(Array::const_iterator it = _arr->begin(); it != _arr->end(); ++it) {
                if(it != _arr->begin()) {
                    os << ",";
                }
                it->save(os);
            }
            os << "]";
            break;
        case TrueType:
            os << "true";
            break;
        case FalseType:
            os << "false";
            break;
        case NullType:
            os << "null";
            break;
        default:
            assert(0 && "error type");
            return false;
        }
        return true;
    }

    bool load(std::istream& is) {
        std::queue<Token> q;
        return _parse(is, *this, q);
    }

private:
    enum Type { StringType, NumberType, ObjectType, ArrayType, TrueType, FalseType, NullType, };

    enum TokenType {
        TokenEOF, TokenString, TokenNumber, TokenTFN, TokenError,
        TokenComma    = ',', TokenColon    = ':',
        TokenLBracket = '[', TokenRBracket = ']',
        TokenLBrace   = '{', TokenRBrace   = '}',
    };

    struct Token {
        TokenType type;
        String text; // for TokenString, TokenTFN
        Number num; // for TokenNumber
        
        std::string toString() const {
            std::stringstream ss; ss << num;
            switch(type) {
                case TokenEOF:      return "EOF";
                case TokenString:   return "\"" + text + "\"";
                case TokenNumber:   return ss.str();
                case TokenTFN:      return text;
                case TokenError:    return "error: " + text;
                default:            return text;
            }
        }
    };

    bool _isValueType() const {
        return _type == NumberType || _type == TrueType || _type == FalseType || _type == NullType;
    }

    void _init(Type type) {
        _type = type;
        _ref = 0; _str = 0; _obj = 0; _arr = 0;
        if(! _isValueType()) {
            _ref = new unsigned long(1);
        }
    }

    void _copy(const Value& v) {
        _type = v._type;
        if(_isValueType()) {
            _num = v._num;
        } else {
            _ref = v._ref; _str = v._str; _obj = v._obj; _arr = v._arr;
            assert(_ref); ++*_ref;
        }
    }

    void _destroy() {
        if(! _isValueType()) {
            assert(_ref);
            if(--*_ref == 0) {
                delete _str; delete _obj; delete _arr;
            }
            _str = 0; _obj = 0; _arr = 0;
        }
    }

    Token _next(std::istream& is, std::queue<Token>& q) {
        if(! q.empty()) {
            Token t = q.front();
            q.pop();
            return t;
        }
        enum State { s_start, s_str, s_tfn, s_esc, s_end } s = s_start; Token t; char c;
        // test end before get
        while(s != s_end && is.get(c)) {
            bool sv = true;
            switch(s) {
            case s_start:
                if(isspace(c)) {
                    sv = false;
                }
                else if(c == ',' || c == ':' || c == '[' || c == ']' || c == '{' || c == '}') {
                    t.type = TokenType(c);
                    s = s_end;
                }
                else if(c == '"') {
                    t.type = TokenString;
                    sv = false;
                    s = s_str;
                }
                else if(c == '-' || isdigit(c) ) {
                    t.type = TokenNumber;
                    is.unget();
                    is >> t.num;
                    assert(! is.fail());
                    sv = false;
                    s = s_end;
                }
                else if(islower(c)) {
                    t.type = TokenTFN;
                    s = s_tfn;
                }
                else {
                    // error token
                    t.type = TokenError;
                    s = s_end;
                }
                break;
            case s_str:
                if(c == '\\') {
                    sv = false;
                    s = s_esc;
                }
                else if(c == '"') {
                    sv = false;
                    s = s_end;
                }
                break;
            case s_tfn:
                if(!islower(c)) {
                    s = s_end;
                    sv = false;
                    is.unget();
                }
                break;
            case s_esc:
                switch(c) {
                case 'b': c = '\b'; break;
                case 'f': c = '\f'; break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case 't': c = '\t'; break;
                // case 'u'
                }
                s = s_str;
                break;
            case s_end:
            default:
                assert(0 && "bug");
                break;
            }
            if(sv) {
                t.text.push_back(c);
            }
        }
        if(s != s_end) {
            t.type = TokenEOF;
        }
        return t;
    }

    // @return false if error
    bool _parse(std::istream& is, Value& v, std::queue<Token>& q) {
        Token t = _next(is, q);
        switch(t.type) {
        case TokenString:
            v = t.text;
            return true;
        case TokenNumber:
            v = t.num;
            return true;
        case TokenTFN:
            if(t.text == "null") {
                v = Value();
            } else if(t.text == "true") {
                v = Value(true);
            } else if(t.text == "false") {
                v = Value(false);
            } else {
                // error
                v = Value(t.text);
                return false;
            }
            return true;
        case TokenLBrace: {
            Object obj;
            while(1) {
                Token n = _next(is, q);
                if(n.type == TokenRBrace) {
                    v = obj;
                    return true;
                } else {
                    q.push(n);
                }
                Value key, val;
                if(! _parse(is, key, q)) {
                    return false;
                }
                if(! key.isString()) {
                    // error
                    return false;
                }
                n = _next(is, q);
                if(n.type != TokenColon) {
                    // error
                    return false;
                }
                if(! _parse(is, val, q)) {
                    return false;
                }
                n = _next(is, q);
                if(n.type != TokenComma) {
                    if(n.type == TokenRBrace) {
                        q.push(n);
                    } else {
                        return false;
                    }
                }
                obj[key.toString()] = val;
            }
            v = obj;
            return true;
        }
        case TokenLBracket: {
            Array arr;
            while(1) {
                Token n = _next(is, q);
                if(n.type == TokenRBracket) {
                    v = arr;
                    return true;
                } else {
                    q.push(n);
                }
                Value elem;
                if(! _parse(is, elem, q)) {
                    return false;
                }
                n = _next(is, q);
                if(n.type != TokenComma) {
                    if(n.type == TokenRBracket) {
                        q.push(n);
                    } else {
                        return false;
                    }
                }
                arr.push_back(elem);
            }
            v = arr;
            return true;
        }
        default:
            // error
            return false;
        }
    }

private:    
    Type   _type;
    Number _num;
    unsigned long *_ref;
    String *_str;
    Object *_obj;
    Array  *_arr;
};

} // end of json


#endif
