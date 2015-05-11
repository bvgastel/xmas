#ifndef PARSE_H
#define PARSE_H

#include <map>
#include <sstream>

#include "memorypool.h"
#include "parser_json.h"
#include "symbolic-interval.h"
#include "symbolic-packet-set.h"
#include "parse-specset.h"
#include "parse-packet-expression-parse-result.h"
#include "parse-packet-function-parse-result.h"
#include "parse-source-expression-parse-result.h"
#include "parse-parsed-xmas-expression-interface.h"
#include "parsed-xmas-function.h"
#include "xmas.h"

PacketExpressionParseResult ParsePacketExpression(const bitpowder::lib::String &str,
                                                  bitpowder::lib::MemoryPool &memoryPool);
SourceExpressionParseResult ParseSourceExpression(const bitpowder::lib::String &str,
                                                  bitpowder::lib::MemoryPool &memoryPool);

struct ParsedXMASFunctionExtension : public XMASComponentExtension {
    std::shared_ptr<ParsedXMASFunction> value;
    ParsedXMASFunctionExtension() {
    }
    ParsedXMASFunctionExtension(const std::shared_ptr<ParsedXMASFunction>& value) : value(value) {
    }
    ParsedXMASFunctionExtension& operator=(const ParsedXMASFunctionExtension& b) {
        this->value = b.value;
        return *this;
    }

};

struct ParsedXMASRestrictedJoin: public XMASComponentExtension {
    int function = 0;
    ParsedXMASRestrictedJoin() {
    }
    ParsedXMASRestrictedJoin(int function) : function(function) {
    }
    ParsedXMASRestrictedJoin& operator=(const ParsedXMASRestrictedJoin& b) {
        this->function = b.function;
        return *this;
    }
};


PacketFunctionParseResult ParsePacketFunction(const bitpowder::lib::String &str,
                                              bitpowder::lib::MemoryPool &mp);


/**
 * @brief Parse Reads a file from a specified filename and parses it using the json parser
 *
 * This function parses a file containing correct JSON for an xmas network
 * and returns a map containing the xmas components (indexed by name) and the json data.
 *
 * Remark that the lifetime of strings in the structure is limited to the lifetime of
 * the memory pool.
 *
 * @param filename the name of the file containing the json for the network.
 *
 * @param mp the memory pool
 * @return the pair of a map to all xmas components and the json data.
 */
std::pair<std::map<bitpowder::lib::String, XMASComponent *>,bitpowder::lib::JSONData>
parse_xmas_from_file(const std::string &filename, bitpowder::lib::MemoryPool &mp);

/**
 * @brief parse_network_json Reads a string containing a json spec of a network and parses it using the json parser
 *
 * This function parses a string containing correct JSON for an xmas network
 * and returns a map containing the xmas components (indexed by name) and the json data.
 *
 * Remark that the lifetime of strings in the structure is limited to the lifetime of
 * the memory pool.
 *
 * @param sstr The stringstream contain the json for the network.
 * @param mp the memory pool.
 * @return the pair of a map to all xmas components and the json data.
 */
std::pair<std::map<bitpowder::lib::String, XMASComponent *>,bitpowder::lib::JSONData>
parse_xmas_from_json(const std::string &str, bitpowder::lib::MemoryPool &mp);

bitpowder::lib::JSONParseResult
read_json_from_file(const std::string &filename, bitpowder::lib::MemoryPool &mp);

/**
 * @brief parse_json_buffer Uses the parsed JSON for a network to return the xmas components.
 *
 * This function uses the parsed JSON for an xmas network to return a map containing the
 * xmas components indexed by name plus the json data.
 *
 * Remark that the lifetime of strings in the structure is limited to the lifetime of
 * the memory pool.
 *
 * @param parseResult the parse result from ParseJSON.
 *
 * @param mp the memory pool.
 * @return the pair of a map to all xmas components and the json data.
 */
std::pair<std::map<bitpowder::lib::String, XMASComponent *>,bitpowder::lib::JSONData>
generate_xmas_from_parse_result(bitpowder::lib::JSONParseResult &parseResult,
                                bitpowder::lib::MemoryPool &mp,
                                std::function<XMASNetwork*(std::string)> getNetwork);

#endif // PARSE_H
