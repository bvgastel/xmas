#include "feedback-interface.h"
#include "xmas.h"

using bitpowder::lib::String;

static std::ostream* stream = &std::cout;

const char* severityName(FeedbackSeverity severity) {
    switch (severity) {
        case FeedbackSeverity::Info:    return "INFO";
        case FeedbackSeverity::Warning: return "WARNING";
        case FeedbackSeverity::Fault:   return "FAULT";
        case FeedbackSeverity::Crash:   return "CRASH";
    }
    return "???";
}

/*
 * Formats and streams a vector (of components or ports) using a comma separated array notation.
 *
 * (Note: combining templates and lambdas requires the use of GetValue as a template parameter)
 * See also: http://stackoverflow.com/questions/9998402/c11-does-not-deduce-type-when-stdfunction-or-lambda-functions-are-involved
 */
template <typename T, typename GetValue>
void streamVector(std::ostream& stream, const std::vector<T*>& v, GetValue f)
{
    bool first = true;
    stream << '[';
    for (auto x : v) {
        if (!first) {
            stream << ", ";
        } else {
            first = false;
        }

        stream << f(x);
    }
    stream << ']';
}



void feedback_message(  const std::string& sender,
                        FeedbackSeverity severity,
                        const std::string& message,
                        const std::vector<XMASComponent*>& components,
                        const std::vector<Port*>& ports)
{
    *stream << '[' << sender << "] **" << severityName(severity) << "**: " << message;

    if (components.size() > 0) {
        *stream << "  Comp: ";
        streamVector(*stream, components, [] (XMASComponent* c) { return c->getStdName(); });

    }

    if (ports.size() > 0) {
        *stream <<"  Port: ";
        streamVector(*stream, ports, [] (Port* p) { return p->getComponent()->getStdName() + '.' + p->getName(); });
    }

    *stream << std::endl;
}

std::ostream& feedback_stream(  const std::string& sender,
                                FeedbackSeverity severity)
{
    return *stream << '[' << sender << "] **" << severityName(severity) << "**: ";
}


void feedback_progress( const std::string& sender, int progress, int total)
{
    *stream << '[' << sender << "] **PROGRESS** " << progress << '/' << total << std::endl;
}

void feedback_set_stream(std::ostream& stream)
{
    ::stream = &stream;
}

