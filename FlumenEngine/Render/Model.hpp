#pragma once

#include "FlumenCore/Conventions.hpp"

class Mesh;
class DataBuffer;
class Camera;
class Texture;
class Shader;
class Light;

enum class Shaders;

class Model
{
protected:
	bool isInitialized_;

	Map <Mesh*, Index> meshes_;

	Map <DataBuffer*, Word> buffers_;

	Map <Shader*, Shaders> shaders_;

	Map <Texture*, Word> textures_;

	void SetupBuffer();

public:
	Model();

	Model(Mesh*, Shader*);

	virtual void Initialize();

	void Initialize(Mesh*, Shader*);

	void AddTexture(Texture*, const char*);

	virtual void Render(Camera*, Light*);
};
