#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "GL/glew.h"

#include "FlumenEngine/Render/Shader.hpp"
#include "ShaderParser.hpp"
#include "ShaderUtility.hpp"
#include "FlumenEngine/Render/Texture.hpp"

// ----- SHADER Definitions

Shader::Shader(const char* name, Array <ShaderFile*> &files)
{
	Initialize(name, files);
}

void Shader::Initialize(const char* name, Array <ShaderFile*> &shaderFiles)
{
	shaderFiles_ = &shaderFiles;
	name_.Add(name);

	Index index = 0;
	for(auto shaderFile = shaderFiles.GetStart(); shaderFile != shaderFiles.GetEnd(); ++shaderFile, ++index)
	{
		Compile(index, (GLenum)(*shaderFile)->GetType());
	}

	Link();

	Parse();
}

void Shader::Bind()
{
	glUseProgram(key_);

	DEBUG_OPENGL
}

void Shader::Unbind()
{
	glUseProgram(0);
	glFinish();

	DEBUG_OPENGL
}

bool Shader::BindTexture(Texture* texture, const char* identifier)
{
	auto binding = textureBindings_.Get(identifier);
	if(!binding)
	{
		std::cout<<"Texture identifier "<<identifier<<" is not valid.\n";
		return false;
	}

	glUniform1i(binding->GetLocation(), binding->GetUnitIndex());
	glActiveTexture(GL_TEXTURE0 + binding->GetUnitIndex());
	texture->Bind();

	DEBUG_OPENGL

	return true;
}

