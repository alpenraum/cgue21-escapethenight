#pragma once
#pragma warning( disable : 4244 )
#include "utils/Utils.h"
#include <vector>
#include "TwoDGridPF.h"

struct cell {
	int parent_i, parent_j;
	double f, g, h;
};

// Creating a shortcut for int, int pair type
typedef std::pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef std::pair<double, std::pair<int, int> > pPair;

class Pathfinder {
private:
	glm::vec2 size;
	std::vector<std::vector<bool>> closedList;
	std::vector<std::vector<cell>> cellDetails;

	glm::vec2 OFFSET;
	
public:
	Pathfinder(glm::vec2 size);

	std::vector<glm::vec2> findPath(glm::vec2 startPos, glm::vec2 endPos, TwoDGridPF grid);
	bool isValid(int row, int col, TwoDGridPF grid);
	bool isDestination(int row, int col, glm::vec2 dest);

	double calcHValue(int row, int col, glm::vec2 dest);

	std::vector<glm::vec2> loadFinishedPath(glm::vec2 endPos);

	void resetLists();
};

