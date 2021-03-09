#ifndef VISUALIZER_HPP
# define VISUALIZER_HPP

# include "npuzzle.hpp"

# include <sys/ioctl.h>
# include <unistd.h>

typedef struct s_cursor {
    int     x = 0;
    int     y = 0;
} t_cursor;

class Visualizer {

    public :
        Visualizer(puzzleInfos infos);

        int     launch();

    private :
        puzzleInfos                 m_infos;
        t_cursor                    m_cursor;
        std::list<State*>::iterator m_current;
        int                         m_length;
        int                         m_nbr_digit;
        int                         m_width;
        int                         m_height;
        WINDOW                      *m_win;
        bool                        m_running;

        void        init();
        void        print_array();
        void        key_hook();
        void        move_to(int x, int y);
};

#endif