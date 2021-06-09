#include "TwoDGridPF.h"

TwoDGridPF::TwoDGridPF(std::vector<Model*> modelList, glm::vec2 size)
{
	grid.resize(size.x);
	for (int i = 0; i < size.y; i++) {
		grid[i].resize(size.y);
	}
	
	this->size = size;
	insertModels(modelList);
}

void TwoDGridPF::insertModel(Model* model)
{
	
}

void TwoDGridPF::insertModels(std::vector<Model*> modelList)
{
	for each (Model* m in modelList)
	{
		insertModel(m);
	}
}

glm::vec2 TwoDGridPF::getSize()
{
	glm::vec2 size = glm::vec2(0);
	size.x = grid.size();
	size.y = grid[0].size();

	return size;
}

GridData TwoDGridPF::getEntry(int x, int y)
{
	return grid[x][y];
}
