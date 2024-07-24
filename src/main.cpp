#include <ostream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <iomanip>

#include "include/clok.h"
#include "include/utils.h"
#include "include/shared.h"

using std::cout, std::endl, std::string;

char** args;
int args_len;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: '" << argv[0] << " [command]'" << endl;
        cout << "Try '" << argv[0] << " help'" << endl;
        std::exit(1);
    }

    args = argv;
    args_len = argc;
    string command = argv[1];

    if (command == "help") help();
    else if (command == "term-clock") term_clock();

    return 0;
}

void term_clock() {
    string re = "\r\033[K";
    if (got_flag("-n")) re = "\n";

    while (1) {
        int hours;
        int minutes;
        int seconds;

        get_time(&hours, &minutes, &seconds);

        cout << std::setfill('0') << std::setw(2) << hours << ":"
              << std::setfill('0') << std::setw(2) << minutes << ":"
              << std::setfill('0') << std::setw(2) << seconds << std::flush;

        sleep(1);
        cout << re;
    }
}

void help() {
    cout << help_text << endl;
}
