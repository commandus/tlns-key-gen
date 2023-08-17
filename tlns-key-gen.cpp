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
#include <iomanip>
#include <algorithm>

#include "argtable3/argtable3.h"
#include "key128gen.h"
#include "err-list.h"
#include "lorawan-types.h"
#include "lorawan-string.h"

const std::string programName = "tlns-key-gen";

class AddressRange {
private:
    bool parseTypeNwkAddr(
        const char *value
    )
    {
        std::string s(value);
        auto p = s.find(':');
        if (p == std::string::npos)
            return false;
        uint8_t t = (uint8_t) strtoul(s.substr(0, p).c_str(), nullptr, 16);
        auto p2 = s.find(':', p + 1);
        if (p2 == std::string::npos)
            return false;
        uint32_t nid = strtoul(s.substr(p + 1, p2 - p - 1).c_str(), nullptr, 16);
        auto p3 = s.find('-', p2 + 1);
        if (p3 == std::string::npos)
            return false;
        uint32_t a = strtoul(s.substr(p2 + 1, p3 - p2 - 1).c_str(), nullptr, 16);
        addr_start.set(t, nid, a);
        uint32_t a2 = strtoul(s.substr(p3 + 1).c_str(), nullptr, 16);
        addr_finish.set(t, nid, a2);
        return true;
    }

    bool parseAddrAddr(
        const char *value
    )
    {
        std::string s(value);
        auto p = s.find('-');
        if (p == std::string::npos)
            return false;
        uint32_t a1 = (uint32_t) strtoul(s.substr(0, p).c_str(), nullptr, 16);
        uint32_t a2 = (uint32_t) strtoul(s.substr(p + 1).c_str(), nullptr, 16);
        addr_start.set(a1);
        addr_finish.set(a2);
        return true;
    }

public:
    DEVADDR addr_start;
    DEVADDR addr_finish;
    AddressRange() = default;
    explicit AddressRange(const char *value) {
        parse(value);
    }

    bool parse(
        const char *value
    )
    {
        if (!parseTypeNwkAddr(value))
            return parseAddrAddr(value);
        else
            return true;        
    }
};

class TlnsKeyGenConfiguration {
public:
    std::vector<std::string> magic;
    std::vector<uint32_t> address;
    std::vector<AddressRange> address_range;
    int verbosity{};			// verbosity level
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
    struct arg_str* a_magic = arg_strn("k", "key", "<passphrase>", 0, 100, "Passphrase (10-20 characters). Default- random number");
    struct arg_str* a_address = arg_strn(nullptr, nullptr, "<addr> | <addr-addr>| <T:NwkId:addr-addr>", 1, 100, "address up to 8 hex digits). Address range: T- type(0..7), NwkId- network id (0..N)");
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
        config->magic.emplace_back(a_magic->sval[i]);
    }
    for (size_t i = 0; i < a_address->count; i++) {
        AddressRange ar;
        if (ar.parse(a_address->sval[i]))
            config->address_range.push_back(ar);
        else
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

void printKeys(
    std::ostream &strm,
    const std::string &masterkey,
    uint8_t *phraseKey,
    uint32_t addr
)
{
    // generate EUI
    uint8_t eui[8];
    euiGen(eui, KEY_NUMBER_EUI, phraseKey, addr);
    strm << std::setw(8) << std::hex << addr << " ";
    strm << std::hex << std::setw(16) << hexString(eui, 8)  << " ";

    for (int i = KEY_NUMBER_NWK; i <= KEY_NUMBER_APP; i++) {
        uint8_t key[16];
        keyGen(key, i, phraseKey, addr);
        strm << std::hex << std::setw(32) << hexString(key, 16)  << " ";
    }
    strm << " " << masterkey << std::endl;
}

int main(int argc, char **argv)
{
    TlnsKeyGenConfiguration config;
    int r = parseCmd(&config, argc, argv);
    if (r != 0)
        exit(ERR_CODE_COMMAND_LINE);
    if (config.magic.empty())
        config.magic.emplace_back("");

    if (config.verbosity > 0)
        std::cout << "| Addr  | EUI            | NWK                            | APP                            | master Key\n"
                  << "+-------+----------------+--------------------------------+--------------------------------+------------\n";

    for (std::vector<std::string>::const_iterator it(config.magic.begin()); it != config.magic.end(); it++) {
        uint8_t phraseKey[16];
        if (it->empty()) {
            rnd2key(phraseKey);
        } else {
            // generate "master key" by the passphrase
            phrase2key(phraseKey, it->c_str(), it->size());
        }
        for (std::vector<uint32_t>::const_iterator ita(config.address.begin()); ita != config.address.end(); ita++) {
            printKeys(std::cout, *it, phraseKey, *ita);
        }
        for (std::vector<AddressRange>::const_iterator itr(config.address_range.begin()); itr != config.address_range.end(); itr++) {
            for (DEVADDR itra(itr->addr_start); itra <= itr->addr_finish; ++itra) {
                printKeys(std::cout, *it, phraseKey, itra.get());
            }
        }
    }
    return 0;
}
