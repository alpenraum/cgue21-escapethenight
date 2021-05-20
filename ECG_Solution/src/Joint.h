#pragma once
#include "utils/Utils.h"
//source: https://github.com/TheThinMatrix/OpenGL-Animation
class Joint
{
private:
	glm::mat4 animatedTransform; //finalBoneMatrix -> is loaded into shader
	glm::mat4 localBindTransform; 
	glm::mat4 offsetMatrix;
	glm::mat4 inverseBindTransform;
public:
	 int id;
	 string name;
	 string parentName;
	 std::vector<Joint*> children;

	 
	 

	Joint();
	Joint(int index, string name, glm::mat4 bindLocalTransform);
	Joint(int index, string name, glm::mat4 bindLocalTransform, glm::mat4 offsetMatrix);
	Joint(int index, string name, glm::mat4 bindLocalTransform, string parentName);

	void addChild(Joint child);

	glm::mat4 getAnimatedTransform();
	void setAnimatedTransform(glm::mat4 animatedTransform);

	glm::mat4 getLocalBindTransform();

	glm::mat4 getOffsetTransform();

	void calcInverseBindTransform(glm::mat4 parentBindTransform);



	void setParentName(string name) {
		this->parentName = name;
	}

	void printName() {
		std::cout << name << std::endl;

		for each (Joint* j in children) {
			j->printName();
		}
	}
};

