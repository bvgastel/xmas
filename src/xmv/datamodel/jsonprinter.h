#ifndef JSON_PRINTER
#define JSON_PRINTER

#include <ostream>
#include <stack>
#include <type_traits>

class JsonPrinter
{
public:
    JsonPrinter(std::ostream&);
    JsonPrinter(std::ostream*);
    ~JsonPrinter();

    void startObject();
    void endObject();

    void startArray();
    void endArray();

    void startProperty(const std::string& name);
    void endProperty();

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
    writeNumber(T value);

    void writeString(const std::string& value);
    void writeBool(bool value);
    void writeNull();

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
    writeNumberProperty(const std::string& name, T value);

    void writeStringProperty(const std::string& name, const std::string& value);
    void writeBoolProperty(const std::string& name, bool value);
    void writeNullProperty(const std::string& name);

    class InvalidStateException
    {
    };

private:
    std::ostream*   stream;
    bool            ownStream;

    bool            firstProperty   = true;
    bool            firstElement    = true;

    enum class State {
        Init,
        InObject,
        InProperty,
        InArray
    };
    std::stack<State>       states;

};

template<typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
JsonPrinter::writeNumber(T value) {
    if (states.top() != State::InProperty && states.top() != State::InArray)
        throw JsonPrinter::InvalidStateException {};

    if (states.top() == State::InArray && !firstElement)
        *stream << ',';
    else
        firstElement = false;

    *stream << value;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
JsonPrinter::writeNumberProperty(const std::string& name, T value) {
    startProperty(name);
    writeNumber(value);
    endProperty();
}

#endif // JSON_PRINTER

