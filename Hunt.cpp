// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1261AAD

#include "Hunt.hpp"


void printHelp(char *command) {
    std::cout << "Usage: " << command << " [-c|-f <STACK|QUEUE>] | -h\n" << std::flush;
}

//void printStats(char *command) {
//    std::cout << "results!" << std::flush;
//}

// Process the command line; there is no return value, but the Options
// struct is passed by reference and is modified by this function to send
// information back to the calling function.
void getOptions(int argc, char **argv, Options &options) {
    // These are used with getopt_long()
    opterr = static_cast<int>(false);  // Let us handle all error output for command line options
    int choice = 0;
    int index = 0;

    // NOLINTBEGIN: getopt predates C++ style, this usage is from `man getopt`
    option longOptions[] = {
  
  {"help", no_argument, nullptr, 'h'},
  {"captain", required_argument, nullptr, 'c'},
  {"first-mate", required_argument, nullptr, 'f'},
  {"hunt-order", required_argument, nullptr, 'o'},
  {"verbose", no_argument, nullptr, 'v'},
  {"stats", no_argument, nullptr, 's'},
  {"show-path", required_argument, nullptr, 'p'},
        {nullptr, 0, nullptr, '\0'},
    };  // longOptions[]
    // NOLINTEND

    // Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "hc:f:o:vsp:", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(*argv);
            exit(0);

        case 'c': {  // Need a block here to declare a variable inside a case
            std::string arg { optarg };
            if (arg != "STACK" && arg != "QUEUE") {
                // The first line of error output has to be a 'fixed' message
                // for the autograder to show it to you.
                std::cerr << "Invalid argument to --captain\n" << std::flush;
                exit(1);
            }  // if ..arg valid

            if (arg == "STACK") {
                options.captain_mode = Mode::kStack;
            } else {
                options.captain_mode = Mode::kQueue;
            } 
            break;
        }
        case 'f': {  // Need a block here to declare a variable inside a case
            std::string arg { optarg };
            if (arg != "STACK" && arg != "QUEUE") {
                // The first line of error output has to be a 'fixed' message
                // for the autograder to show it to you.
                std::cerr << "Invalid argument to --first-mate\n" << std::flush;
                exit(1);
            }  // if ..arg valid

            if (arg == "STACK") {
                options.first_mate_mode = Mode::kStack;
            } else {
                options.first_mate_mode = Mode::kQueue;
            } 
            break;
        }
        case 'o': {
            std::string arg { optarg };
            if(arg.size() != 4) {
                std::cerr << "Invalid argument to --hunt-order\n" << std::flush;
                exit(1);
            }
            std::string word = arg;
            std::sort(word.begin(), word.end());
            if(word != "ENSW") {
                std::cerr << "Invalid argument to --hunt-order\n" << std::flush;
                exit(1);                    
            }
            options.hunt_order = optarg;
            break;
        }
        case 'v': {
            options.verbose = true;
            break;
        }
        case 's': {
            options.stats = true;
            break;
        }
        case 'p': {
            std::string arg { optarg };
            if(arg != "M" && arg != "L") {
                std::cerr << "Invalid argument to --show-path" << std::flush;
                exit(1); 
            }
            if(!(options.show_path.empty())) {
                std::cerr << "Specify --show-path only once" << std::flush;
                exit(1);                 
            }
            options.show_path = arg;
            break;
        }

        default:
            std::cerr << "Unknown option\n" << std::flush;
            exit(1);

        }  // switch ..choice
    }  // while
}  // getOptions()

