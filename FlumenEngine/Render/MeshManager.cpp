#include "FlumenEngine/Render/MeshManager.hpp"

#include "FlumenEngine/Core/AssetManager.hpp"
#include "FlumenEngine/Core/File.hpp"
#include "Mesh.hpp"

Map <Mesh, Word> MeshManager::meshes_ = Map <Mesh, Word>();

#define MAXIMUM_MESH_COUNT 256

void MeshManager::Initialize()
{
}

void MeshManager::LoadMeshes()
{
	meshes_.Initialize(MAXIMUM_MESH_COUNT);

	auto files = AssetManager::GetFiles();

	for(File* file = files->GetStart(); file != files->GetEnd(); ++file)
	{
		auto meshExtension = ".mesh";

		auto extensionPosition = FindString(file->GetName(), meshExtension);
		if(extensionPosition == nullptr)
			continue;

		Word meshName;
		meshName.Add(file->GetName(), extensionPosition - file->GetName());

		auto mesh = meshes_.Add(meshName);
		if(mesh == nullptr)
			continue;

		mesh->Initialize(file);
	}
}

Map <Mesh, Word> & MeshManager::GetMeshes()
{
	return meshes_;
}

Mesh* MeshManager::GetMesh(const char* identifier)
{
	return meshes_.Get(identifier);
}
