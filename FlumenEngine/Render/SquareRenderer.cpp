#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/ShaderManager.hpp"
#include "FlumenEngine/Render/Camera.hpp"

#include "SquareRenderer.h"

using namespace engine::render;

void SquareRenderer::DrawSquare(Camera *camera, Position2 position, Scale2 scale, float rotation, Float4 color, float opacity, float depth)
{
    static const auto shader = ShaderManager::GetShader("Square");

    shader->Bind();

    shader->SetConstant(camera->GetMatrix(), "viewMatrix");

	shader->SetConstant(opacity, "opacity");

	shader->SetConstant(depth, "depth");

    shader->SetConstant(position, "hexPosition");

    shader->SetConstant(scale, "hexSize");

    shader->SetConstant(color, "color");

    shader->SetConstant(rotation, "rotation");

    glDrawArrays(GL_TRIANGLES, 0, 6);

    shader->Unbind();
}