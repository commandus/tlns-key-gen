#include <string>

class KEY128 {
public:
    union {
        unsigned char c[16];
        struct {
            uint64_t u[2];
        };
    };
    KEY128();
    explicit KEY128(const std::string &hex);
    explicit KEY128(const char* hex);
    KEY128(KEY128 &value);
    KEY128(uint64_t hi, uint64_t lo);
    std::size_t operator()(const KEY128 &value) const;
    bool operator==(const KEY128 &rhs) const;
    bool operator<(const KEY128 &rhs) const;
    bool operator>(const KEY128 &rhs) const;
    bool operator!=(const KEY128 &rhs) const;
};   // 16 bytes

uint32_t calc();
