#include "ModelManager.hpp"

#include "FlumenEngine/Render/Model.hpp"

#define MAXIMUM_MODEL_COUNT 256

Map <Model, Word> ModelManager::models_ = Map <Model, Word> (MAXIMUM_MODEL_COUNT);

Map <Model, Word> & ModelManager::GetModels()
{
	return models_;
}

void ModelManager::AddModel(Model* model, const char* name)
{
	auto newModel = models_.Add(name);
	if(newModel == nullptr)
	{
		std::cout<<"Maximum model count was reached.\n";
		return;
	}

	*newModel = *model;
}

Model* ModelManager::GetModel(const char* identifier)
{
	return models_.Get(identifier);
}


