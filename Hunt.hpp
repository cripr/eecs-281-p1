// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1261AAD

#ifndef HUNT__HPP
#define HUNT__HPP

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
#include <getopt.h>

// put all operations here
// everything should be controlled as member functions and member variables
// 1. reading maps (either mapfile, listfile should create the same grid map)
// 2. hunt operations
// - use containers of structs, so it contains row, col, terrain type, bool investigated
// - use deque, and only switch output method between stack/queue
// - store investigated paths into path vector
// 3. use bool discovered, to keep track of elements in stack
// - if reached dead end, clear path vector
// - start over from next order in container
// 4. check if treasure was found or not
// 5. if treasure not found, captain keeps searching, repeat process
// 6. if treasure found, program ends

// for separating hunts
enum class Role {
    CAPTAIN,
    FIRST_MATE,
};
enum class Mode {
    kStack,
    kQueue,
};
struct Options {
    Mode captain_mode = Mode::kStack;
    Mode first_mate_mode = Mode::kQueue;
    std::string hunt_order = "NESW";
    bool verbose = false;
    bool stats = false;
    std::string show_path;
};  // Options{}

void printHelp(char const *command);

void getOptions(int argc, char **argv, Options &options);

class Hunt {
    struct Block;
    struct Coordinates;
    public:
    // read command line
    void load_from_input();

    // make a hunt variable that will operate the entire game.
    Hunt();
    Hunt(Mode &captain, Mode &first_mate, std::string &hunt_order_input);
    
    // functions to check variables inside the hunt algorithm
    //char terrain_at(int row, int col);
    int get_size();
    
    // functions to run each iteration of searches
    bool captain_hunt(Mode &capmode, Mode &matemode, Coordinates current);
    bool first_mate_hunt(Mode &matemode, Coordinates current);
    std::pair<int, int> directionOffset(char direction);
    void cal_path();
    std::deque<char> build_path();
    void build_path_map();

    //try parent out in block struct
    std::pair<int, int> getParent(int row, int col, char direction);

    Coordinates get_start();
    Coordinates get_treasure();
    int getLand();
    int getWater();
    int getPath();

     // 4. printing output
    void printStats(bool treasure_found);
    void printPath(std::string &path_option);
    void printVerbose(bool treasure_found);

    private:
    // each block of the map
    struct Block {
        char terrain = '.';
        char parent = '\0';
    };
    struct Coordinates {
        int row;
        int col;
    };

    // 1. variables for making the grid
    int size = 0;
    std::vector<std::vector<Block>> map;
    //std::vector<std::vector<char>> path_track;
    //std::vector<std::vector<char>> path_map;
    char file_type;
    //std::string filename = "";
    Coordinates starting;

    // 2. variables for hunt operation
    Mode captain_mode;
    Mode first_mate_mode;
    std::string huntOrder;
    //std::pair<int, int> directions;
    Coordinates TREASURE;
    std::vector<Coordinates> ashore_list;

    // 3. variables for storing output results
    int water_investigated = 0;
    int land_investigated = 0;
    int ashore = 0;
    int path_length = 0;

};

#endif