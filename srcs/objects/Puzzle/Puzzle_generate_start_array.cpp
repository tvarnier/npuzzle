#include "Puzzle.hpp"

#include <stdlib.h>
#include <time.h>

/*****
** Generate random number
**
** @param from[in]  : Minimum number generated
** @param to[in]    : Maximum number generated
**
** @return          : Number generated between [from, to]
*****/
static int     generate_number(const int& from, const int& to)
{
    return ((rand() % (to - from + 1)) + from);
}

/*****
** Count the number of Move possible
**
** @param zero[in]      : Position of the zero
** @param last[in]      : Last move done
** @param length[in]    : Length of the array
**
** @return              : Number of move possible
*****/
static int     count_possibities(const int& zero, const int& last, const int& length)
{
    int     count(0);
    if (last != +length && zero >= length) ++count;             // Check UP
    if (last != -length && zero / length < length - 1) ++count; // Check DOWN
    if (last != +1 && zero % length > 0) ++count;               // Check LEFT
    if (last != -1 && zero % length < length - 1) ++count;      // Check RIGHT
    return (count);
}

/*****
** Swap values in an array with the zero
**
** @param start[in, out]    : Array to change
** @param zero[in, out]     : Position of the zero
** @param last[in, out]     : Last move done (UP -Length, DOWN +Length, LEFT -1, RIGHT +1)
** @param where[in]         : Diretion of the move (UP -Length, DOWN +Length, LEFT -1, RIGHT +1)
*****/
void    swap_values(int *start, int& zero, int& last, const int& where)
{
    start[zero] = start[zero + where];  // Change Zero Position value with the new value
	start[zero + where] = 0;            // Change value of the new Zero Position to 0
	zero = zero + where;                // Change Zero Position
    last = where;                       // Change Last move
}

/*****
** Generate Random Start Array
**
** @param start[in, out]    : Start array to modify (already allocated)
** @param target[in]        : Base the generate from Target to generate a solvable solution
**
** @return                  : Generated Start Array
*****/
int     *Puzzle::generate_start_array(int *start, const int *target)
{
    int     zero(0);
    int     possibilities(0);
    int     rdm;
    int     last(0);

    for (int i = 0; i < m_infos.size; ++i) start[i] = target[i];                                        // Copy Content of Target to Start
    zero = m_target->getZeroPosition();                                                                 // Get zero position from Target's zero position

    srand (time(NULL));                                                                                 // Inialize Rand

    for (int i = m_options.generation_iteration; i > 0; --i)                                            // While Iterations, make a move
    {
        possibilities = count_possibities(zero, last, m_infos.length);                                  // Count Number of possibilities
        rdm = generate_number(1, possibilities);                                                        // Choose Move Randomly from possiblitites
        if (last != +m_infos.length && zero >= m_infos.length && !(--rdm))                              // Move UP
            swap_values(start, zero, last, -m_infos.length);
        else if (last != -m_infos.length && zero / m_infos.length < m_infos.length - 1 && !(--rdm))     // Move DOWN
            swap_values(start, zero, last, +m_infos.length);
        else if (last != +1 && zero % m_infos.length > 0 && !(--rdm))                                   // Move LEFT
            swap_values(start, zero, last, -1);
        else if (last != -1 && zero % m_infos.length < m_infos.length - 1 && !(--rdm))                  // Move RIGHT
            swap_values(start, zero, last, +1);
    }
    return (start);                                                                                     // Return Start Array
}
