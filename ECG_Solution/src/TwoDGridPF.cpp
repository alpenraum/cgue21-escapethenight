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
	glm::vec2 pos = glm::vec2 (model->getPosition().x, model->getPosition().z);
	glm::vec2 bottomLeft = model->getBottomLeft();
	glm::vec2 topRight = model->getTopRight();

	bottomLeft = pos - bottomLeft;
	topRight = pos + topRight;

	for (int x = (int)bottomLeft.x; x < ((int)topRight.x - (int)bottomLeft.x); x++) {
		for (int y = (int)bottomLeft.y; y < ((int)topRight.y - (int)bottomLeft.y); y++) {
			grid[x + (size.x / 2)][y + (size.y / 2)] = { x,y,true };
		}
	}
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
