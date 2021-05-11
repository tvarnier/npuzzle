#include "Puzzle.hpp"

/*****
** Create State from Current State
**
** @param current[in]       : Current State selected
** @param piece[in]         : Piece to move
** @param target[in]        : Target State
** @param heuristic[in]     : Heuristic used
** @param algorithm[in]     : Algorithm used
**
** @return                  : Return Child
*****/
static State    *generate_child(const State *current, int piece, const int *target, const e_heuristic& heuristic, const e_algorithm& algorithm)
{
    State   *child = new State(*current);               // Create Child from Current
    int     zero_position = current->getZeroPosition();

    child->swapZeroPosition(piece);                     // Swap Zero Position with target Piece
    child->setNbrMoves(current->getNbrMoves() + 1);     // Add one more move than Current
    child->setHash();                                   // Set Hash from Array
    child->updateScore(zero_position, target, heuristic, algorithm); // Update Score depending on Algorithm and Heuristic
    child->setParent((State*)current);                  // Set Parent of Child to Current

    return (child);
}

/*****
** Create Successor State and Manage it
**
** @param current[in]       : Current State selected
** @param piece[in]         : Piece to move
*****/
void    Puzzle::manage_sucessor(const State *current, int piece)
{
    std::unordered_map<size_t, State*>::iterator    it;
    std::set<State*, compare>::iterator             it_queue;
    State                                           *child;

    child = generate_child(current, piece, m_target->getArray(), m_options.heuristic, m_options.algorithm);   // Generate Successor

    if ((it = m_list.find(child->getHash())) == m_list.end())   // if no similar State in List, Insert in Queue and List
    {
        m_queue.insert(child);
        m_list[child->getHash()] = child;
    }
    else                                                        // Else if similar State in List
    {
        if (child->getScore() < it->second->getScore())         //      If Successor have smaller score than similar one (better)
        {
            if (!(it->second->isDone()) && (it_queue = m_queue.find(it->second)) != m_queue.end())  // If Similar one in Queue, Erase it
                m_queue.erase(it_queue);
            *(it->second) = *child;                             //          Replace Similar content with Successor
            m_queue.insert(it->second);                         //          Reinsert Similar with successor content in Queue
        }
        delete (child);
    }
}

/*****
** Try to Create Successor in all 4 directions
**
** @param current[in]       : Current State selected
*****/
void    Puzzle::generate_successors(const State *current)
{
    int     zero_position(current->getZeroPosition());

    // Up
    if (zero_position >= m_infos.length)
        manage_sucessor(current, zero_position - m_infos.length);
    // Down
    if (zero_position / m_infos.length < m_infos.length - 1)
        manage_sucessor(current, zero_position + m_infos.length);
    // Left
    if (zero_position % m_infos.length > 0)
        manage_sucessor(current, zero_position - 1);
    // Right
    if (zero_position % m_infos.length < m_infos.length - 1)
        manage_sucessor(current, zero_position + 1);
}

/*****
** Solve Puzzle
**
** @return      : 0 If Puzzle solved, else 1
*****/
int         Puzzle::solve()
{
    State   *current;

    if (m_infos.initalize == false)     // If Puzzle not initialized, return Error
        return (lib::printerr(RED, "ERROR : The puzzle isn't initialize"));

    if ((*m_start) == (*m_target))      // If Puzzle is already solved, return Error
        return (lib::printerr(RED, "ERROR : The puzzle is already solved"));

    m_queue.insert(new State(*m_start));                // Insert Start State in Queue
    m_list[m_start->getHash()] = *(m_queue.begin());    // Insert Start in List
    while (!m_queue.empty())                // While States not analized in Queue
    {
        ++m_infos.state_selected;           // Add one to counter of State selected
        current = *(m_queue.begin());       // Get first element of Queue (smaller score)
        m_queue.erase(m_queue.begin());     // Remove first element of Queue
        current->setDone();                 // Set State as analized

        if (*current == *m_target)          // If State selected is Target State, Win
        {
            sucess(current);
            return (0);
        }
        else                                // Else if isn't Target, generate Successors
            generate_successors(current);
    }
    lib::printendl(BOLD, RED, "(Fail)");
    return (1);
}