void Shader::Compile(int index, GLenum shaderType)
{
	GLuint key = glCreateShader(shaderType);
	shaderKeys_[index] = key;

	auto shaderFile = *shaderFiles_->Get(index);

	std::ifstream fileStream(shaderFile->GetPath());
	std::string fileContent = "";
	std::string line;
	while(std::getline(fileStream, line))
		fileContent += line + "\n";

	fileStream.close();
	const char* fC = fileContent.c_str();

	glShaderSource(key, 1, &fC, nullptr);
	glCompileShader(key);

	GLint isCompiled = 0;
	glGetShaderiv(key, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(key, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar errorLog[maxLength];
		glGetShaderInfoLog(key, maxLength, &maxLength, &errorLog[0]);
		for(int i = 0; i < maxLength; ++i)
		{
			std::cout<<errorLog[i];
		}
		glDeleteShader(key);
		return;
	}

	DEBUG_OPENGL
}

void Shader::Link()
{
	key_ = glCreateProgram();

	for(int i = 0; i < shaderFiles_->GetSize(); ++i)
	{
		glAttachShader(key_, shaderKeys_[i]);
	}

	glLinkProgram(key_);
	glValidateProgram(key_);

	GLint isLinked = 0;
	glGetProgramiv(key_, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(key_, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar infoLog[maxLength];
		glGetProgramInfoLog(key_, maxLength, &maxLength, &infoLog[0]);
		for(int i = 0; i < maxLength; ++i)
		{
			std::cout<<infoLog[i];
		}
		glDeleteProgram(key_);
		for(int i = 0; i < shaderFiles_->GetSize(); ++i)
		{
			glDeleteShader(shaderKeys_[i]);
		}
		return;
	}

	for(int i = 0; i < shaderFiles_->GetSize(); ++i)
	{
		glDetachShader(key_, shaderKeys_[i]);
	}

	DEBUG_OPENGL
}

ConstantBinding* Shader::GetConstant(const char* identifier)
{
	return constantBindings_.Get(identifier);
}

void Shader::SetConstant(int value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	int castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_1:
		castValue = (float)value;
		glUniform1fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_1:
		castValue = (int)value;
		glUniform1iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_1:
		castValue = (unsigned int)value;
		glUniform1uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(unsigned int value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	unsigned int castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_1:
		castValue = (float)value;
		glUniform1fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_1:
		castValue = (int)value;
		glUniform1iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_1:
		castValue = (unsigned int)value;
		glUniform1uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(float value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	float castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_1:
		castValue = (float)value;
		glUniform1fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_1:
		castValue = (int)value;
		glUniform1iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_1:
		castValue = (unsigned int)value;
		glUniform1uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::vec2 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::vec2 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_2:
		castValue = glm::vec2(value.x, value.y);
		glUniform2fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_2:
		castValue = glm::ivec2(value.x, value.y);
		glUniform2iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_2:
		castValue = glm::uvec2(value.x, value.y);
		glUniform2uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::ivec2 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::ivec2 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_2:
		castValue = glm::vec2(value.x, value.y);
		glUniform2fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_2:
		castValue = glm::ivec2(value.x, value.y);
		glUniform2iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_2:
		castValue = glm::uvec2(value.x, value.y);
		glUniform2uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::uvec2 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::uvec2 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_2:
		castValue = glm::vec2(value.x, value.y);
		glUniform2fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_2:
		castValue = glm::ivec2(value.x, value.y);
		glUniform2iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_2:
		castValue = glm::uvec2(value.x, value.y);
		glUniform2uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::vec3 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::vec3 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_2:
		castValue = Float3(value.x, value.y, value.z);
		glUniform2fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::FLOAT_3:
		castValue = Float3(value.x, value.y, value.z);
		glUniform3fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_3:
		castValue = Integer3(value.x, value.y, value.z);
		glUniform3iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_3:
		castValue = glm::uvec3(value.x, value.y, value.z);
		glUniform3uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::ivec3 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::ivec3 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_3:
		castValue = glm::vec3(value.x, value.y, value.z);
		glUniform3fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_3:
		castValue = glm::ivec3(value.x, value.y, value.z);
		glUniform3iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_3:
		castValue = glm::uvec3(value.x, value.y, value.z);
		glUniform3uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::uvec3 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::uvec3 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_3:
		castValue = glm::vec3(value.x, value.y, value.z);
		glUniform3fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_3:
		castValue = glm::ivec3(value.x, value.y, value.z);
		glUniform3iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_3:
		castValue = glm::uvec3(value.x, value.y, value.z);
		glUniform3uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::vec4 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::vec4 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_3:
		castValue = Float4(value.x, value.y, value.z, 0.0f);
		glUniform3fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::FLOAT_4:
		castValue = Float4(value.x, value.y, value.z, value.w);
		glUniform4fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_4:
		castValue = glm::ivec4(value.x, value.y, value.z, value.w);
		glUniform4iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_4:
		castValue = glm::uvec4(value.x, value.y, value.z, value.w);
		glUniform4uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type "<<(int)binding->GetType()<<".\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::ivec4 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::ivec4 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_4:
		castValue = glm::vec4(value.x, value.y, value.z, value.w);
		glUniform4fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_4:
		castValue = glm::ivec4(value.x, value.y, value.z, value.w);
		glUniform4iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_4:
		castValue = glm::uvec4(value.x, value.y, value.z, value.w);
		glUniform4uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::uvec4 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	glm::uvec4 castValue;
	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_4:
		castValue = glm::vec4(value.x, value.y, value.z, value.w);
		glUniform4fv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::INTEGER_4:
		castValue = glm::ivec4(value.x, value.y, value.z, value.w);
		glUniform4iv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	case AttributeElementTypes::UINTEGER_4:
		castValue = glm::uvec4(value.x, value.y, value.z, value.w);
		glUniform4uiv(binding->GetLocation(), 1, (void*)&castValue);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::mat3 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	switch(binding->GetType())
	{
	case AttributeElementTypes::MATRIX_3:
		glUniformMatrix3fv(binding->GetLocation(), 1, GL_FALSE, (void*)&value);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

void Shader::SetConstant(glm::mat4 value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	switch(binding->GetType())
	{
	case AttributeElementTypes::MATRIX_4:
		glUniformMatrix4fv(binding->GetLocation(), 1, GL_FALSE, (void*)&value);
		break;
	default:
		std::cout<<"Constant identifier "<<identifier<<" corresponds to different type.\n";
		break;
	}

	DEBUG_OPENGL
}

/*void Shader::SetConstant(void* value, const char* identifier)
{
	auto binding = constantBindings_.Get(identifier);
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_1:
		glUniform1fv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::FLOAT_2:
		glUniform2fv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::FLOAT_3:
		glUniform3fv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::FLOAT_4:
		glUniform4fv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::INTEGER_1:
		glUniform1iv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::INTEGER_2:
		glUniform2iv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::INTEGER_3:
		glUniform3iv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::INTEGER_4:
		glUniform4iv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::UINTEGER_1:
		glUniform1uiv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::UINTEGER_2:
		glUniform2uiv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::UINTEGER_3:
		glUniform3uiv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::UINTEGER_4:
		glUniform4uiv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::MATRIX_3:
		glUniformMatrix3fv(binding->GetLocation(), 1, GL_FALSE, value);
		break;
	case AttributeElementTypes::MATRIX_4:
		glUniformMatrix4fv(binding->GetLocation(), 1, GL_FALSE, value);
		break;
	}

	DEBUG_OPENGL
}*/

void Shader::Update()
{
	/*glDeleteProgram(key_);
	for(int i = 0; i < shaderFiles_->getSize(); ++i)
	{
		glDeleteShader(shaderKeys_[i]);
	}

	if(shaderFiles_.getSize() == 1)
	{
		Compile(0, GL_COMPUTE_SHADER);
	}
	else if(shaderFiles_.getSize() == 2)
	{
		Compile(0, GL_VERTEX_SHADER);
		Compile(1, GL_FRAGMENT_SHADER);
	}
	else if(shaderFiles_.getSize() == 3)
	{
		Compile(0, GL_VERTEX_SHADER);
		Compile(1, GL_FRAGMENT_SHADER);
		Compile(2, GL_GEOMETRY_SHADER);
	}
	Link();*/
}

Length Shader::GetTextureCount() const
{
	return textureBindings_.GetSize();
}

GLuint Shader::GetTextureLocation(const char* textureName)
{
	return glGetUniformLocation(key_, textureName);
}

void Shader::Parse()
{
	auto parser = new ShaderParser(this);

	auto textureExpressions = parser->FetchTextures();

	textureBindings_.Initialize(textureExpressions.GetSize());
	Index index = 0;
	for(auto expression = textureExpressions.GetStart(); expression != textureExpressions.GetEnd(); ++expression, ++index)
	{
		Index location = glGetUniformLocation(key_, expression->name_);

		*textureBindings_.Add(expression->name_) = TextureBinding(index, location);
	}

	auto constantExpressions = parser->FetchConstants();

	constantBindings_.Initialize(constantExpressions.GetSize());
	index = 0;
	for(auto expression = constantExpressions.GetStart(); expression != constantExpressions.GetEnd(); ++expression, ++index)
	{
		Index location = glGetUniformLocation(key_, expression->name_);

		auto& typeName = expression->typeName_;
		AttributeElementTypes type;
		if(CompareMemory(typeName, "int", 3) == 0)
			type = AttributeElementTypes::INTEGER_1;
		else if(CompareMemory(typeName, "ivec2", 5) == 0)
			type = AttributeElementTypes::INTEGER_2;
		else if(CompareMemory(typeName, "ivec3", 5) == 0)
			type = AttributeElementTypes::INTEGER_3;
		else if(CompareMemory(typeName, "ivec4", 5) == 0)
			type = AttributeElementTypes::INTEGER_4;
		else if(CompareMemory(typeName, "uint", 4) == 0)
			type = AttributeElementTypes::UINTEGER_1;
		else if(CompareMemory(typeName, "uvec2", 5) == 0)
			type = AttributeElementTypes::UINTEGER_2;
		else if(CompareMemory(typeName, "uvec3", 5) == 0)
			type = AttributeElementTypes::UINTEGER_3;
		else if(CompareMemory(typeName, "uvec4", 5) == 0)
			type = AttributeElementTypes::UINTEGER_4;
		else if(CompareMemory(typeName, "float", 5) == 0)
			type = AttributeElementTypes::FLOAT_1;
		else if(CompareMemory(typeName, "vec2", 4) == 0)
			type = AttributeElementTypes::FLOAT_2;
		else if(CompareMemory(typeName, "vec3", 4) == 0)
			type = AttributeElementTypes::FLOAT_3;
		else if(CompareMemory(typeName, "vec4", 4) == 0)
			type = AttributeElementTypes::FLOAT_4;
		else if(CompareMemory(typeName, "mat3", 4) == 0)
			type = AttributeElementTypes::MATRIX_3;
		else if(CompareMemory(typeName, "mat4", 4) == 0)
			type = AttributeElementTypes::MATRIX_4;

		*constantBindings_.Add(expression->name_) = ConstantBinding(location, type);
	}

	delete parser;
}

void Shader::DispatchCompute(Size computeSize)
{
	glDispatchCompute(computeSize.x, computeSize.y, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	DEBUG_OPENGL
}

void Shader::DispatchCompute(Length computeSize)
{
	glDispatchCompute(computeSize, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	DEBUG_OPENGL
}

Shader::~Shader(void) {}
