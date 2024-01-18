#pragma once

#include "FlumenEngine/Core/File.hpp"
#include "FlumenEngine/Core/Types.hpp"

class ShaderFile : public File
{
private:
	ShaderTypes type_;

public:
	ShaderFile() {}

	ShaderFile(const char* name, const char* path, ShaderTypes type) : File(name, path), type_(type) {}

	const char* GetPath() {return path_.Get();}

	void SetPath(const char* path) {path_ = path;}

	ShaderTypes GetType() const {return type_;}
};

class BufferBinding
{
private:
	Index index_;

public:
	BufferBinding() {}

	BufferBinding(Index index) : index_(index) {}
};

class TextureBinding
{
private:
	Index unitIndex_;

	Index location_;

public:
	TextureBinding() {}

	TextureBinding(Index unitIndex, Index location) : unitIndex_(unitIndex), location_(location) {}

	Index GetUnitIndex() const {return unitIndex_;}

	Index GetLocation() const {return location_;}
};

class ConstantBinding
{
private:
	Index location_;

	AttributeElementTypes type_;

public:
	ConstantBinding() {}

	ConstantBinding(Index location, AttributeElementTypes type) : location_(location), type_(type) {}

	Index GetLocation() const {return location_;}

	AttributeElementTypes GetType() const {return type_;}
};
