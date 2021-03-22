#include "Pathfinder.h"

Pathfinder::Pathfinder(glm::vec2 size)
{
	this->size = size;
	closedList.resize(size.x);
	for (int i = 0; i < size.y; i++) {
		closedList[i].resize(size.y);
	}
	cellDetails.resize(size.x);
	for (int i = 0; i < size.y; i++) {
		cellDetails[i].resize(size.y);
	}

	OFFSET=  glm::vec2((size.x / 2) +1,(size.y/2)+1);
}

//returns positions where to go to to follow the calculated path
std::vector<glm::vec2> Pathfinder::findPath(glm::vec2 startPos, glm::vec2 endPos, TwoDGridPF grid)
{
	std::vector<glm::vec2> results;
	glm::vec2 size = grid.getSize();

	if (isDestination(startPos.x, startPos.y, endPos)
		== true) {
		results.push_back(endPos);
		return results;
	}

	int i, j;

	for (i = 0; i < size.x; i++) {
		for (j = 0; j < size.y; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	startPos += OFFSET;
	endPos += OFFSET;

	i = startPos.x, j = startPos.y;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	// <f, <i, j>>
	std::set<pPair> openList;

	// Put the starting cell on the open list and set its
	// 'f' as 0
	openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

	while (!openList.empty()) {
		pPair p = *openList.begin();

		// Remove this vertex from the open list
		openList.erase(openList.begin());

		// Add this vertex to the closed list
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		/*
			 Generating all the 8 successor of this cell

				 N.W   N   N.E
				   \   |   /
					\  |  /
				 W----Cell----E
					  / | \
					/   |  \
				 S.W    S   S.E

			 Cell-->Popped Cell (i, j)
			 N -->  North       (i-1, j)
			 S -->  South       (i+1, j)
			 E -->  East        (i, j+1)
			 W -->  West           (i, j-1)
			 N.E--> North-East  (i-1, j+1)
			 N.W--> North-West  (i-1, j-1)
			 S.E--> South-East  (i+1, j+1)
			 S.W--> South-West  (i+1, j-1)*/

			 // To store the 'g', 'h' and 'f' of the 8 successors
		double gNew, hNew, fNew;

		//----------- 1st Successor (North) ------------

		// Only process this cell if this is a valid one
		if (isValid(i - 1, j, grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i - 1, j, endPos) == true) {
				// Set the Parent of the destination cell
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;

				results = loadFinishedPath(endPos);
				resetLists();
				return results;
			}
			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i - 1][j] == false
				&& grid.getEntry(i - 1, j).isOccupied == false) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calcHValue(i - 1, j, endPos);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i - 1][j].f == FLT_MAX
					|| cellDetails[i - 1][j].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i - 1, j)));

					// Update the details of this cell
					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
				}
			}
		}

		//----------- 2nd Successor (South) ------------

		// Only process this cell if this is a valid one
		if (isValid(i + 1, j, grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i + 1, j, endPos) == true) {
				// Set the Parent of the destination cell
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;

				results = loadFinishedPath(endPos);
				resetLists();
				return results;
			}
			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i + 1][j] == false
				&& grid.getEntry(i + 1, j).isOccupied == false) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calcHValue(i + 1, j, endPos);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i + 1][j].f == FLT_MAX
					|| cellDetails[i + 1][j].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i + 1, j)));
					// Update the details of this cell
					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

		//----------- 3rd Successor (East) ------------

		// Only process this cell if this is a valid one
		if (isValid(i, j + 1, grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i, j + 1, endPos) == true) {
				// Set the Parent of the destination cell
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;

				results = loadFinishedPath(endPos);
				resetLists();
				return results;
			}

			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i][j + 1] == false
				&& grid.getEntry(i, j + 1).isOccupied == false) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calcHValue(i, j + 1, endPos);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i][j + 1].f == FLT_MAX
					|| cellDetails[i][j + 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i, j + 1)));

					// Update the details of this cell
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}

		//----------- 4th Successor (West) ------------

		// Only process this cell if this is a valid one
		if (isValid(i, j - 1, grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i, j - 1, endPos) == true) {
				// Set the Parent of the destination cell
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;

				results = loadFinishedPath(endPos);
				resetLists();
				return results;
			}

			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i][j - 1] == false
				&& grid.getEntry(i, j - 1).isOccupied == false) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calcHValue(i, j - 1, endPos);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i][j - 1].f == FLT_MAX
					|| cellDetails[i][j - 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i, j - 1)));

					// Update the details of this cell
					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}

		//----------- 5th Successor (North-East)
		//------------

		// Only process this cell if this is a valid one
		if (isValid(i - 1, j + 1, grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i - 1, j + 1, endPos) == true) {
				// Set the Parent of the destination cell
				cellDetails[i - 1][j + 1].parent_i = i;
				cellDetails[i - 1][j + 1].parent_j = j;

				results = loadFinishedPath(endPos);
				resetLists();
				return results;
			}

			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i - 1][j + 1] == false
				&& grid.getEntry(i - 1, j + 1).isOccupied == false) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calcHValue(i - 1, j + 1, endPos);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i - 1][j + 1].f == FLT_MAX
					|| cellDetails[i - 1][j + 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i - 1, j + 1)));

					// Update the details of this cell
					cellDetails[i - 1][j + 1].f = fNew;
					cellDetails[i - 1][j + 1].g = gNew;
					cellDetails[i - 1][j + 1].h = hNew;
					cellDetails[i - 1][j + 1].parent_i = i;
					cellDetails[i - 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- 6th Successor (North-West)
		//------------

		// Only process this cell if this is a valid one
		if (isValid(i - 1, j - 1, grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i - 1, j - 1, endPos) == true) {
				// Set the Parent of the destination cell
				cellDetails[i - 1][j - 1].parent_i = i;
				cellDetails[i - 1][j - 1].parent_j = j;

				results = loadFinishedPath(endPos);
				resetLists();
				return results;
			}

			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i - 1][j - 1] == false
				&& grid.getEntry(i - 1, j - 1).isOccupied == false) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calcHValue(i - 1, j - 1, endPos);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i - 1][j - 1].f == FLT_MAX
					|| cellDetails[i - 1][j - 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i - 1, j - 1)));
					// Update the details of this cell
					cellDetails[i - 1][j - 1].f = fNew;
					cellDetails[i - 1][j - 1].g = gNew;
					cellDetails[i - 1][j - 1].h = hNew;
					cellDetails[i - 1][j - 1].parent_i = i;
					cellDetails[i - 1][j - 1].parent_j = j;
				}
			}
		}

		//----------- 7th Successor (South-East)
		//------------

		// Only process this cell if this is a valid one
		if (isValid(i + 1, j + 1, grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i + 1, j + 1, endPos) == true) {
				// Set the Parent of the destination cell
				cellDetails[i + 1][j + 1].parent_i = i;
				cellDetails[i + 1][j + 1].parent_j = j;

				results = loadFinishedPath(endPos);
				resetLists();
				return results;
			}

			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i + 1][j + 1] == false
				&& grid.getEntry(i + 1, j + 1).isOccupied == false) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calcHValue(i + 1, j + 1, endPos);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i + 1][j + 1].f == FLT_MAX
					|| cellDetails[i + 1][j + 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i + 1, j + 1)));

					// Update the details of this cell
					cellDetails[i + 1][j + 1].f = fNew;
					cellDetails[i + 1][j + 1].g = gNew;
					cellDetails[i + 1][j + 1].h = hNew;
					cellDetails[i + 1][j + 1].parent_i = i;
					cellDetails[i + 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- 8th Successor (South-West)
		//------------

		// Only process this cell if this is a valid one
		if (isValid(i + 1, j - 1, grid) == true) {
			// If the destination cell is the same as the
			// current successor
			if (isDestination(i + 1, j - 1, endPos) == true) {
				// Set the Parent of the destination cell
				cellDetails[i + 1][j - 1].parent_i = i;
				cellDetails[i + 1][j - 1].parent_j = j;
				
				results = loadFinishedPath(endPos);
				resetLists();
				return results;
			}

			// If the successor is already on the closed
			// list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i + 1][j - 1] == false
				&& grid.getEntry(i + 1, j - 1).isOccupied == false) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calcHValue(i + 1, j - 1, endPos);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to
				// the open list. Make the current square
				// the parent of this square. Record the
				// f, g, and h costs of the square cell
				//                OR
				// If it is on the open list already, check
				// to see if this path to that square is
				// better, using 'f' cost as the measure.
				if (cellDetails[i + 1][j - 1].f == FLT_MAX
					|| cellDetails[i + 1][j - 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i + 1, j - 1)));

					// Update the details of this cell
					cellDetails[i + 1][j - 1].f = fNew;
					cellDetails[i + 1][j - 1].g = gNew;
					cellDetails[i + 1][j - 1].h = hNew;
					cellDetails[i + 1][j - 1].parent_i = i;
					cellDetails[i + 1][j - 1].parent_j = j;
				}
			}
		}
	}
	resetLists();
	results.push_back(startPos);
	return results;
}

