#include "FlumenEngine/Render/DataBuffer.hpp"
#include "FlumenEngine/Render/ShaderManager.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/Camera.hpp"
#include "FlumenEngine/Render/RenderManager.hpp"

#include "HexRenderer.h"

using namespace engine::render;

void HexRenderer::RenderHex(Camera * camera, Position2 position, float size, float opacity, Float4 color)
{
    static auto hexShader = ShaderManager::GetShader("Hex");

    hexShader->Bind();

	hexShader->SetConstant(camera->GetMatrix(), "viewMatrix");

    hexShader->SetConstant(0.0f, "depth");

    hexShader->SetConstant(opacity, "opacity");

    hexShader->SetConstant(position, "hexPosition");

    hexShader->SetConstant(size, "hexSize");

    hexShader->SetConstant(color, "color");

    glDrawArrays(GL_TRIANGLES, 0, 18);

    hexShader->Unbind();
}

void HexRenderer::RenderEmptyHex(Camera * camera, Position2 position, float size, float opacity, Float4 color, float depth)
{
    static auto hexShader = ShaderManager::GetShader("EmptyHex");

    hexShader->Bind();

	hexShader->SetConstant(camera->GetMatrix(), "viewMatrix");

    hexShader->SetConstant(depth, "depth");

    hexShader->SetConstant(opacity, "opacity");

    hexShader->SetConstant(position, "hexPosition");

    hexShader->SetConstant(size, "hexSize");

    hexShader->SetConstant(color, "color");

    glDrawArrays(GL_TRIANGLES, 0, 18);

    hexShader->Unbind();
}