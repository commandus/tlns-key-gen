/**
 * @brief generate LoRaWAN keys
 * @file tlns-key-gen.cpp
 * Copyright (c) 2023 Andrei Ivanov andrei.i.ivanov@gmail.com
 * MIT license
 * Usage:
 *   ./tlns-key-gen <magic string>
 */
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "argtable3/argtable3.h"
#include "key128gen.h"
#include "key-string.h"
#include "err-list.h"

const std::string programName = "tlns-key-gen";

class TlnsKeyGenConfiguration {
public:
    std::vector<std::string> magic;
    std::vector<uint32_t> address;
    int verbosity;			// verbosity level
};

/**
 * Parse command line
 * Return 0- success
 *        1- show help and exit, or command syntax error
 *        2- output file does not exists or can not open to write
 **/
int parseCmd(
        TlnsKeyGenConfiguration *config,
        int argc,
        char *argv[])
{
    // magic string(key)
    struct arg_str* a_magic = arg_strn("k", "key", "<magic-string>", 1, 100, "Magic");
    struct arg_str* a_address = arg_strn(nullptr, nullptr, "<hex-number>", 1, 100, "address");

    struct arg_lit *a_verbosity = arg_litn("v", "verbose", 0, 3, "Set verbosity level");
    struct arg_lit *a_help = arg_lit0("?", "help", "Show this help");
    struct arg_end *a_end = arg_end(20);

    void *argtable[] = {
        a_magic, a_address, a_verbosity, a_help, a_end
    };

    // verify the argtable[] entries were allocated successfully
    if (arg_nullcheck(argtable) != 0) {
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 1;
    }
    // Parse the command line as defined by argtable[]
    int nerrors = arg_parse(argc, argv, argtable);

    config->verbosity = a_verbosity->count;
    for (size_t i = 0; i < a_magic->count; i++) {
        config->magic.push_back(std::string(a_magic->sval[i]));
    }
    for (size_t i = 0; i < a_address->count; i++) {
        config->address.push_back(strtoul(a_address->sval[i], nullptr, 16));
    }

    // special case: '--help' takes precedence over error reporting
    if ((a_help->count) || nerrors) {
        if (nerrors)
            arg_print_errors(stderr, a_end, programName.c_str());
        std::cerr << "Usage: " << programName << std::endl;
        arg_print_syntax(stderr, argtable, "\n");
        std::cerr
            << "Print keys" << std::endl;
        arg_print_glossary(stderr, argtable, "  %-25s %s\n");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return ERR_CODE_COMMAND_LINE;
    }

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return 0;
}

int main(int argc, char **argv)
{
    TlnsKeyGenConfiguration config;
    int r = parseCmd(&config, argc, argv);
    if (r != 0)
        exit(ERR_CODE_COMMAND_LINE);
    for (std::vector<std::string>::const_iterator it(config.magic.begin()); it != config.magic.end(); it++) {
        if (config.verbosity > 0)
            std::cout << *it << "\n";
        uint8_t phraseKey[16];
        phrase2key(phraseKey, it->c_str(), it->size());
        for (std::vector<uint32_t>::const_iterator ita(config.address.begin()); ita != config.address.end(); ita++) {
            std::cout << std::hex << *ita << ": ";
            for (int i = 0; i < 4; i++) {
                uint8_t key[16];
                keyGen(key, i, phraseKey, *ita);
                std::cout << std::hex << hexString(key, 16)  << " ";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
