#pragma once

#include "FlumenCore/Conventions.hpp"

class Mesh;
class DataBuffer;
class Camera;
class Texture;
class Shader;
class Light;

enum class Shaders;

struct ModelData {};

class Model
{
protected:
	bool isInitialized_;

	Map <Mesh*, Index> meshes_;

	Map <DataBuffer*, Word> buffers_;

	Map <Shader*, Shaders> shaders_;

	Map <Texture*, Word> textures_;

	void SetupBuffer();

	Shader *BindShader();

	Shader *GetShader();

public:
	Model();

	Model(Mesh*, Shader*);

	Model(Mesh*, Word);

	Model(Word, Word);

	virtual void Initialize();

	void Initialize(Mesh*, Shader*);

	void Initialize(Mesh*, Word);

	void AddTexture(Texture*, const char*);

	virtual void Render(Camera*, Light*);

	virtual void Render(Camera*, ModelData *) {}
};
