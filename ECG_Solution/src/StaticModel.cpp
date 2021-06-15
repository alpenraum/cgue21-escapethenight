#include "StaticModel.h"

StaticModel::StaticModel()
{
	collisionModel = nullptr;
}

StaticModel::StaticModel(string const& path, glm::vec3 position, PhysxMaster* physxMaster, bool shapeIsExclusive)
{
	this->transform.setPosition(position);
	PxQuat rot(-PxHalfPi, PxVec3(1.0f,0.0f,0.0f));
	this->transform.setRotation(rot);

	loadModel(path);

	PxTransform staticPosition = PxTransform(PxVec3(position.x, position.y, position.z),rot);
	collisionModel = physxMaster->createRigidStatic(staticPosition);
	PxShape* shape;

	PxCooking* cooker = physxMaster->getCooking();
	for (unsigned int i = 0; i < meshes.size(); i++) {
		
		int vertexCount = 0;
		int indexCount = 0;
		std::vector<PxVec3> vertexData;
		std::vector<GLuint> indexData;
		std::vector<Vertex> tempV = meshes[i].getVertices();
		std::vector<GLuint> tempI = meshes[i].getIndices();
		for (unsigned int k = 0; k < tempV.size(); k++) {



			vertexData.push_back(PxVec3(tempV.at(k).position.x, tempV.at(k).position.y, tempV.at(k).position.z));
			vertexCount++;
		}
		for (unsigned int j = 0; j < tempI.size(); j++) {
			indexData.push_back(tempI.at(j));
			indexCount++;
		}

		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = vertexCount;
		meshDesc.points.stride = sizeof(PxVec3);
		meshDesc.points.data = &vertexData[0];

		meshDesc.triangles.count = indexCount/3.0f;
		meshDesc.triangles.stride = 3 * sizeof(PxU32);
		meshDesc.triangles.data = &indexData[0];


		PxDefaultMemoryOutputStream buf;
		PxTriangleMeshCookingResult::Enum result;
		if (!cooker->cookTriangleMesh(meshDesc, buf, &result))
			return;
		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			
		shape = physxMaster->createShapefromTriangleMesh(input, convert(this->transform.getScale()), true);


		

		
		
		collisionModel->attachShape(*shape);
		shape->release();
	}
	physxMaster->addActor(collisionModel);



}

StaticModel::StaticModel(string const& path, glm::vec3 position, PxQuat rot, PhysxMaster* physxMaster, bool shapeIsExclusive)
{
	this->transform.setPosition(position);
	this->transform.setRotation(rot);

	loadModel(path);

	PxTransform staticPosition = PxTransform(PxVec3(position.x, position.y, position.z), rot);
	collisionModel = physxMaster->createRigidStatic(staticPosition);
	PxShape* shape;

	PxCooking* cooker = physxMaster->getCooking();
	for (unsigned int i = 0; i < meshes.size(); i++) {

		int vertexCount = 0;
		int indexCount = 0;
		std::vector<PxVec3> vertexData;
		std::vector<GLuint> indexData;
		std::vector<Vertex> tempV = meshes[i].getVertices();
		std::vector<GLuint> tempI = meshes[i].getIndices();
		for (unsigned int k = 0; k < tempV.size(); k++) {



			vertexData.push_back(PxVec3(tempV.at(k).position.x, tempV.at(k).position.y, tempV.at(k).position.z));
			vertexCount++;
		}
		for (unsigned int j = 0; j < tempI.size(); j++) {
			indexData.push_back(tempI.at(j));
			indexCount++;
		}

		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = vertexCount;
		meshDesc.points.stride = sizeof(PxVec3);
		meshDesc.points.data = &vertexData[0];

		meshDesc.triangles.count = indexCount / 3.0f;
		meshDesc.triangles.stride = 3 * sizeof(PxU32);
		meshDesc.triangles.data = &indexData[0];


		PxDefaultMemoryOutputStream buf;
		PxTriangleMeshCookingResult::Enum result;
		if (!cooker->cookTriangleMesh(meshDesc, buf, &result))
			return;
		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());

		shape = physxMaster->createShapefromTriangleMesh(input, convert(this->transform.getScale()), true);






		collisionModel->attachShape(*shape);
		shape->release();
	}
	physxMaster->addActor(collisionModel);



}


StaticModel::StaticModel(string const& path, glm::vec3 position, glm::vec3 scale, PhysxMaster* physxMaster, bool shapeIsExclusive): StaticModel(path, position, physxMaster, shapeIsExclusive)
{
	this->transform.setScale(scale);
	
}


