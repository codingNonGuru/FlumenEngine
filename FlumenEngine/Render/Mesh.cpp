#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <string>

#include <GL/glew.h>

#include "tiny_obj_loader.h"

#include "FlumenEngine/Render/Mesh.hpp"
#include "FlumenEngine/Core/AssetManager.hpp"
#include "FlumenEngine/Core/File.hpp"

Mesh::Mesh() {}

Mesh* Mesh::GenerateQuad()
{
	auto mesh = new Mesh();

	return mesh;
}

Mesh* Mesh::GenerateHex()
{
	auto mesh = new Mesh();
	mesh->Initialize(2);

	auto positionAttribute = mesh->attributes_.Add("position");
	positionAttribute->SetSize(7);
	auto positionData = new Array <Float3> (7);

	*positionData->Allocate() = Float3(0.0f, 0.0f, 0.0f);
	*positionData->Allocate() = Float3(0.866f, 0.5f, 0.0f);
	*positionData->Allocate() = Float3(0.0f, 1.0f, 0.0f);
	*positionData->Allocate() = Float3(-0.866f, 0.5f, 0.0f);
	*positionData->Allocate() = Float3(-0.866f, -0.5f, 0.0f);
	*positionData->Allocate() = Float3(0.0f, -1.0f, 0.0f);
	*positionData->Allocate() = Float3(0.866f, -0.5f, 0.0f);

	positionAttribute->GetData() = positionData;

	auto indexAttribute = mesh->attributes_.Add("index");
	indexAttribute->SetSize(18);
	auto indexData = new Array <Index> {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 1};

	indexAttribute->GetData() = indexData;

	mesh->vertexCount_ = 7;
	mesh->indexCount_ = 18;

	return mesh;
}

void Mesh::Initialize(int vertexDataCount)
{
	attributes_.Initialize(vertexDataCount);
}

struct MeshAttributeHeader
{
	char Name_[32];

	Length ElementCount_;

	Length ElementSize_;

	char Type_[8];
};

void* attributeData = malloc(16 * MEGABYTE);

#define RED_CODE "\033[1;31m"
#define ORANGE_CODE "\033[0;91m"
#define BOLD_WHITE_CODE "\033[1;97m"
#define BLANK_CODE "\033[0m"

void Mesh::Initialize(File* file)
{
	FILE* fileStream = fopen(file->GetPath(), "rb");
	if(fileStream == nullptr)
		return;

	Length attributeCount;
	fread(&attributeCount, 1, sizeof(attributeCount), fileStream);

	std::cout << "> Mesh name: " << RED_CODE << file->GetName() << BLANK_CODE << "\n";
	std::cout << "> Attribute count: " << BOLD_WHITE_CODE << attributeCount << BLANK_CODE << "\n";

	attributes_.Initialize(attributeCount);

	MeshAttributeHeader *headers = new MeshAttributeHeader[attributeCount];
	fread(headers, 1, sizeof(MeshAttributeHeader) * attributeCount, fileStream);

	for(auto header = headers; header != headers + attributeCount; ++header)
	{
		std::cout << "---> Mesh Attribute name: " << ORANGE_CODE << header->Name_ << BLANK_CODE << "\n";
		std::cout << "---> Mesh Attribute element count: " << BOLD_WHITE_CODE << header->ElementCount_ << BLANK_CODE << "\n";
		std::cout << "---> Mesh Attribute element size: " << BOLD_WHITE_CODE << header->ElementSize_ << BLANK_CODE << "\n";
		std::cout << "---> Mesh Attribute type: " << BOLD_WHITE_CODE << header->Type_ << BLANK_CODE << "\n";
		std::cout << "\n";

		auto attribute = attributes_.Add(header->Name_);
		if(attribute == nullptr)
			continue;

		Length attributeMemorySize = header->ElementCount_ * header->ElementSize_;
		fread(attributeData, 1, attributeMemorySize, fileStream);

		if(CompareStrings(header->Type_, "vec3") == 0)
		{
			attribute->GetData() = new Array <Float3> (header->ElementCount_);
		}
		else if(CompareStrings(header->Type_, "vec2") == 0)
		{
			attribute->GetData() = new Array <Float2> (header->ElementCount_);
		}
		else if(CompareStrings(header->Type_, "float") == 0)
		{
			attribute->GetData() = new Array <Float> (header->ElementCount_);
		}
		else if(CompareStrings(header->Type_, "uint") == 0)
		{
			attribute->GetData() = new Array <Index> (header->ElementCount_);
		}

		CopyMemory(attribute->GetData()->GetData(), attributeData, attributeMemorySize);
		auto attributeType = AssetManager::GetAttributeTypes().Get(header->Type_);
		attribute->SetType(attributeType);
		attribute->SetSize(header->ElementCount_);
	}
	std::cout<<"\n";

	indexCount_ = attributes_.Get("index")->GetSize();
	vertexCount_ = attributes_.Get("position")->GetSize();

	fclose(fileStream);
	delete [] headers;
}

