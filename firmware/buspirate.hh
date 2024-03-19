#ifndef BUSPIRATE_HH_
#define BUSPIRATE_HH_

#include "tokens.hh"
#include <cstdint>

class BusPirate {
public:
    void parse(char* line);

private:
    void execute(uint8_t token_n, Token* tokens);
};

#endif //BUSPIRATE_HH_
