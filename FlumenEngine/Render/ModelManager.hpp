#pragma once

#include "FlumenCore/Conventions.hpp"

class Model;

class ModelManager
{
private:
	static Map <Model, Word> models_;

public:
	static Map <Model, Word> & GetModels();

	static Model* GetModel(const char*);

	static void AddModel(Model*, const char*);
};
