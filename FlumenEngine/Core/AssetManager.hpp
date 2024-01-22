#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Core/Attribute.hpp"

class Delegate;
class File;

class AssetManager
{
private:
	static const Array <File> *files_;

	static Map <AttributeType, Word> attributeTypes_;

	static void LoadTextures();

	static void LoadMeshes();

	static void CountScripts();

public:
	static Delegate* OnAssetsLoaded_;

	static Array <File> *GetFiles();

	static Map <AttributeType, Word> & GetAttributeTypes();

	static void Initialize();

	static void LoadAssets();
};
