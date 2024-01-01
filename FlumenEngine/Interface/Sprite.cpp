#include <stdlib.h>
#include "GL/glew.h"

#include "Sprite.hpp"
#include "FlumenEngine/Render/Texture.hpp"
#include "FlumenEngine/Render/Camera.hpp"
#include "Element.hpp"
#include "FlumenEngine/Render/Model.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Core/Transform.hpp"

Sprite::Sprite()
{
	isActive_ = true;

	parent_ = nullptr;
}

Sprite::Sprite(Shader* shader, TextureData textureData)
{
	Initialize(shader, textureData);
}

void Sprite::Initialize(Shader* shader, TextureData textureData)
{
	isActive_ = true;

	textureData_ = textureData;

	shader_ = shader;

	parent_ = nullptr;

	opacity_ = 1.0f;
}

void Sprite::Draw(Camera* camera, const SpriteDrawData data = SpriteDrawData())
{
	shader_->Bind();

	SetDefaultConstants(camera, parent_ ? nullptr : &data);

	SetExtraConstants();

	BindDefaultTextures();

	BindExtraTextures();

	glDrawArrays(GL_TRIANGLES, 0, 6);

	shader_->Unbind();

	if(textureData_.Texture)
	{
		textureData_.Texture->Unbind();
	}
}

float & Sprite::GetOpacity()
{
	return opacity_;
}

void Sprite::SetDefaultConstants(Camera* camera, const SpriteDrawData *data)
{
	shader_->SetConstant(camera->GetMatrix(), "viewMatrix");

	shader_->SetConstant(parent_ ? parent_->GetGlobalPosition() : data->Position, "spritePosition");

	auto size = [this, data]
	{
		if(parent_)
		{
			auto scale = parent_->GetTransform()->GetScale();
			if(textureData_.Texture)
				return scale * (Scale2)textureData_.Texture->GetSize();
			else
				return scale * (Scale2)parent_->GetSize();
		}
		else
		{
			if(textureData_.Texture)
				return (Scale2)textureData_.Texture->GetSize() * data->Size;
			else
				return data->Size;
		}
	} ();
	shader_->SetConstant(size, "spriteSize");

	Opacity opacity = parent_ ? opacity_ * parent_->GetOpacity() : data->Opacity;
	shader_->SetConstant(opacity, "opacity");

	auto drawOrder = parent_ ? (float)parent_->GetDrawOrder() * 0.1f : data->Depth;
	shader_->SetConstant(drawOrder, "depth");

	shader_->SetConstant(textureData_.Texture ? 1 : 0, "hasTexture");

	shader_->SetConstant(textureData_.Offset, "textureOffset");

	shader_->SetConstant(textureData_.Scale, "textureScale");
}

void Sprite::BindDefaultTextures()
{
	if(textureData_.Texture)
	{
		shader_->BindTexture(textureData_.Texture, "diffuse");
	}
}

void Sprite::SetExtraConstants()
{
}

void Sprite::BindExtraTextures()
{
}
