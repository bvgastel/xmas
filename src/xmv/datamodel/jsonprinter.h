#ifndef JSON_PRINTER
#define JSON_PRINTER

#include <ostream>
#include <stack>

class JsonPrinter
{
public:
    JsonPrinter(std::ostream&);
    JsonPrinter(std::ostream*);
    ~JsonPrinter();

    void startObject();
    void endObject();

    //void startArray();
    //void endArray();

    void startProperty(const std::string& name);
    void endProperty();

    void writeNumber(int value);
    void writeString(const std::string& value);

    class InvalidStateException
    {
    };

private:
    std::ostream*   stream;
    bool            ownStream;

    bool            firstProperty;

    enum class State {
        Init,
        InObject,
        InProperty,
        InArray
    };
    std::stack<State>       states;

};

#endif // JSON_PRINTER

