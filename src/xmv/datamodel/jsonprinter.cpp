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
    *stream << '}';
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
