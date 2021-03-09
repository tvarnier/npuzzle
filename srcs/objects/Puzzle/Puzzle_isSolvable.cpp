#include "Puzzle.hpp"

/*****
** Row of Zero position starting from bottom
**
** @param zero_pos[in]  : Position of zero in Start
** @param length[in]    : Length of the Puzzle
**
** @return              : Row of Zero position starting from bottom
*****/
static int  zero_row_from_botom(const int& zero_pos, const int& length)
{
    return (length - (zero_pos / length));
}

/*****
** Check if 2 Piece from Start Array are in an inverted order compared to Target Array
**
** @param target[in]    : Target Array
** @param nbr1[in]      : Value of First Piece
** @param nbr2[in]      : Value of Second Piece (nbr1_pos < nbr2_pos)
** @param size[in]      : Size of the Puzzle
**
** @return              : True if pieces are inverted, else False
*****/
static bool is_inverted(const int *target, const int& nbr1, const int& nbr2, const int& size)
{
    int     nbr1_pos(0);
    int     nbr2_pos(0);

    for (int i = 0; i < size; ++i)  // Find position of nbr1 in Target Array
        if (target[i] == nbr1)
        {
            nbr1_pos = i;
            break ;
        }
    for (int i = 0; i < size; ++i)  // Find position of nbr2 in Target Array
        if (target[i] == nbr2)
        {
            nbr2_pos = i;
            break ;
        }
    return (nbr2_pos < nbr1_pos);   // True if nbr1_pos is inverted from nbr2_pos, Else False
}

/*****
** Get Inversion number from Start Array and Target Array
**
** @param start[in]     : Start Array
** @param target[in]      : Target Array
** @param size[in]      : Size of the Puzzle
**
** @return              : True if pieces are inverted, else False
*****/
static int  inversion_number(const int *start, const int *target, const int& size)
{
    int     inv_nbr(0);

    for (int i = 0; i < size; ++i)      // For Each Piece (Except 0 Value), check if following ones are in inverted order
        if (start[i] != 0)
            for (int j = i + 1; j < size; ++j)
                if (start[j] != 0 && is_inverted(target, start[i], start[j], size))
                    ++inv_nbr;
    return (inv_nbr);                   // Return Inversion number;
}

/*****
** Check if Puzzle is solvable
**
** @return      : True if solvable, else False
*****/
bool    Puzzle::isSolvable()
{
    if (m_infos.initalize == false)     // If Puzzle isn't initialize, Return error
    {
        lib::printerr(RED, "ERROR : The puzzle isn't initialize");
        return (0);
    }

    if (m_infos.length % 2 == 1) // If Length of Puzzle odd
    {
        if (inversion_number(m_start->getArray(), m_target->getArray(), m_infos.size) % 2 == 0) // If Inversion number is even, Puzzle is Solvable
            return (true);
    }
    else if ((m_target->getZeroPosition() / m_infos.length) % 2 == 0)   // Else if length of Puzzle even and Row of Zero position even (beginning from 0)
    {
        // if Zero position from bottom and inversion number are odd OR Zero position from bottom and inversion number are even, Puzzle is solvable
        if ((zero_row_from_botom(m_start->getZeroPosition(), m_infos.length) % 2 == 0 && inversion_number(m_start->getArray(), m_target->getArray(), m_infos.size) % 2 == 0)
            || (zero_row_from_botom(m_start->getZeroPosition(), m_infos.length) % 2 == 1 && inversion_number(m_start->getArray(), m_target->getArray(), m_infos.size) % 2 == 1))
            return (true);
    }
    else
    {
        // if Zero position from bottom is odd and inversion number is even OR Zero position from bottom is even and inversion number is odd, Puzzle is solvable
        if ((zero_row_from_botom(m_start->getZeroPosition(), m_infos.length) % 2 == 0 && inversion_number(m_start->getArray(), m_target->getArray(), m_infos.size) % 2 == 1)
            || (zero_row_from_botom(m_start->getZeroPosition(), m_infos.length) % 2 == 1 && inversion_number(m_start->getArray(), m_target->getArray(), m_infos.size) % 2 == 0))
            return (true);
    }

    return (false); // Puzzle is not solvable
}