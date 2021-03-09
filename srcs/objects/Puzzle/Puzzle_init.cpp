#include "Puzzle.hpp"

#include <stdlib.h>
#include <time.h>

/*****
** Regex Functions to determine string type (comment, length, row, row of a precise length)
**
** @param line[in]      : String to analyze
** @param length[in]    : Puzzle Length
**
** @return              : True if string correspond else false
*****/
static bool regex_comment(const string& line) { return (std::regex_search(line, std::regex("^[\t ]*($|[#])"))); }   // Is the line a comment or a blank line
static bool regex_length(const string& line) { return (std::regex_search(line, std::regex("^[\t ]*([+]|[-])?[0-9]+[\t ]*($|[#])"))); }  // Is the line a length
static bool regex_row(const string& line) { return (std::regex_search(line, std::regex("^[\t ]*(([+]|[-])?[0-9]+)([\t ]+(([+]|[-])?[0-9]+))+[\t ]*($|[#])"))); }    // Is the line a row
static bool regex_row_wlength(const string& line, const int& length) { return (std::regex_search(line, std::regex("^[\t ]*(([+]|[-])?[0-9]+)([\t ]+(([+]|[-])?[0-9]+)){" + std::to_string(length - 1) + "}[\t ]*($|[#])"))); } // Is the line a row of size <length>

/*****
** Generate random number
**
** @param from[in]  : Minimum number generated
** @param to[in]    : Maximum number generated
**
** @return          : 0 if everything goes well then 1 if error
*****/
static int     generate_number(const int& from, const int& to)
{
    return ((rand() % (to - from + 1)) + from);
}

/*****
** Get Length from Input Stream
**
** @param strm[in, out]         : Input Stream to read from
** @param length[out]           : Length variable to determine
** @param line_count[in, out]   : Number of line read
**
** @return                      : False if Length found, else True id Error
*****/
static bool get_length(std::istream& strm, int& length, int& line_count)
{
    std::string     line;

    while (std::getline(strm, line))                        // While can read line from Input Stream
    {
        ++line_count;                                       // Add a line to count
        if (regex_length(line))                             // If Line is a length, set Length and end function normally
        {
           length = std::atoi(line.c_str());
            return (false);
        }
        else if (regex_row(line))                           // Else If Line is a row, return Error
            return (lib::printerr(RED, "ERROR: No Size"));
        else if (!regex_comment(line))                      // Else If line isn't a comment, return Error
            return (lib::printerr(RED, "ERROR: Line ", line_count, " : \"", line, "\" : Expect Size"));
        
    }
    return (lib::printerr(RED, "ERROR: file is empty"));    // If No Length found, return Error
}

/*****
** Get Array from Input Stream
**
** @param strm[in, out]             : Input Stream to read from
** @param array[out]                : Array to modify (Already allocated)
** @param line_count[in, outout]    : Number of line read
** @param length[in]                : Length of the Puzzle
**
** @return                          : False if everything goes well then True if error
*****/
static bool get_array(std::istream& strm, int *array, int& line_count, const int& length)
{
    std::string     line;
    int             row_count(0);

    while (std::getline(strm, line))                            // While can read line from Input Stream
    {
        ++line_count;                                           // Add a line to count
        if (regex_row(line))                                    // If Line is a row
        {
            if (row_count >= length)                            //  If number Row greater than Length, return Error
                return (lib::printerr(RED, "ERROR: Too much rows row(s) : Size is ", length));
            if (!regex_row_wlength(line, length))               //  If number of element in Row different than Length, return Error
                return (lib::printerr(RED, "ERROR: Line ", line_count, " : \"", line, "\" : Number of columns incorrect, Size is ", length));
            std::stringstream   line_strm(line);
            for (int i = 0; i < length; ++i)                    //  Insert Values of the line in Array
                line_strm >> array[row_count * length + i];
            ++row_count;                                        //  Add a row to count
        }
        else if (!regex_comment(line))                          // Else if line isn't a Comment as well, return Error
            return (lib::printerr(RED, "ERROR: Line ", line_count, " : \"", line, "\" (Expected Row)"));
    }
    if (row_count < length)                                     // If Number of row smaller than Length, return Error
        return (lib::printerr(RED, "ERROR: Missing row(s), Only ", row_count, " row(s) : Size is ", length));
    return (false);                                             // Return False, everything goes well
}

