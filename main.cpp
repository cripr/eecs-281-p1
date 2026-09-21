// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1261AAD
#include "Hunt.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <functional>
#include <utility>
#include <cassert>

// 1. longopt reads arguments from commandline, sets mode
// 2. 

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    
    Options options;
    getOptions(argc, argv, options);
    
    Hunt hunt(options.captain_mode, options.first_mate_mode, options.hunt_order);
    hunt.load_from_input();

    bool found = hunt.captain_hunt(options.captain_mode, options.first_mate_mode, hunt.get_start());
    if (found) {
        hunt.cal_path();
    }

    if(options.verbose) {
        hunt.printVerbose(found);
    }
    if(options.stats) {
        hunt.printStats(found);
    }
    if(options.show_path == "M" || options.show_path == "L") {
        if(found) {
            if(options.show_path == "M") {hunt.build_path_map();}
            hunt.printPath(options.show_path);
        }
    }

    if(found) {
        std::cout << "Treasure found at " << hunt.get_treasure().row << "," <<
        hunt.get_treasure().col << " with path length " << 
        hunt.getPath() << ".\n" << std::flush;
    }
    else {
        std::cout << "No treasure found after investigating " << hunt.getWater() + hunt.getLand() << " locations.\n" << std::flush;
    }
}