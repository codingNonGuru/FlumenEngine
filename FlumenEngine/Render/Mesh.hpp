#pragma once

#include <glm/glm.hpp>

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Core/Types.hpp"

class File;
class AttributeType;

class MeshAttribute
{
private:
	Container* data_;

	AttributeType* type_;

	Length size_;

public:
	Container*& GetData() {return data_;}

	AttributeType* GetType() {return type_;}

	Length GetSize() {return size_;}

	void SetType(AttributeType* type) {type_ = type;}

	void SetSize(Length size) {size_ = size;}

	MeshAttribute() {}

	MeshAttribute(Container* data, AttributeType* type, Length size) : data_(data), type_(type), size_(size) {}
};

class Camera;

class Mesh
{
private:
	Map <MeshAttribute, Word> attributes_;

	Length indexCount_;

	Length vertexCount_;

public:
	static Mesh* GenerateQuad();

	static Mesh* GenerateHex();

	Mesh();

	void Initialize(int);

	void Initialize(File*);

	void LoadFromWavefront(File *);

	Map <MeshAttribute, Word> & GetAttributes() {return attributes_;}

	MeshAttribute* GetAttribute(const char*);

	void* GetAttributeData(const char*);

	Length GetAttributeMemoryCapacity(const char*);

	Length GetIndexCount() const {return indexCount_;}

	Length GetVertexCount() const {return vertexCount_;}

	virtual ~Mesh();
};
