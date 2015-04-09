#ifndef FEEDBACKINTERFACE
#define FEEDBACKINTERFACE

#include <ostream>
#include <vector>

enum class FeedbackSeverity
{
    Info,
    Warning,
    Fault,
    Crash
};

class XMASComponent;
class Port;

/**
 * @brief feedback_message
 * @param sender name of the verification tool
 * @param severity severity of the detected issue
 * @param message description of the issue detected by the verification tool
 * @param components list of all components (if relevant) that are affected by this issue
 * @param ports list of all ports (if relevant) that are affected by this issue
 */
void feedback_message(  const std::string& sender,
                        FeedbackSeverity severity,
                        const std::string& message,
                        const std::vector<XMASComponent*>& components = {},
                        const std::vector<Port*>& ports = {});

/**
 * @brief feedback_progress
 * @param sender name of the verification tool
 * @param progress progress of the verification tool, relative to total
 * @param total see progress
 */
void feedback_progress( const std::string& sender, int progress, int total);


/**
 * @brief feedback_set_stream set the feedback output stream (default std::cout)
 * @param stream
 */
void feedback_set_stream(std::ostream& stream);



#endif // FEEDBACKINTERFACE