/*****
** Check if Array correspond to a puzzle
**
** @param array[in] : Array to modify (Already allocated)
** @param size[in]  : Size of the Puzzle
**
** @return          : False if everything is fine, else return True if incorrect Array (Error)
*****/
static bool     verif_array(const int *array, const int& size)
{
    bool    *verif = new bool[size];                                // Allocate an array of bool for each piece of Puzzle

    for (int i = 0; i < size; ++i) verif[i] = false;                // Initialize the Array of bool to false

    for (int i = 0; i < size; ++i)                                  // For each piece
    {
        if (array[i] >= 0 && array[i] < size && !verif[array[i]])   // If Piece correspond to range of piece and isn't already found, set corresponding bool to true in verif array
            verif[array[i]] = true;
        else if (verif[array[i]])                                   // Else if piece already found, return Error
        {
            delete (verif);
            return (lib::printerr(RED, "ERROR: Value '", array[i], "' is used at least twice"));
        }
        else if (array[i] < 0)                                      // Else if piece smaller than range of number, return Error
        {
            delete (verif);
            return (lib::printerr(RED, "ERROR: Value '", array[i], "' is too small, must be positive"));
        }
        else                                                        // Else Piece greater than range of number, return Error
        {
            delete (verif);
            return (lib::printerr(RED, "ERROR: Value '", array[i], "' is too high, must be strictly smaller than ", size, " (Size * Size)"));
        }
    }

    for (int i = 0; i < size; ++i) if (!verif[i]) { delete (verif); return (lib::printerr(RED, "ERROR: Missing Value '", i, "'")); }    // Return Error if missing a value

    delete (verif);
    return (false);                                                 // Return False, everything goes well
}

/*****
** Intialize Target Array to Snail form
**
** @param target[out]   : Target array to modify (Already allocated)
*****/
void    Puzzle::init_target_snail(int *target)
{
    int     row(0);
    int     row_end(m_infos.length - 1);
    int     col(0);
    int     col_end(m_infos.length - 1);
    int     i;
    int     nbr(0);

    while (row <= row_end && col <= col_end)                // Initialize Target array to snail form
    {
        for (i = col; i <= col_end; ++i)
            target[row * m_infos.length + i] = ++nbr;
        ++row;
        for (i = row; i <= row_end; ++i)
            target[i * m_infos.length + col_end] = ++nbr;
        --col_end;
        for (i = col_end; i >= col; --i)
            target[row_end * m_infos.length + i] = ++nbr;
        --row_end;
        for (i = row_end; i >= row; --i)
            target[i * m_infos.length + col] = ++nbr;
        ++col;
    }
    if (m_infos.length % 2 == 1)                            // Get Zero position correponding to Puzzle length
        m_target->setZeroPosition(((m_infos.length / 2) * m_infos.length) + (m_infos.length / 2));
    else
        m_target->setZeroPosition(((m_infos.length / 2) * m_infos.length) + (m_infos.length / 2 - 1));
    target[m_target->getZeroPosition()] = 0;                // Change Zero position value to 0
}

/*****
** Intialize Target Array to Ordered form
**
** @param target[out]   : Target array to modify (Already allocated)
*****/
void    Puzzle::init_target_ordered(int *target)
{
    for (int i = 0; i < m_infos.size; ++i) target[i] = i + 1;   // Initialize Target Array to ordered form
    target[m_infos.size - 1] = 0;                               // Set last position to zero
    m_target->setZeroPosition(m_infos.size - 1);                // Set Zero position to last one
}

/*****
** Intialize Target Array to Random form
**
** @param target[out]   : Target array to modify (Already allocated)
*****/
void    Puzzle::init_target_random(int *target)
{
    int     rdm(0);
    int     count(0);
    int     i(0);
    int     y(0);

    for (i = 0; i < m_infos.size; ++i) target[i] = -1;        // Intialize Array values to -1
    srand (time(NULL));                                     // Initialize Rand
    for (i = m_infos.size - 1; i >= 0; --i)                 // For each values to insert
    {
        rdm = (i != 0) ? generate_number(0, i) : 0;         // Determine random position from left ones
        count = 0;
        for (y = 0; y < m_infos.size; ++y)                  // Find the position and set the value to this one
        {
            if (target[y] == -1 && count == rdm)
            {
                target[y] = i;
                break;
            }
            if (target[y] == -1)
                count++;
        }
    }
    for (int i = 0; i < m_infos.size; ++i) if (target[i] == 0) { m_target->setZeroPosition(i); break ; }    // Find 0 position and set it to Target
}

