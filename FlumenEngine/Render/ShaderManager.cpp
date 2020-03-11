#include "FlumenEngine/Render/ShaderManager.hpp"

#include "FlumenEngine/Render/Shader.hpp"
#include "ShaderUtility.hpp"
#include "FlumenEngine/Core/AssetManager.hpp"

Array <ShaderFile> ShaderManager::shaderFiles_ = Array <ShaderFile>();

Map <Shader, Word> ShaderManager::shaders_ = Map <Shader, Word>();

#define MAXIMUM_SHADER_COUNT 128

#define SHADER_EXTENSION_COUNT 6

struct ShaderExtension
{
	ShortWord Extension_;

	ShaderTypes Type_;

	ShaderExtension(ShortWord extension, ShaderTypes type) : Extension_(extension), Type_(type) {}
};

ShaderExtension shaderExtensions[SHADER_EXTENSION_COUNT] =
{
	ShaderExtension(".vert", ShaderTypes::VERTEX),
	ShaderExtension(".frag", ShaderTypes::FRAGMENT),
	ShaderExtension(".geom", ShaderTypes::GEOMETRY),
	ShaderExtension(".tcon", ShaderTypes::TESSELLATION_CONTROL),
	ShaderExtension(".tevl", ShaderTypes::TESSELLATION_EVALUATION),
	ShaderExtension(".comp", ShaderTypes::COMPUTE),
};

struct ShaderFileBatch
{
	Array <ShaderFile*> Files_;

	ShaderFileBatch() {Files_.Initialize(3);}

	void Add(ShaderFile* file) {*Files_.Allocate() = file;}
};

Map <ShaderFileBatch, Word> shaderFileBatches;

void ShaderManager::LoadShaders()
{
	shaders_.Initialize(MAXIMUM_SHADER_COUNT);
	shaderFiles_.Initialize(MAXIMUM_SHADER_COUNT * 3);
	shaderFileBatches.Initialize(MAXIMUM_SHADER_COUNT);

	auto files = AssetManager::GetFiles();
	for(File* file = files->GetStart(); file != files->GetEnd(); ++file)
	{
		for(int i = 0; i < SHADER_EXTENSION_COUNT; ++i)
		{
			auto &shaderExtension = shaderExtensions[i];

			auto extensionPosition = FindString(file->GetName(), shaderExtension.Extension_);
			if(extensionPosition == nullptr)
				continue;

			Word shaderName;
			shaderName.Add(file->GetName(), extensionPosition - file->GetName());

			auto shaderFileBatch = shaderFileBatches.Get(shaderName);
			if(shaderFileBatch == nullptr)
			{
				shaderFileBatch = shaderFileBatches.Add(shaderName);
			}

			auto shaderFile = shaderFiles_.Allocate();
			if(shaderFile)
			{
				*shaderFile = ShaderFile(file->GetName(), file->GetPath(), shaderExtension.Type_);
				shaderFileBatch->Add(shaderFile);
			}

			break;
		}
	}

	auto fileBatchKey = shaderFileBatches.GetFirstKey();
	for(auto fileBatch = shaderFileBatches.GetStart(); fileBatch != shaderFileBatches.GetEnd(); ++fileBatch, ++fileBatchKey)
	{
		std::cout<<"> "<<*fileBatchKey<<"\n";

		for(auto file = fileBatch->Files_.GetStart(); file != fileBatch->Files_.GetEnd(); ++file)
		{
			std::cout<<"---> "<<(*file)->GetName()<<"\n";
		}

		auto shader = shaders_.Add(*fileBatchKey);
		shader->Initialize(*fileBatchKey, fileBatch->Files_);
	}
}

Shader* ShaderManager::GetShader(const char* identifier)
{
	return shaders_.Get(identifier);
}
