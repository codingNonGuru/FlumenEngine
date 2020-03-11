#pragma once

#include "FlumenEngine/Core/Types.hpp"

struct AttributeType
{
	Word Identifier_;

	Length MemorySize_;

	Length ComponentCount_;

	AttributeElementTypes ElementType_;

	AttributeType() {}

	AttributeType(Word identifier, Length memorySize, Length componentCount, AttributeElementTypes type) : Identifier_(identifier), MemorySize_(memorySize), ComponentCount_(componentCount), ElementType_(type) {}
};

class Attribute
{

};