// create map with this function
void Hunt::load_from_input() {
    int money = false;
    int start = false;
    std::string junk;
    while (std::cin.peek() == '#') {
        getline(std::cin, junk);
    }
    if (!(std::cin >> file_type) || !(std::cin >> size) || size < 2) {
        exit(1);
    }

    map.resize(size);
    path_track.resize(size);
    path_map.resize(size);
    for (int row = 0; row < size; row++) {
        map[row].resize(size);
        path_track[row].resize(size);
        path_map[row].resize(size);
    }

    if(file_type == 'M') {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                char terrain;
                std::cin >> terrain;
                Block b = {row, col, terrain, false};
                if(!(b.terrain == '@') && !(b.terrain == '$') && 
                !(b.terrain == 'o') && !(b.terrain == '.') && 
                !(b.terrain == '#')) {
                    std::cerr << "Invalid terrain type\n" << std::flush;
                    exit(1);                    
                }  
                if (b.terrain == '@') {
                    b.discovered = true;
                    start++;
                    starting = b;
                }
                map[row][col] = b;              
                if (b.terrain == '$') {
                    money++;
                }
            }
        }
    }
    else if (file_type == 'L') {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                map[row][col] = {row, col, '.', false};
            }
        }
        int input_row;
        int input_col;
        char terrain;
        while (std::cin >> input_row >> input_col >> terrain) {
            if((input_row > size - 1) || (input_row < 0) ||
            (input_col > size - 1) || (input_col < 0)) {
                std::cerr << "Invalid coordinates in list mode input\n" << std::flush;
                exit(1);                 
            }
            Block b = {input_row, input_col, terrain, false};
            if(!(b.terrain == '@') && !(b.terrain == '$') && 
            !(b.terrain == 'o') && !(b.terrain == '.') && 
            !(b.terrain == '#')) {
                std::cerr << "Invalid terrain type\n" << std::flush;
                exit(1);                    
            }            
            if (b.terrain == '@') {
                b.discovered = true;
                start++;
                starting = b;
            }
            map[input_row][input_col] = b;
            if (b.terrain == '$') {
                money++;
            }
            
        }
    }
    if ((start == 0)) {
        std::cerr << "Map does not have a start location\n" << std::flush;
        exit(1);
    }
    if ((money == 0)) {
        std::cerr << "Map does not have a treasure location\n" << std::flush;
        exit(1);
    }
    if ((start > 1)) {
        std::cerr << "Map has more than 1 start location\n" << std::flush;
        exit(1);
    }
    if ((money > 1)) {
        std::cerr << "Map has more than 1 treasure location\n" << std::flush;
        exit(1);
    }
}

// hunt variable that hosts the entire hunt game
Hunt::Hunt() {}

Hunt::Hunt(Mode &captain, Mode &first_mate, std::string hunt_order_input) : 
captain_mode(captain), first_mate_mode(first_mate), huntOrder(hunt_order_input) {
    
}

std::pair<int, int> Hunt::directionOffset(char direction) {
    if (direction == 'N') {
        return {-1, 0};
    }
    else if (direction == 'E') {
        return {0, 1};
    }
    else if (direction == 'S') {
        return {1, 0};
    }
    else if (direction == 'W') {
        return {0, -1};
    }
    else {
        assert(false);
        return {0, 0};
    }
}

// big hunt function. return true if treasure found, false if not
bool Hunt::captain_hunt(Mode &capmode, Mode &matemode, Block current) {
    bool winnerwinner = false;
    while(true) {
        water_investigated++;
        for (char d : huntOrder) {
            std::pair<int, int> offset = directionOffset(d);
            int new_row = current.row + offset.first;
            int new_col = current.col + offset.second;

            if (new_row < 0 || new_col < 0 || new_row >= size || new_col >= size) {continue;}
            
            Block &dis = map[new_row][new_col];

            if (dis.discovered || dis.terrain == '#') {continue;}

            else if (dis.terrain == 'o' || dis.terrain == '$') {
                dis.discovered = true;
                path_track[new_row][new_col] = current; 
                winnerwinner = first_mate_hunt(matemode, dis);
                ashore_list.push_back(dis);
                ashore++;
                if(winnerwinner) {return true;}
            }

            else {
                dis.discovered = true;
                path_track[new_row][new_col] = current;
                captain_search.push_back(dis);
            }
        }
        // if treasure not found, keep on sailing: update the next block
        if (captain_search.empty()) {
            return false;
        }
        if (capmode == Mode::kQueue) {
            current = captain_search.front();
            captain_search.pop_front();
        }
        else {
            current = captain_search.back();
            captain_search.pop_back();
        }
    }
}
// depending on the mode, change pop order from container
// reset the container by removing all the discovered blocks (max4 at a turn), 
// going to control going back to start, starting again from the starting point in another function

bool Hunt::first_mate_hunt(Mode &matemode, Block current) {
    if (current.terrain == '$') {
        land_investigated++;
        TREASURE = current;
        return true;
    }
    while(true) {
        land_investigated++;
        for (char d : huntOrder) {
            std::pair<int, int> offset = directionOffset(d);
            int new_row = current.row + offset.first;
            int new_col = current.col + offset.second;

            if (new_row < 0 || new_col < 0 || new_row >= size || new_col >= size) {continue;}
            
            Block &dis = map[new_row][new_col];

            if (dis.discovered || dis.terrain == '#') {continue;}

            else if (dis.terrain == 'o') {
                dis.discovered = true;
                path_track[new_row][new_col] = current;
                first_mate_search.push_back(dis);
            }

            else if (dis.terrain == '$') {
                dis.discovered = true;
                path_track[new_row][new_col] = current;
                TREASURE = dis;
                land_investigated++;
                return true;
            }

            else {continue;}
        }
        if (first_mate_search.empty()) {
            return false;
        }
        if (matemode == Mode::kQueue) {
            current = first_mate_search.front();
            first_mate_search.pop_front();
        }
        else {
            current = first_mate_search.back();
            first_mate_search.pop_back();
        }
    }
}

