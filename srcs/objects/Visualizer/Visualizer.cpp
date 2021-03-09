# include "Visualizer.hpp"

Visualizer::Visualizer(puzzleInfos infos) : m_infos(infos), m_length(infos.length)
{
	int		div(1);

    m_nbr_digit = 1;
	while (infos.length * infos.length / div >= 10)
	{
		div *= 10;
		m_nbr_digit++;
	}

    m_current = m_infos.steps.begin();
}

// ----- PUBLIC FUNCTIONS ----- //

int     Visualizer::launch()
{
    init();

    print_array();
    while (m_running)
    {
        key_hook();
    }

    delwin(m_win);
    endwin();

    return (0);
}

// ----- PRIVATE FUNCTIONS ----- //

void    Visualizer::init()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    m_running = true;

    m_width = size.ws_col;
    m_height = size.ws_row;

    initscr();

    m_win = newwin(m_height, m_width, 0, 0);
    noecho();
    cbreak();
    nodelay(m_win, TRUE);
    box(m_win, 0, 0);
    wrefresh(m_win);
}

void    Visualizer::print_array()
{
    int     *array;

    array = (*m_current)->getArray();

    for (int i = 0; i < m_length; ++i)
    {
        move_to((m_width - ((m_length + 1 ) * m_nbr_digit - 1)) / 5, (m_height - m_length * 2) / 5 + i * 2);
        for (int y = 0; y < m_length; ++y)
        {
            if (array[i * m_length + y])
                wprintw(m_win, "%0*d  ", m_nbr_digit, array[i * m_length + y]);
            else
                wprintw(m_win, "%*s  ", m_nbr_digit, " ");
        }
    }
    wrefresh(m_win);
}

void    Visualizer::key_hook()
{
    int     key;

    if ((key = wgetch(m_win)) == ERR)
        return ;

    wprintw(m_win, " :: %c ", key);
    wrefresh(m_win);
    switch (key)
    {
        case 'q' : // QUIT
            m_running = false;
            break ;
        case 67 : // RIGHT ARROW
            ++m_current;
            if (m_current == m_infos.steps.end())
                --m_current;
            print_array();
            break ;
        case 68 : // LEFT ARROW
            if (m_current != m_infos.steps.begin())
                --m_current;
            print_array();
            break ;
        default :
            break ;
    }
}

void    Visualizer::move_to(int x, int y)
{
    m_cursor.x = x;
    m_cursor.y = y;
    wmove(m_win, y, x);
}