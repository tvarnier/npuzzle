#include "State.hpp"

void	State::setScore(const int *goal, const e_heuristic& h, const e_algorithm& a)
{
	m_score = 0;

	if (a == A_STAR || a == A_UNIFORM)
	{
		if (h == H_MANHATTAN)
			m_score += (scoreManhattan(goal) * 2);
		else if (h == H_LINEARCONFLICT)
			m_score += (scoreManhattan(goal) + scoreLinearConflict(goal)) * 2;
		else if (h == H_MISPLACED)
			m_score += scoreMisplaced(goal) * 2;
	}
	if (a == A_STAR || a == A_GREEDY)
		m_score += m_nbrMoves;
}
void	State::updateScore(const int& piece, const int *goal, const e_heuristic& h, const e_algorithm& a)
{
	if (a == A_STAR || a == A_UNIFORM)
	{
		if (h == H_MANHATTAN)
			m_score += (updateManhattan(goal, piece) * 2);
		else if (h == H_LINEARCONFLICT)
			m_score += ((updateManhattan(goal, piece) + updateLinearConflict(goal, piece)) * 2);
		else if (h == H_MISPLACED)
			m_score += updateMisplaced(goal, piece) * 2;
	}
	if (a == A_STAR || a == A_GREEDY)
		m_score += 1;
}

// ----- Private Functions ----- //

static int		getTileGoalPosition(const int *goal, const int& tile)
{
	int i;
	for (i = 0; tile != goal[i]; ++i);
	return (i);
}

static int      isTileInGoodRow(const int *goal, const int& tileValue, const int& tilePosition, const int& length)
{
	int		i;
	int		row = tilePosition / length;
	for (i = row; i / length == row; ++i)
		if (goal[i] == tileValue)
			return (i);
	return (-1);
}

static int		isTileInGoodColumn(const int *goal, const int& tileValue, const int& tilePosition, const int& length)
{
	int		i;
	int		column = tilePosition % length;
	for (i = column; i < length * length; i += length)
		if (goal[i] == tileValue)
			return (i);
	return (-1);
}

///////////////////////////////////////////////
// --------------- MANHATTAN --------------- //
///////////////////////////////////////////////

int		State::scoreManhattan(const int *goal)
{
	int		i;
	int		j;
	int		score(0);

	for(i = 0; i < m_length * m_length; ++i)
		if (m_array[i] != 0)
		{
			j = getTileGoalPosition(goal, m_array[i]);
			score += std::abs((j / m_length) - (i / m_length));
			score += std::abs((j % m_length) - (i % m_length));
		}
	return (score);
}

int		State::updateManhattan(const int *goal, const int& piece)
{
	int 	score(0);
	int 	zero_position = getZeroPosition();
	int 	j(0);

	j = getTileGoalPosition(goal, m_array[piece]);
	score += std::abs((j / m_length) - (piece / m_length));
	score += std::abs((j % m_length) - (piece % m_length));

	score -= std::abs((j / m_length) - (zero_position / m_length));
	score -= std::abs((j % m_length) - (zero_position % m_length));

	return (score);
}

/////////////////////////////////////////////////////
// --------------- LINEAR CONFLICT --------------- //
/////////////////////////////////////////////////////


int		scoreLinearConflictRow(const int *array, const int *goal, const int& row, const int& length)
{
	int 	tile;
	int 	tileGoal;
	int		score(0);
	int		nextTile;
	int		nextTileGoal;

	for (tile = row * length; tile / length == row; ++tile)
	{
		if (array[tile] != 0 && (tileGoal = isTileInGoodRow(goal, array[tile], tile, length)) != -1)
		{
			for (nextTile = tile + 1; nextTile / length == row  && nextTile < length * length; ++nextTile)
			{
				if (array[nextTile] != 0 && (nextTileGoal = isTileInGoodRow(goal, array[nextTile], nextTile, length)) != -1)
				{
					if (nextTileGoal < tileGoal)
						score += 2;
				}
			}
		}
	}
	return (score);
}

int		scoreLinearConflictColumn(const int *array, const int *goal, const int& column, const int& length)
{
	int 	tile;
	int 	tileGoal;
	int		score(0);
	int		nextTile;
	int		nextTileGoal;

	for (tile = column; tile < length * length; tile += length)
	{
		if (array[tile] != 0 && (tileGoal = isTileInGoodColumn(goal, array[tile], tile, length)) != -1)
		{
			for (nextTile = tile + length; nextTile < length * length; nextTile += length)
			{
				if (array[nextTile] != 0 && (nextTileGoal = isTileInGoodColumn(goal, array[nextTile], nextTile, length)) != -1)
				{
					if (nextTileGoal < tileGoal)
						score += 2;
				}
			}
		}
	}
	return (score);
}