/*****
** Intialize Target Array from file
**
** @param target[out]   : Target array to modify (Already allocated)
*****/
int     Puzzle::init_target_file(int *target)
{
    std::ifstream   strm(m_options.target_file);                  // Iniatlize Input Stream from file
    int     line_count(-1);
    int     length(0);

    if (strm.fail())                                            // If find doesn't open, return Error
        return (lib::printerr(RED, "ERROR: Target file \"", m_options.target_file, "\" could't be open (not existing or failed to open)"));
    if (get_length(strm, length, line_count))                   // Get length, else return Error
        return (1);
    if (m_infos.length != length)                               // If Length doesn't correspond to actual Puzzle Length, return Error
        return (lib::printerr(RED, "ERROR: Target Size is different from Start"));
    if (get_array(strm, target, line_count, m_infos.length))    // Get Target Array, else return Error
        return (1);
    strm.close();                                               // Close Input Stream
    for (int i = 0; i < m_infos.size; ++i) if (target[i] == 0) { m_target->setZeroPosition(i); break ; }    // Find 0 position and set it to Target
    if (verif_array(target, m_infos.size))                      // Check if Array correspond to a correct puzzle
        return (1);
    return (0);
}

/*****
** Intialize Target
*****/
int     Puzzle::init_target()
{
    int     *target;

    target = new int[m_infos.size];                 // Allocate Array

    if (m_options.target_type == T_SNAIL)           // If Target Form is SNAIL, init to SNAIL one
        init_target_snail(target);
    else if (m_options.target_type == T_ORDERED)    // If Target Form is ORDERED, init to ORDERED one
        init_target_ordered(target);
    else if (m_options.target_type == T_RANDOM)     // If Target Form is RANDOM, init to RANDOM one
        init_target_random(target);
    else if (m_options.target_type == T_FILE)       // If Target Form is determine from a file, init from it
    {
        if (init_target_file(target))               //  If Error from init, Return Error
        {
            delete (target);
            return (1);
        }
    }
    m_target->setArray(target);                     // Set Target Array, from created array
    m_target->setNbrMoves(0);                       // Set Target Number of Move to 0
    m_target->setHash();                            // Create Hash for Target
    m_target->setScore(m_target->getArray(), m_options.heuristic, m_options.algorithm); // Set score of Target corresponding to Heuristic and Algorithm
    return (0);
}

/*****
** Intialize Start
**
** @param target[in]            : Target array
** @param line_count[in, out]   : Number of line read
*****/
int     Puzzle::init_start(const int *target, int& line_count)
{
	int     *start;
    start = new int[m_infos.size];                  // Allocate array

    if (m_options.generation)                       // If Generate Option, generate array from Target array
        Puzzle::generate_start_array(start, target);
    else if (get_array(std::cin, start, line_count, m_infos.length))    // Else Get array from stdin
    {
        delete (start);
        return (1);
    }
    m_start->setArray(start);                       // Set Start Array, from created array
    m_start->setNbrMoves(0);                        // Set Start Number of moves to 0
    m_start->setHash();                             // Create Hash for Start
    m_start->setScore(target, m_options.heuristic, m_options.algorithm);  // Set score of Start corresponding to Heuristic and Algorithm
    for (int i = 0; i < m_infos.size; ++i) if (start[i] == 0) { m_start->setZeroPosition(i); break ; }  // Set Zero position to Start
    return (0);
}

/*****
** Intialize Puzzle
**
** @return      : 1 if Error, else 0
*****/
int     Puzzle::init()
{
    int     line_count(-1);
    string  line;

    m_infos.initalize = false;                          // Set Initialize boolean to false
    if (!m_options.generation && get_length(std::cin, m_infos.length, line_count))  // If Generate option false, get Length from stdin
        return (1);
    else if (m_options.generation)                      // Else if generate option true, get Length from generated length option
        m_infos.length = m_options.generation_length;
    if (m_infos.length <= 2 || m_infos.length > 256)    // If Length smaller than 3 and greater than 256, return Error
        return (lib::printerr(RED, "ERROR: Size must be greater strictly greater than 2 and smaller than 256"));
    m_start->setLength(m_infos.length);                 // Set Length of Start
    m_target->setLength(m_infos.length);                // Set Lenght of Target
    m_infos.size = m_infos.length * m_infos.length;     // Set Size of Puzzle
    if (init_target())                                  // Initialize Target
        return (1);
    if (init_start(m_target->getArray(), line_count))   // Initialize Start
        return (1);
    if (verif_array(m_start->getArray(), m_infos.size)) // Check if Array correspond to a puzzle
        return (1);
    m_infos.initalize = true;                           // Set Initialize boolean to true
    return (0);
}

