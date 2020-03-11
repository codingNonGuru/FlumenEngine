#include <stdio.h>

#include "FlumenEngine/Core/AssetManager.hpp"

#include "FileManager.hpp"
#include "FlumenCore/Delegate/Delegate.hpp"
#include "FlumenEngine/Core/File.hpp"
#include "FlumenEngine/Render/ShaderManager.hpp"
#include "FlumenEngine/Render/MeshManager.hpp"
#include "FlumenEngine/Render/TextureManager.hpp"
#include "FlumenEngine/Interface/FontManager.hpp"

Array <File>* AssetManager::files_ = nullptr;

Map <AttributeType, Word> AssetManager::attributeTypes_ = Map <AttributeType, Word> ();

Delegate* AssetManager::OnAssetsLoaded_ = new Delegate();

void AssetManager::Initialize()
{
	attributeTypes_.Initialize(16);

	*attributeTypes_.Add("float") = AttributeType("float", 4, 1, AttributeElementTypes::FLOAT_1);
	*attributeTypes_.Add("vec2") = AttributeType("vec2", 8, 2, AttributeElementTypes::FLOAT_2);
	*attributeTypes_.Add("vec3") = AttributeType("vec3", 12, 3, AttributeElementTypes::FLOAT_3);
	*attributeTypes_.Add("vec4") = AttributeType("vec4", 16, 4, AttributeElementTypes::FLOAT_4);

	*attributeTypes_.Add("int") = AttributeType("int", 4, 1, AttributeElementTypes::INTEGER_1);
	*attributeTypes_.Add("ivec2") = AttributeType("ivec2", 8, 2, AttributeElementTypes::INTEGER_2);
	*attributeTypes_.Add("ivec3") = AttributeType("ivec3", 12, 3, AttributeElementTypes::INTEGER_3);
	*attributeTypes_.Add("ivec4") = AttributeType("ivec4", 16, 4, AttributeElementTypes::INTEGER_4);

	*attributeTypes_.Add("uint") = AttributeType("uint", 4, 1, AttributeElementTypes::UINTEGER_1);
	*attributeTypes_.Add("uvec2") = AttributeType("uvec2", 8, 2, AttributeElementTypes::UINTEGER_2);
	*attributeTypes_.Add("uvec3") = AttributeType("uvec3", 12, 3, AttributeElementTypes::UINTEGER_3);
	*attributeTypes_.Add("uvec4") = AttributeType("uvec4", 16, 4, AttributeElementTypes::UINTEGER_4);

	*attributeTypes_.Add("mat3") = AttributeType("mat3", 36, 9, AttributeElementTypes::MATRIX_3);
	*attributeTypes_.Add("mat4") = AttributeType("mat4", 64, 16, AttributeElementTypes::MATRIX_4);
}

char* code = new char[1024 * 1024];

void AssetManager::LoadAssets()
{
	FileManager::Initialize();

	files_ = FileManager::GetFilesInFolder(".", true);

	ShaderManager::LoadShaders();

	MeshManager::LoadMeshes();

	TextureManager::LoadTextures();

	FontManager::LoadFonts();

	//CountScripts();

	OnAssetsLoaded_->Invoke();
}

void AssetManager::CountScripts()
{
	Length scriptCount = 0;
	Length totalLineCount = 0;
	for(auto file = files_->GetStart(); file != files_->GetEnd(); ++file)
	{
		if((FindString(file->GetName(), ".hpp") || FindString(file->GetName(), ".cpp")) && !FindString(file->GetName(), "FastDelegate") && !FindString(file->GetName(), ".o") && FindString(file->GetPath(), "Source"))
		{
			FILE* fileStream = fopen(file->GetPath(), "r");

			fseek(fileStream, 0, SEEK_END);
			Length fileSize = ftell(fileStream);
			Length lineCount = 0;
			rewind(fileStream);

			fread(code, 1, fileSize, fileStream);
			for(auto sign = code; sign != code + fileSize; ++sign)
			{
				if(*sign == '\n')
				{
					lineCount++;
				}
			}

			fclose(fileStream);

			std::cout<<file->GetName()<<" ---------> "<<lineCount<<"\n";

			totalLineCount += lineCount;
			scriptCount++;
		}
	}
	std::cout<<"\n";
	std::cout<<scriptCount<<" Scripts found.\n";
	std::cout<<totalLineCount<<" Lines found.\n";
}

Array <File>* AssetManager::GetFiles()
{
	return files_;
}

Map <AttributeType, Word> & AssetManager::GetAttributeTypes()
{
	return attributeTypes_;
}