bool Pathfinder::isValid(int row, int col, TwoDGridPF grid)
{
	glm::vec2 size = grid.getSize();
	return (row >= 0) && (row < size.x) && (col >= 0)
		&& (col < size.y);
}

bool Pathfinder::isDestination(int row, int col, glm::vec2 dest)
{
	if (row == dest.x && col == dest.y)
		return (true);
	else
		return (false);
}

double Pathfinder::calcHValue(int row, int col, glm::vec2 dest)
{
	return ((double)sqrt(
		(row - dest.x) * (row - dest.x)
		+ (col - dest.y) * (col - dest.y)));
}

std::vector<glm::vec2> Pathfinder::loadFinishedPath(glm::vec2 endPos)
{
	std::vector<glm::vec2>path;
	int row = endPos.x;
	int col = endPos.y;

	while (!(cellDetails[row][col].parent_i == row
		&& cellDetails[row][col].parent_j == col)) {
		path.push_back(glm::vec2(row-OFFSET.x, col-OFFSET.y));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	path.push_back(glm::vec2(row - OFFSET.x, col - OFFSET.y));

	std::reverse(path.begin(), path.end());
	return path;
}

void Pathfinder::resetLists()
{
	closedList.clear();
	cellDetails.clear();

	closedList.resize(size.x);
	for (int i = 0; i < size.y; i++) {
		closedList[i].resize(size.y);
	}
	cellDetails.resize(size.x);
	for (int i = 0; i < size.y; i++) {
		cellDetails[i].resize(size.y);
	}
}