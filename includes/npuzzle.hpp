#ifndef NPUZZLE_HPP
# define NPUZZLE_HPP

# include <string>
# include <iostream>
# include <limits>
# include <iterator>
# include <sstream>
# include <algorithm>
# include <set>
# include <unordered_map>
# include <functional>
# include <regex>
# include <array>
# include <cmath>
# include <list>
# include <fstream>
# include <iomanip>

# include <curses.h>

# include "../lib/includes/lib.hpp"

typedef enum {
	A_STAR,
	A_UNIFORM,
	A_GREEDY
} e_algorithm;

typedef enum {
	H_MANHATTAN,
	H_EUCLIDEAN,
	H_MISPLACED
} e_heuristic;

# include "../srcs/objects/State/State.hpp"
class State;

/*****
** Compare function for Queue
*****/
struct compare {
    bool operator() (State *a, State *b) const;
};

/*****
** Target Type Enumeration
*****/
enum e_target {
    T_SNAIL,
    T_ORDERED,
    T_RANDOM,
    T_FILE
};

/*****
** Options structure
*****/
typedef struct s_puzzleOptions {
    e_algorithm algorithm;              // Algorithm selected
    e_heuristic heuristic;              // Heuristic selected
    bool        generation;             // Is Generated
    int         generation_length;      // Generation length
    int         generation_iteration;   // Generation iteration
    e_target    target_type;            // Type of Target
    std::string target_file;            // File if Target Type = T_FILE
    bool        visualizer;             // Is visualized
} puzzleOptions;

/*****
** Informations structure
*****/
typedef struct s_puzzleInfos {
    int                 state_selected;     // Number of state selected
    int                 max_state_memory;   // Number of state maximum in memory
    int                 number_steps;       // Number of steps to get from Start to Target
    std::list<State*>   steps;              // List of steps to get from Start to Target
    bool                initalize;          // Is Puzzle initialized
    int                 length;             // Length of Puzzle
    int                 size;               // Size of Puzzle (length * length)
} puzzleInfos;

# include "../srcs/objects/Visualizer/Visualizer.hpp"
class Visualizer;

# include "../srcs/objects/Puzzle/Puzzle.hpp"
class Puzzle;

#endif