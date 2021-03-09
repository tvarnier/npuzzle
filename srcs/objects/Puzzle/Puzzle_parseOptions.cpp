#include "Puzzle.hpp"

/*****
** Print Usage of the program
**
** @return      : 1
*****/
static int  print_program_usage()
{
    return (lib::printerr("usage: ./name_program [-h] [-alg <SEARCH_ALGORITHM>] [-hr <HEURISTIC>] [-gen <GENERATION_LENGTH> [<GENERATION_ITERATION>]] [-tgt <TARGET_TYPE or TARGET_FILE>] [-visu]\n",
                            "\n",
                            "optional arguments:\n",
                            "  -h, --help           show this help message\n",
                            "  -alg <SEARCH_ALGORITHM>, --algorithm <SEARCH_ALGORITHM>\n",
                            "                       <SEARCH_ALGORITHM> available (astar, uniform, greedy), default astar\n",
                            "  -hr <HEURISTIC>, --heuristic <HEURISTIC>\n",
                            "                       <HEURISTIC> available (manhattan, euclidean, misplaced), default manahattan\n",
                            "  -gen <GENERATION_LENGTH> [<GENERATION_ITERATION>], --generation <GENERATION_LENGTH. [<GENERATION_ITERATION>]\n",
                            "                       <GENERATION_LENGTH> must be greater than 2 and smaller or equal than 256\n",
                            "                       <GENERATION_ITERATION> to determine the number of iterations used in generation, default 10000\n",
                            "  -tgt <TARGET_TYPE or TARGET_FILE>, --target <TARGET_TYPE or TARGET_FILE>\n",
                            "                       <TARGET_TYPE> to define target (snail, ordered, random), default snail\n",
                            "                       <TARGET_FILE> to define target with a file\n",
                            "  -visu, --visualizer   show animated path"));
}

/*****
** Check if a string is a Number
**
** @param s[in] : String to check
**
** @return      : True if is a represent a number, Else False
*****/
static bool is_number(char *s) { int i; for (i = 0; s[i]; ++i) if (!std::isdigit(s[i])) return (false); return (i > 0); }

/*****
** Parse arguments of the program and Modify Puzzle informations
**
** @param ac[in]    : Number of argument of the program
** @param av[in]    : Argument of the program
**
** @return          : Return 1 if Error in parsing, Else 0
*****/
int     Puzzle::parseOptions(const int& ac, char **av)
{
    for (int i = 1; i < ac; ++i)
    {
        if (!std::strcmp(av[i], "-h") || !std::strcmp(av[i], "--help"))                 // HELP
            return (print_program_usage());
        else if (!std::strcmp(av[i], "-alg") || !std::strcmp(av[i], "--algorithm"))     // ALGORITHM
        {
            ++i;
            if (i < ac && !std::strcmp(av[i], "astar"))
                Puzzle::setAlgorithm(A_STAR);
            else if (i < ac && !std::strcmp(av[i], "uniform"))
                Puzzle::setAlgorithm(A_UNIFORM);
            else if (i < ac && !std::strcmp(av[i], "greedy"))
                Puzzle::setAlgorithm(A_GREEDY);
            else
                return (print_program_usage());
        }
        else if (!std::strcmp(av[i], "-hr") || !std::strcmp(av[i], "--heuristic"))      // HEURISTIC
        {
            ++i;
            if (i < ac && !std::strcmp(av[i], "manhattan"))
                Puzzle::setHeuristic(H_MANHATTAN);
            else if (i < ac && !std::strcmp(av[i], "euclidean"))
                Puzzle::setHeuristic(H_EUCLIDEAN);
            else if (i < ac && !std::strcmp(av[i], "misplaced"))
                Puzzle::setHeuristic(H_MISPLACED);
            else
                return (print_program_usage());
        }
        else if (!std::strcmp(av[i], "-gen") || !std::strcmp(av[i], "--generation"))    // GENEREATIO
        {
            ++i;
            if (i < ac && is_number(av[i]))
            {
                if (i + 1 < ac && is_number(av[i + 1]))
                {
                    Puzzle::setGeneration(true, atoi(av[i]), atoi(av[i + 1]));
                    ++i;
                }
                else
                    Puzzle::setGeneration(true, atoi(av[i]));
            }
            else
                return (print_program_usage());
        }
        else if (!std::strcmp(av[i], "-tgt") || !std::strcmp(av[i], "--target"))        // TARGET
        {
            ++i;
            if (i < ac && !std::strcmp(av[i], "snail"))
                Puzzle::setTarget(T_SNAIL, "");
            else if (i < ac && !std::strcmp(av[i], "ordered"))
                Puzzle::setTarget(T_ORDERED, "");
            else if (i < ac && !std::strcmp(av[i], "random"))
                Puzzle::setTarget(T_RANDOM, "");
            else if (i < ac && std::strlen(av[i]) > 0 && av[i][0] != '-')
                Puzzle::setTarget(T_FILE, av[i]);
            else
                return (print_program_usage());
        }
        else if (!std::strcmp(av[i], "-visu") || !std::strcmp(av[i], "--visualizer"))   // VISUALIZER
            Puzzle::setVisualizer(true);
        else
            return (print_program_usage());
    }
    return (0);
}