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
    *stream << '{';
    states.push(State::InObject);
    firstProperty = true;
}

void JsonPrinter::endObject() {
    if (states.top() != State::InObject)
        throw JsonPrinter::InvalidStateException {};
    states.pop();
    *stream << '}';
}

void JsonPrinter::startArray()  {
    *stream << '[';
    states.push(State::InArray);
    firstElement = true;
}

void JsonPrinter::endArray() {
    if (states.top() != State::InArray)
        throw JsonPrinter::InvalidStateException {};
    states.pop();
    *stream << ']';
}

void JsonPrinter::startProperty(const std::string &name) {
    if (states.top() != State::InObject)
        throw JsonPrinter::InvalidStateException {};
    if (!firstProperty) {
        *stream << ',';
    } else {
        firstProperty = false;
    }
    *stream << '"' << name << '"' << ':';
    states.push(State::InProperty);
}

void JsonPrinter::endProperty() {
    if (states.top() != State::InProperty)
        throw JsonPrinter::InvalidStateException {};
    states.pop();
}


void JsonPrinter::writeNumber(int value) {
    *stream << std::to_string(value);
}

void JsonPrinter::writeString(const std::string& value) {
    *stream << '"' << value << '"';
}

void JsonPrinter::writeBool(bool value) {
    *stream << (value ? "true" : "false");
}

void JsonPrinter::writeNull() {
    *stream << "null";
}

void JsonPrinter::writeNumberProperty(const std::string& name, int value) {
    startProperty(name);
    writeNumber(value);
    endProperty();
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
