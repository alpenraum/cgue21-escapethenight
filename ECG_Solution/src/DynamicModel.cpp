#include "DynamicModel.h"

DynamicModel::DynamicModel()
{
	collisionModel = nullptr;
}

DynamicModel::DynamicModel(string const& path, glm::vec3 position, PhysxMaster* physxMaster, bool shapeIsExclusive)
{
	this->transform.setPosition(position);
	loadModel(path);

	PxTransform planePos = PxTransform(PxVec3(position.x, position.y, position.z));
	collisionModel = physxMaster->createRigidDynamic(planePos);
	PxShape* shape;

	PxCooking* cooker = physxMaster->getCooking();

	
	int vertexCount = 0;

	std::vector<PxVec3> vertexData;
	for (unsigned int i = 0; i < meshes.size(); i++) {
		std::vector<Vertex> tempV = meshes[i].getVertices();
		for (unsigned int k = 0; k < tempV.size(); k++) {



			vertexData.push_back(PxVec3(tempV.at(k).position.x, tempV.at(k).position.y, tempV.at(k).position.z));
			vertexCount++;
		}
		tempV.clear();
	}

	PxConvexMeshDesc meshDesc;
	meshDesc.points.count = vertexCount;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = &vertexData[0];
	meshDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;


	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!cooker->cookConvexMesh(meshDesc, buf, &result))
		return;
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	this->transform.setScale(glm::vec3(0.4f));
	shape = physxMaster->createShapefromConvexMesh(input,convert(this->transform.getScale()), shapeIsExclusive);

	vertexData.clear();
	
	
	collisionModel->attachShape(*shape);
	shape->release();

	physxMaster->addActor(collisionModel);


}

DynamicModel::DynamicModel(string const& path, glm::vec3 position, glm::vec3 scale, PhysxMaster* physxMaster, bool shapeIsExclusive) : DynamicModel(path, position, physxMaster, shapeIsExclusive)
{
	this->transform.setScale(scale);
}


void DynamicModel::draw(AdvancedShader& shader) {
	PxTransform pTm = collisionModel->getGlobalPose();
	transform.setPosition(pTm.p);
	transform.setRotation(pTm.q);
	
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader, transform);
	}


}
