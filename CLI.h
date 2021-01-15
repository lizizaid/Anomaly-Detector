#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include <vector>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO* dio;
    vector<Command*> commands;
    Analyzer* analyzer;

public:
    CLI(DefaultIO* dio);
    void start();
    void add_command(Command* command);
    void generate_commands();
    void print_commands();
    virtual ~CLI();
};

#endif /* CLI_H_ */
