#include "jsonprinter.h"


JsonPrinter::JsonPrinter(std::ostream& stream)
  : stream(&stream), ownStream(false)
{
    states.push(State::Init);
}

JsonPrinter::JsonPrinter(std::ostream* stream)
  : stream(stream), ownStream(true)
{
    states.push(State::Init);
}

JsonPrinter::~JsonPrinter()
{
    if (ownStream)
        delete(stream);
}

void JsonPrinter::startObject()  {
    if (!firstItem)
        *stream << ',';

    *stream << '{';
    states.push(State::InObject);
    firstItem = true;
}

void JsonPrinter::endObject() {
    if (states.top() != State::InObject)
        throw JsonPrinter::InvalidStateException {};
    states.pop();
    *stream << '}';
    firstItem = false;
}

void JsonPrinter::startArray()  {
    if (!firstItem)
        *stream << ',';

    *stream << '[';
    states.push(State::InArray);
    firstItem = true;
}

void JsonPrinter::endArray() {
    if (states.top() != State::InArray)
        throw JsonPrinter::InvalidStateException {};
    states.pop();
    *stream << ']';
    firstItem = false;

}

void JsonPrinter::startProperty(const std::string &name) {
    if (states.top() != State::InObject)
        throw JsonPrinter::InvalidStateException {};
    if (!firstItem)
        *stream << ',';

    *stream << '"';
    encodeString(name);
    *stream << '"' << ':';

    states.push(State::InProperty);
    firstItem = true;
}

void JsonPrinter::endProperty() {
    if (states.top() != State::InProperty)
        throw JsonPrinter::InvalidStateException {};
    states.pop();
}


void JsonPrinter::startString() {
    if (states.top() == State::InString)
        throw JsonPrinter::InvalidStateException {};

    *stream << '"';
    states.push(State::InString);
}

void JsonPrinter::endString() {
    if (states.top() != State::InString)
        throw JsonPrinter::InvalidStateException {};

    states.pop();
    *stream << '"';
}

void JsonPrinter::encodeString(const std::string& str) {
    // TODO: encode control codes, for now just encode " \ and \n
    // unicode control range U+0000..U+001F and U+007F.. U+009F
    for (char ch : str) {
        switch (ch) {
        case '\n':
            *stream << '\\' << 'n';
            break;
        case '"':
        case '\\':
            *stream << '\\';        // fall-through
        default:
            *stream << ch;
        }
    }
}

void JsonPrinter::writeString(const std::string& value) {
    State state = states.top();
    if (state != State::InProperty && state != State::InString && state != State::InArray)
        throw JsonPrinter::InvalidStateException {};

    if (states.top() == State::InArray && !firstItem)
        *stream << ',';
    else
        firstItem = false;

    if (state != State::InString) {
        *stream << '"';
        encodeString(value);
        *stream << '"';
    } else
        encodeString(value);
}

void JsonPrinter::writeBool(bool value) {
    if (states.top() != State::InProperty && states.top() != State::InArray)
        throw JsonPrinter::InvalidStateException {};

    if (states.top() == State::InArray && !firstItem)
        *stream << ',';
    else
        firstItem = false;

    *stream << (value ? "true" : "false");
}

void JsonPrinter::writeNull() {
    if (states.top() != State::InProperty && states.top() != State::InArray)
        throw JsonPrinter::InvalidStateException {};

    if (states.top() == State::InArray && !firstItem)
        *stream << ',';
    else
        firstItem = false;

    *stream << "null";
}

void JsonPrinter::writeStringProperty(const std::string& name, const std::string& value) {
    startProperty(name);
    writeString(value);
    endProperty();
}

void JsonPrinter::writeBoolProperty(const std::string& name, bool value) {
    startProperty(name);
    writeBool(value);
    endProperty();
}

void JsonPrinter::writeNullProperty(const std::string& name) {
    startProperty(name);
    writeNull();
    endProperty();
}