void Mesh::LoadFromWavefront(File *file)
{
	std::cout << "> Mesh name: " << RED_CODE << file->GetName() << BLANK_CODE << "\n";

	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./"; // Path to material files

	tinyobj::ObjReader reader;

	if (reader.ParseFromFile(file->GetPath(), reader_config) == false) 
	{
		if (reader.Error().empty() == false) 
		{
			std::cerr << "TinyObjReader: " << reader.Error();
		}

		exit(1);
	}

	auto& attributes = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	std::cout << "> Vertex count: " << BOLD_WHITE_CODE << attributes.vertices.size() << BLANK_CODE << "\n";

	auto &shape = shapes.at(0);

	indexCount_ = shape.mesh.indices.size();
	vertexCount_ = attributes.vertices.size() / 3;

	auto indices = new container::Array <unsigned int> (indexCount_);
    for(auto &index : shape.mesh.indices)
    {
        *indices->Add() = index.vertex_index;
    }

    auto positions = new container::Array <Position2> (vertexCount_);
    for(int i = 0; i < attributes.vertices.size(); i += 3)
    {
        *positions->Add() = {attributes.vertices[i], attributes.vertices[i + 2]};
    }

	attributes_.Initialize(2);

	auto attribute = attributes_.Add("position");
	*attribute = MeshAttribute(positions, new AttributeType{"position", 8, 2, AttributeElementTypes::FLOAT_2}, vertexCount_);

	attribute = attributes_.Add("index");
	*attribute = MeshAttribute(indices, new AttributeType{"index", 4, 1, AttributeElementTypes::UINTEGER_1}, indexCount_);

    /*tinyobj::attrib_t attributes;
    std::vector <tinyobj::shape_t> shapes;
    std::vector <tinyobj::material_t> materials;

    std::string warnings;
    std::string errors;

    bool hasLoaded = tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, file->GetName());

	std::cout << "> Vertex count: " << BOLD_WHITE_CODE << attributes.vertices.size() << BLANK_CODE << "\n";

    auto shape = shapes.at(0);

	indexCount_ = shape.mesh.indices.size();
	vertexCount_ = attributes.vertices.size() / 3;*/

    /*auto indices = new container::Array <unsigned int> (indexCount_);
    for(auto &index : shape.mesh.indices)
    {
        *indices->Add() = index.vertex_index;
    }*/

    /*auto positions = new container::Array <Position2> (vertexCount_);
    for(int i = 0; i < attributes.vertices.size(); i += 3)
    {
        *positions->Add() = {attributes.vertices[i], attributes.vertices[i + 2]};
    }*/

	/*attributes_.Initialize(2);

	auto attribute = attributes_.Add("position");
	*attribute = MeshAttribute(positions, new AttributeType{"position", 8, 2, AttributeElementTypes::FLOAT_2}, vertexCount_);

	attribute = attributes_.Add("index");
	*attribute = MeshAttribute(indices, new AttributeType{"index", 4, 1, AttributeElementTypes::UINTEGER_1}, indexCount_);*/

	
}

MeshAttribute* Mesh::GetAttribute(const char* identifier)
{
	return attributes_.Get(identifier);
}

void* Mesh::GetAttributeData(const char* identifier)
{
	auto attribute = attributes_.Get(identifier);
	if(attribute == nullptr)
		return nullptr;

	return attribute->GetData()->GetData();
}

Length Mesh::GetAttributeMemoryCapacity(const char* identifier)
{
	auto attribute = attributes_.Get(identifier);
	if(attribute == nullptr)
		return 0;

	return attribute->GetData()->GetMemoryCapacity();
}

Mesh::~Mesh() {}

