#pragma once

#include "FlumenCore/Conventions.hpp"

class Shader;
class ShaderFile;

class ShaderParser
{
private:
	struct Expression
	{
		Index position_;

		Word name_;

		Word typeName_;

		Expression() {}

		Expression(Index position, Word name) {position_ = position; name_ = name;}

		Expression(Index position, Word name, Word typeName) {position_ = position; name_ = name; typeName_ = typeName;}
	};

	static bool isTracing_;

	Shader* shader_;

	Array <Expression> samplerExpressions_;

	Array <Expression> constantExpressions_;

	void Parse();

	void Parse(ShaderFile*);

	void Setup();

	void LocateSamplers();

	void LocateConstants();

public:
	ShaderParser() {}

	ShaderParser(Shader*);

	Array <Expression> &FetchTextures();

	Array <Expression> &FetchConstants();

	~ShaderParser();
};
