#ifndef STATE_HPP
# define STATE_HPP

# include "npuzzle.hpp"

class State
{
	public :
		State();					// Constructor
		State(const State& other);	// Constructor
		~State();					// Destructor

		void	print() const; // Print Array

		void	swapZeroPosition(const int& piece);	// Swap any piece with Zero

		std::string		array_to_string() const;	// Get string to print array

		// ----- Encapsulation ----- //

		int		*getArray() const;
		void	setArray(int *array);

		int		getLength() const;
		void	setLength(const int& length);

		int		getZeroPosition() const;
		void	setZeroPosition(const int& zero_position);

		int		getScore() const;
		void	setScore(const int *target, const e_heuristic& h = H_MANHATTAN, const e_algorithm& a = A_STAR);

		int		getNbrMoves() const;
		void	setNbrMoves(const int& nbr_moves);

		size_t	getHash() const;
		void	setHash();

		State	*getParent() const;
		void	setParent(State *parent);

		bool	isDone() const;
		void	setDone();

		// ----- Operator Overloading ----- //

		State	&operator=(const State& other);

	private :
		int				*m_array;			// Array of the State
		int				m_length;			// Length of the Puzzle
		int				m_zeroPosition;		// Zero position of the State
		int				m_score;			// Score of the State
		int				m_nbrMoves;			// Number of moves from Start State
		size_t 			m_hash;				// Hash of the Array
		State			*m_parent;			// Parent of the State
		bool			m_done;				// Is State analized

		size_t			hashArray();				// Get Hash from Array

		// ----- Heuristic ----- //

		int		scoreManhattan(const int *target);	// Manhattan Heuristic
		int		scoreEuclidean(const int *target);	// Euclidean Heuristic
		int		scoreMisplaced(const int *target);	// Misplaced Heuristic
};

// ----- Operator Overloading ----- //

bool	operator==(const State& a, const State& b);
bool	operator!=(const State& a, const State& b);
bool	operator<(const State& a, const State& b);

#endif