#include "npuzzle.hpp"

int		main(int ac, char **av)
{
	Puzzle	puzzle;

	if (puzzle.parseOptions(ac, av))
		return (1);

	if (puzzle.init())
		return (1);

	lib::printendl(BOLD, "=====  Start =====");
	puzzle.getStart()->print();

	lib::printendl(BOLD, "=====  Target =====");
	puzzle.getTarget()->print();

	if (puzzle.isSolvable())
		puzzle.solve();
	else
		lib::printendl(RED, "NOT SOLVABLE");

	return (0);
}