Hunt::Block Hunt::get_start() {
    return starting;
}

int Hunt::get_ashore() {
    return ashore;
}

int Hunt::get_path() {
    return path_length;
}

std::pair<int, int> Hunt::get_treasure() {
    return {TREASURE.row, TREASURE.col};
}

Hunt::Block Hunt::Treasure() {
    return TREASURE;
}

int Hunt::getLand() {
    return land_investigated;
}
int Hunt::getWater() {
    return water_investigated;
}

void Hunt::cal_path() {
    std::deque<Block> path = build_path();
    path_length =  static_cast<int>(path.size()) - 1;
}

std::deque<Hunt::Block> Hunt::build_path() {
    std::deque<Block> fullpath; 
    Block curr = TREASURE;
    while(!(curr.row == starting.row && curr.col == starting.col)) {
        fullpath.push_front(curr);
        curr = path_track[curr.row][curr.col];
    }
    fullpath.push_front(starting);
    return fullpath;
}

void Hunt::build_path_map() {
    path_map = map;
    std::deque<Block> path = build_path();
    
    for (size_t i = 1; i + 1 < path.size(); i++) {
        Block &prev = path[i - 1];
        Block &curr = path[i];
        Block &next = path[i + 1];

        bool vert_from = (prev.col == curr.col);
        bool vert_to = (curr.col == next.col);

        if(vert_from && vert_to) {
            path_map[curr.row][curr.col].terrain = '|';
        }
        else if (!vert_from && !vert_to){
            path_map[curr.row][curr.col].terrain = '-';
        }
        else {
            path_map[curr.row][curr.col].terrain = '+';
        }
    }
    path_map[TREASURE.row][TREASURE.col].terrain = 'X';
}

void Hunt::printPath(std::string path_option) {
    if(path_option == "M") {   
        for (int row =0; row <size; row++) {
            for (int col=0; col <size; col++) {
                std::cout << path_map[row][col].terrain;
            }
            std::cout << "\n";
        }
    }
    else {
        std::deque<Block> path = build_path();
        std::deque<std::pair<int, int>> sail_coord;
        std::deque<std::pair<int, int>> search_coord;
        for (size_t i = 0; i < path.size(); i++) {
            if(path[i].terrain == '.' || path[i].terrain == '@'){
                sail_coord.push_back({path[i].row, path[i].col});
            }
            else {
                search_coord.push_back({path[i].row, path[i].col});
            }
        }
        std::cout << "Sail:\n";
        for (size_t i = 0; i < sail_coord.size(); i++) {
            std::cout << sail_coord[i].first << "," << sail_coord[i].second << "\n";
        }
        std::cout << "Search:\n";
        for (size_t i = 0; i < search_coord.size(); i++) {
            std::cout <<  search_coord[i].first << "," << search_coord[i].second << "\n";
        }
    }
}

void Hunt::printStats(bool treasure_found) {
    std::cout << "--- STATS ---\n" <<
    "Starting location: " << starting.row << "," << starting.col << "\n" <<
    "Water locations investigated: " << water_investigated << "\n" <<
    "Land locations investigated: " << land_investigated << "\n" <<
    "Went ashore: " << ashore << "\n";
    if (treasure_found) {
        std::cout << "Path length: " << path_length << "\n" <<
        "Treasure location: " << TREASURE.row << "," << TREASURE.col << "\n";
    }
    std::cout << "--- STATS ---\n";
}

void Hunt::printVerbose(bool treasure_found) {
    std::cout << "Treasure hunt started at: " << starting.row << "," << starting.col << "\n";
    for(size_t i =0; i<ashore_list.size(); i++) {
        std::cout << "Went ashore at: " << ashore_list[i].row << "," << ashore_list[i].col << "\n";
        if (i == ashore_list.size() - 1 && treasure_found) {
            std::cout << "Searching island... party found treasure at " << TREASURE.row << "," << TREASURE.col << ".\n" << std::flush;
            return;
        }
        else {
            std::cout << "Searching island... party returned with no treasure." << "\n";
        }
    }
    std::cout << "Treasure hunt failed\n" << std::flush;
}