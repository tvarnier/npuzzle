#include "Puzzle.hpp"

/*****
** Generate steps
**
** @param current[in]       : Current State selected
** @param steps[out]        : List of Steps required to get from Start to Target
** @param number_steps[out] : Number of Steps required to get from Start to Target
*****/
static void generate_steps(State *current, std::list<State*>& steps, int& number_steps)
{
    State   *tmp;

    tmp  = current;

    while (tmp != nullptr)              // From Last State, Insert them in Steps List
    {
        steps.push_front(tmp);
        tmp = tmp->getParent();
    }
    number_steps = steps.size() - 1;    // Set Number of steps
}

void    Puzzle::sucess( State *current )
{
    m_infos.max_state_memory = m_list.size();   // Get maximum number of States in memory
    generate_steps(current, m_infos.steps, m_infos.number_steps);   // Get Steps to get from Start to Target

    lib::printendl(BOLD, GREEN, "----- WIN -----");
    lib::printendl("Complexity in Time :: ", m_infos.state_selected);
    lib::printendl("Complexity in Size :: ", m_infos.max_state_memory);
    lib::printendl("Number of Moves    :: ", m_infos.number_steps);
    current->print();

    ofstream myfile;
    myfile.open ("results.txt");
    if (myfile.is_open())
    {
        myfile << "=====  Start  =====" << endl;
        myfile << m_start->array_to_string();
        myfile << "=====  Target =====" << endl;
        myfile << m_target->array_to_string();
        myfile << "Complexity in Time :: " << m_infos.state_selected << endl;
        myfile << "Complexity in Size :: " << m_infos.max_state_memory << endl;
        myfile << "Number of Moves    :: " << m_infos.number_steps << endl;
        
        int		nbr_digit(1);
        int		div(1);
        while (m_infos.length * m_infos.length / div >= 10)
        {
            div *= 10;
            nbr_digit++;
        }

        for ( std::list<State*>::iterator it = m_infos.steps.begin(); it != m_infos.steps.end(); ++it )
        {
            myfile << std::string((nbr_digit + 1) * m_infos.length, '=') << endl;
            myfile << (*it)->array_to_string();
        }
        myfile.close();
        lib::printendl(GREY, " >> Steps and more in file \"results.txt\"", RESET);
    }

    if (m_options.visualizer)
        launchVisualizer();
}