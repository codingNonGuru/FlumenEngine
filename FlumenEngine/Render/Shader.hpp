#pragma once

#include "FlumenCore/Conventions.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture;
class TextureBinding;
class ConstantBinding;
class ShaderFile;

class Shader
{
private:
	GLuint key_;

	Word name_;

	GLuint shaderKeys_[3];

	Array <ShaderFile*> *shaderFiles_;

	Map <TextureBinding, Word> textureBindings_;

	Map <ConstantBinding, Word> constantBindings_;

	void Compile(int, GLenum);

	void Link();

	void Parse();

public:
	Shader() {}

	Shader(const char*, Array <ShaderFile*> &);

	void Initialize(const char*, Array <ShaderFile*> &);

	void Bind();

	void Unbind();

	bool BindTexture(Texture*, const char*);

	ConstantBinding* GetConstant(const char*);

	void SetConstant(int, const char*);

	void SetConstant(unsigned int, const char*);

	void SetConstant(float, const char*);

	void SetConstant(glm::ivec2, const char*);

	void SetConstant(glm::uvec2, const char*);

	void SetConstant(glm::vec2, const char*);

	void SetConstant(glm::ivec3, const char*);

	void SetConstant(glm::uvec3, const char*);

	void SetConstant(glm::vec3, const char*);

	void SetConstant(glm::ivec4, const char*);

	void SetConstant(glm::uvec4, const char*);

	void SetConstant(glm::vec4, const char*);

	void SetConstant(glm::mat3, const char*);

	void SetConstant(glm::mat4, const char*);

	void Update();

	Length GetTextureCount() const;

	GLuint GetTextureLocation(const char*);

	Array <ShaderFile*> *GetShaderFiles() {return shaderFiles_;}

	Word& GetName() {return name_;}

	void DispatchCompute(Size);

	void DispatchCompute(Length);

	~Shader(void);
};

