#pragma once
#pragma warning( disable : 4244 )
#include "utils/Utils.h"
#include <vector>
#include "Model.h"

struct GridData {
	int xPos;
	int yPos;
	bool isOccupied;
};
class TwoDGridPF {
private:
	//if true, this spot is occupied by terrain
	std::vector<std::vector<GridData>> grid;
	glm::vec2 size;

public:
	TwoDGridPF(std::vector<Model*> modelList, glm::vec2 size);

	void insertModel(Model* model);

	void insertModels(std::vector<Model*> modelList);

	glm::vec2 getSize();

	GridData getEntry(int x, int y);
};