int		State::scoreLinearConflict(const int *goal)
{
	int		i;
	int 	score(0);
	
	for (i = 0; i < m_length; ++i)
	{
		score += scoreLinearConflictRow(m_array, goal, i, m_length);
		score += scoreLinearConflictColumn(m_array, goal, i, m_length);
	}

	return (score);
}

int		updateLinearConflictRow(const int *array, const int *goal, const int& row, const int& length, const int& piecePosition, const int& pieceValue)
{
	int 	tile;
	int 	tileGoal;
	int		score(0);
	int		nextTile;
	int		nextTileGoal;

	tile = piecePosition;
	if ((tileGoal = isTileInGoodRow(goal, pieceValue, piecePosition, length)) != -1)
		for (nextTile = row * length; nextTile / length == row; ++nextTile)
		{
			if (nextTile != piecePosition && (nextTileGoal = isTileInGoodRow(goal, array[nextTile], nextTile, length)) != -1)
			{
				if ( (nextTile > tile && nextTileGoal < tileGoal)
					|| (nextTile < tile && nextTileGoal > tileGoal) )
					score += 2;
			}
		}	
	return (score);
}

///////////////////////////////////////////////
// --------------- MISPLACED --------------- //
///////////////////////////////////////////////


int		updateLinearConflictColumn(const int *array, const int *goal, const int& column, const int& length, const int& piecePosition, const int& pieceValue)
{
	int 	tile;
	int 	tileGoal;
	int		score(0);
	int		nextTile;
	int		nextTileGoal;

	tile = piecePosition;
	if ((tileGoal = isTileInGoodColumn(goal, pieceValue, piecePosition, length)) != -1)
		for (nextTile = column; nextTile < length * length; nextTile += length)
		{
			if (nextTile != piecePosition && (nextTileGoal = isTileInGoodColumn(goal, array[nextTile], nextTile, length)) != -1)
			{
				if ( (nextTile > tile && nextTileGoal < tileGoal)
					|| (nextTile < tile && nextTileGoal > tileGoal) )
					score += 2;
			}
		}	
	return (score);
}
int		State::updateLinearConflict(const int *goal, const int& piece)
{
	int 	score(0);
	int 	zero_position = getZeroPosition();

	if (zero_position / m_length != piece / m_length) // Different Row
	{
		score += updateLinearConflictRow(m_array, goal, piece / m_length, m_length, piece, m_array[piece]);
		score -= updateLinearConflictRow(m_array, goal, zero_position / m_length, m_length, zero_position, m_array[piece]);
	}
	else if (zero_position % m_length != piece % m_length) // Different Column
	{
		score += updateLinearConflictColumn(m_array, goal, piece % m_length, m_length, piece, m_array[piece]);
		score -= updateLinearConflictColumn(m_array, goal, zero_position % m_length, m_length, zero_position, m_array[piece]);
	}
	return (score);
}

int 	State::scoreMisplaced(const int *goal)
{
	int		i;
	int		score(0);

	for(i = 0; i < m_length * m_length; ++i)
		if (m_array[i] != 0)
		{
			score += ( (isTileInGoodRow( goal, m_array[i], i, m_length )) == -1 ) ? 1 : 0;
			score += ( (isTileInGoodColumn( goal, m_array[i], i, m_length )) == -1 ) ? 1 : 0;
		}
	return (score);
}

int		State::updateMisplaced(const int *goal, const int& piece)
{
	int 	score(0);
	int 	zero_position = getZeroPosition();

	if (zero_position / m_length != piece / m_length) // Different Row
	{
		bool pieceInGoodRow = ((isTileInGoodRow(goal, m_array[piece], piece, m_length)) != -1);
		bool zeroInGoodRow = ((isTileInGoodRow(goal, m_array[piece], zero_position, m_length)) != -1);
		
		score += (pieceInGoodRow != zeroInGoodRow) ? ( pieceInGoodRow ? -1 : 1 ) : 0;
	}
	else if (zero_position % m_length != piece % m_length) // Different Column
	{
		bool pieceInGoodColumn = ((isTileInGoodColumn(goal, m_array[piece], piece, m_length)) != -1);
		bool zeroInGoodColumn = ((isTileInGoodColumn(goal, m_array[piece], zero_position, m_length)) != -1);
		
		score += (pieceInGoodColumn != zeroInGoodColumn) ? ( pieceInGoodColumn ? -1 : 1 ) : 0;
	}
	return (score);
}