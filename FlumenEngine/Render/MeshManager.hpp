#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Core/Types.hpp"
#include "FlumenEngine/Core/Attribute.hpp"

class Mesh;

struct MeshAttributeType : public AttributeType
{
	MeshAttributeTypes AttributeType_;

	MeshAttributeType() {}

	MeshAttributeType(Word identifier, Length memorySize, Length componentCount, AttributeElementTypes elementType, MeshAttributeTypes attributeType) :
		AttributeType(identifier, memorySize, componentCount, elementType), AttributeType_(attributeType) {}
};

class MeshManager
{
private:
	static Map <Mesh*, Word> meshes_;

public:
	static void Initialize();

	static void LoadMeshes();

	static Map <Mesh*, Word> & GetMeshes();

	static Mesh* GetMesh(const char*);
};
