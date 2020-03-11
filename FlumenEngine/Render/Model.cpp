#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

#include "FlumenEngine/Render/Model.hpp"
#include "Mesh.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/Camera.hpp"
#include "HeaderBuffer.hpp"
#include "FlumenEngine/Render/DataBuffer.hpp"

enum class Shaders {GENERIC};

Model::Model() {}

Model::Model(Mesh* mesh, Shader* shader)
{
	Initialize(mesh, shader);
}

void Model::Initialize() {}

void Model::Initialize(Mesh* mesh, Shader* shader)
{
	meshes_.Initialize(1);
	*meshes_.Add(0) = mesh;

	shaders_.Initialize(1);
	*shaders_.Add(Shaders::GENERIC) = shader;

	textures_.Initialize(shader->GetTextureCount());

	SetupBuffer();
}

void Model::AddTexture(Texture* texture, const char* name)
{
	auto texturePointer = textures_.Add(name);
	*texturePointer = texture;
}

void Model::Render(Camera* camera, Light* light)
{
	auto shader = *shaders_.Get(Shaders::GENERIC);

	shader->Bind();

	shader->SetConstant(camera->GetMatrix(), "viewMatrix");

	auto mesh = *meshes_.Get(0);

	auto indexCount = mesh->GetIndexCount();
	shader->SetConstant(indexCount, "indexCount");

	auto vertexCount = mesh->GetVertexCount();
	shader->SetConstant(vertexCount, "vertexCount");

	auto cameraPosition = camera->GetPosition();
	shader->SetConstant(cameraPosition, "cameraPosition");

	DataBuffer* buffer = nullptr;

	buffer = *buffers_.Get("position");
	buffer->Bind(0);

	buffer = *buffers_.Get("normal");
	buffer->Bind(1);

	buffer = *buffers_.Get("index");
	buffer->Bind(2);

	glDrawArrays(GL_TRIANGLES, 0, indexCount);

	shader->Unbind();
}

void Model::SetupBuffer()
{
	auto mesh = *meshes_.Get(0);

	buffers_.Initialize(4);

	int index = 0;
	auto meshAttributes = mesh->GetAttributes();
	auto meshAttributeKey = meshAttributes.GetFirstKey();
	for(auto meshAttribute = meshAttributes.GetStart(); meshAttribute != meshAttributes.GetEnd(); ++meshAttribute, ++index, ++meshAttributeKey)
	{
		auto meshAttributeData = meshAttribute->GetData();
		if(meshAttributeData == nullptr)
			continue;

		auto buffer = new DataBuffer(meshAttributeData->GetMemoryCapacity(), meshAttributeData->GetData());
		*buffers_.Add(*meshAttributeKey) = buffer;
	}
}
