#include "CLI.h"

CLI::CLI(DefaultIO* dio) : dio(dio){
    analyzer = new Analyzer(new HybridAnomalyDetector);
    generate_commands();
}

void CLI::start(){
    float opt_temp;
    int opt;
    int size = commands.size();

    while (true) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        print_commands();
        dio->read(&opt_temp);
        opt = (int) opt_temp - 1;
        if (opt == 5) break;
        else if (opt < 0 || opt >= size) {
            dio->write("INVALID OPTION. Please choose a valid option.\n");
        }
        else
            commands[opt]->execute();
    }
}

void CLI::add_command(Command* command){
    commands.push_back(command);
}

void CLI::generate_commands(){
    add_command(new UploadCsvFile(dio, analyzer));
    add_command(new AlgoSettings(dio, analyzer));
    add_command(new DetectAnomalies(dio, analyzer));
    add_command(new DisplayResults(dio, analyzer));
    add_command(new AnalyzeResults(dio, analyzer));
    add_command(new Exit(dio, analyzer));
}

void CLI::print_commands() {
    int size = commands.size();
    for (int i = 0; i < size; i++){
        dio->write(i+1);
        string option = "." + commands[i]->get_description() + "\n";
        dio->write(option);
    }
}

CLI::~CLI() {
    int size = commands.size();
    for (int i = 0; i < size; i++){
        delete commands[i];
    }
    delete analyzer;
}

