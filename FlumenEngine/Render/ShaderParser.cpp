#include <stdio.h>

#include "GL/glew.h"

#include "ShaderParser.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "ShaderUtility.hpp"

bool ShaderParser::isTracing_ = true;

ShaderParser::ShaderParser(Shader* shader)
{
	if(isTracing_)
	{
		std::cout<<"Parsing Shader: "<<shader->GetName()<<"\n";
	}

	shader_ = shader;

	Setup();

	Parse();

	if(isTracing_)
	{
		for(auto expression = samplerExpressions_.GetStart(); expression != samplerExpressions_.GetEnd(); ++expression)
		{
			std::cout<<"Texture has Identifier <"<<expression->name_<<">\n";
		}
		std::cout<<"\n";

		for(auto expression = constantExpressions_.GetStart(); expression != constantExpressions_.GetEnd(); ++expression)
		{
			std::cout<<"Constant has Identifier <"<<expression->name_<<"> & Type Name <"<<expression->typeName_<<">\n";
		}
		std::cout<<"\n";
	}
}

void ShaderParser::Parse()
{
	if(shader_ == nullptr)
		return;

	auto shaderFiles = shader_->GetShaderFiles();
	for(auto shaderFile = shaderFiles->GetStart(); shaderFile != shaderFiles->GetEnd(); ++shaderFile)
	{
		Parse(*shaderFile);
	}
}

int fileSize = 0;

char* shaderCode = nullptr;

void ShaderParser::Parse(ShaderFile* shaderFile)
{
	FILE* file = fopen(shaderFile->GetPath(), "r");
	if(!file)
		return;

	fseek(file, 0 , SEEK_END);
	fileSize = ftell(file);
	rewind(file);

	shaderCode = new char[fileSize];
	fread(shaderCode, 1, fileSize, file);

	LocateSamplers();

	LocateConstants();

	delete[] shaderCode;
	fclose(file);
}

void ShaderParser::Setup()
{
	int maximumSamplerCount = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maximumSamplerCount);

	samplerExpressions_.Initialize(maximumSamplerCount);

	constantExpressions_.Initialize(32);
}

Index FindNextSymbol(char *codeIterator, char symbol)
{
	for(auto character = codeIterator; character != shaderCode + fileSize; ++character)
	{
		if(*character == symbol)
			return character - shaderCode;
	}
}

Index JumpSymbol(char* codeIterator, char symbol)
{
	for(auto character = codeIterator; character != shaderCode + fileSize; ++character)
	{
		if(*character == symbol)
			continue;

		return character - shaderCode;
	}
}

bool isSingleComment = false;
bool isMultiComment = false;

void CheckCommentary(char* codeIterator)
{
	if(CompareMemory(codeIterator, "//", 2) == 0)
	{
		isSingleComment = true;
	}

	if(CompareMemory(codeIterator, "\n", 1) == 0)
	{
		isSingleComment = false;
	}

	if(CompareMemory(codeIterator, "/*", 2) == 0)
	{
		isMultiComment = true;
	}

	if(CompareMemory(codeIterator, "*/", 2) == 0)
	{
		isMultiComment = false;
	}
}

void ShaderParser::LocateSamplers()
{
	isSingleComment = isMultiComment = false;
	for(auto character = shaderCode; character != shaderCode + fileSize; ++character)
	{
		CheckCommentary(character);

		if(isMultiComment || isSingleComment)
			continue;

		const char* typeName = "sampler2D";
		if(CompareMemory(character, typeName, GetStringLength(typeName)) != 0)
			continue;

		character += GetStringLength(typeName);

		character = shaderCode + FindNextSymbol(character, ' ');

		Index identifierPosition = JumpSymbol(character, ' ');

		character = shaderCode + identifierPosition;

		Index semicolonPosition = FindNextSymbol(character, ';');

		Word identifier;
		identifier.Add(character, semicolonPosition - identifierPosition);

		bool hasFound = false;
		for(auto expression = samplerExpressions_.GetStart(); expression != samplerExpressions_.GetEnd(); ++expression)
		{
			if(expression->position_ != identifierPosition)
				continue;

			hasFound = true;
		}

		if(hasFound)
		{
			character = shaderCode + semicolonPosition;
			continue;
		}

		*samplerExpressions_.Allocate() = Expression(identifierPosition, identifier);
	}
}

void ShaderParser::LocateConstants()
{
	isSingleComment = isMultiComment = false;
	for(auto character = shaderCode; character != shaderCode + fileSize; ++character)
	{
		CheckCommentary(character);

		if(isMultiComment || isSingleComment)
			continue;

		const char* labelName = "uniform";
		if(CompareMemory(character, labelName, GetStringLength(labelName)) != 0)
			continue;

		character += GetStringLength(labelName);

		if(*character != ' ')
			continue;

		character = shaderCode + FindNextSymbol(character, ' ');

		Index typeStartPosition = JumpSymbol(character, ' ');

		character = shaderCode + typeStartPosition;

		if(*character == 's')
			continue;

		Index typeEndPosition = FindNextSymbol(character, ' ');

		Word typeName;
		typeName.Add(character, typeEndPosition - typeStartPosition);

		character = shaderCode + typeEndPosition;

		Index identifierStartPosition = JumpSymbol(character, ' ');

		character = shaderCode + identifierStartPosition;

		Index semiColonPosition = FindNextSymbol(character, ';');

		Word identifier;
		identifier.Add(character, semiColonPosition - identifierStartPosition);

		bool hasFound = false;
		for(auto expression = constantExpressions_.GetStart(); expression != constantExpressions_.GetEnd(); ++expression)
		{
			if(expression->position_ != identifierStartPosition)
				continue;

			hasFound = true;
		}

		if(hasFound)
		{
			character = shaderCode + semiColonPosition;
			continue;
		}

		*constantExpressions_.Allocate() = Expression(identifierStartPosition, identifier, typeName);
	}
}

Array <ShaderParser::Expression> &ShaderParser::FetchTextures()
{
	return samplerExpressions_;
}

Array <ShaderParser::Expression> &ShaderParser::FetchConstants()
{
	return constantExpressions_;
}

ShaderParser::~ShaderParser()
{
	samplerExpressions_.Destroy();

	constantExpressions_.Destroy();
}
