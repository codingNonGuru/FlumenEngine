#pragma once

#include "FlumenCore/Conventions.hpp"

class Shader;
class ShaderFile;

class ShaderManager
{
private:
	static Array <ShaderFile> shaderFiles_;

	static Map <Shader, Word> shaders_;

public:
	static void LoadShaders();

	static Map <Shader, Word> &GetShaderMap() {return shaders_;}

	static Shader* GetShader(const char*);
};
