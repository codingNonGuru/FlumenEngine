#pragma once

#include "FlumenCore/Conventions.hpp"

class Image;

class StencilManager
{
	struct Identifier
	{
		Word Name_;

		Index Index_;

		Identifier() {}

		Identifier(Word Name, Index Index) : Name_(Name), Index_(Index) {}

		bool operator==(Identifier& other) {return Name_ == other.Name_ && Index_ == other.Index_;}
	};

	static Map <Image*, Identifier> stencils_;

public:
	static void Add(Image*, Word, Index);

	static Image* Get(Word, Index);
};
