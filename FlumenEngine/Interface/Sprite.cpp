#include <stdlib.h>
#include "GL/glew.h"

#include "Sprite.hpp"
#include "FlumenEngine/Render/Texture.hpp"
#include "FlumenEngine/Render/TextureManager.hpp"
#include "FlumenEngine/Render/Camera.hpp"
#include "Element.hpp"
#include "FlumenEngine/Render/Model.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Core/Transform.hpp"
#include "FlumenEngine/Utility/Color.hpp"
#include "FlumenEngine/Core/File.hpp"
#include "FlumenEngine/Config.h"

Sprite::Sprite()
{
	isActive_ = true;

	parent_ = nullptr;
}

Sprite::Sprite(Shader* shader, TextureData textureData)
{
	Initialize(shader, textureData);
}

Sprite::Sprite(Shader *shader, Word textureName)
{
	auto texture = render::TextureManager::GetTexture(textureName);

	Initialize(shader, texture);
}

void Sprite::Initialize(Shader* shader, TextureData textureData)
{
	isActive_ = true;

	textureData_ = textureData;

	shader_ = shader;

	parent_ = nullptr;

	opacity_ = Opacity(1.0f);

	if(shader_->GetName() == "SlicedSprite")
	{
		isSliced_ = true;
	}
}

void Sprite::Draw(Camera* camera, const SpriteDrawData data)
{
	shader_->Bind();

	static auto emptyData = SpriteDrawData();
	SetDefaultConstants(camera, parent_ ? &emptyData : &data);

	SetExtraConstants();

	BindDefaultTextures();

	BindExtraTextures();

	if(isSliced_ == true)
	{
		glDrawArrays(GL_TRIANGLES, 0, 54);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	shader_->Unbind();

	if(textureData_.Texture)
	{
		textureData_.Texture->Unbind();
	}
}

void Sprite::DrawStandalone(Camera* camera, const SpriteDrawData data)
{
	shader_->Bind();

	shader_->SetConstant(camera->GetMatrix(), "viewMatrix");

	shader_->SetConstant(data.Position, "spritePosition");

	auto size = (Scale2)textureData_.Texture->GetSize() * data.Size;
	shader_->SetConstant(size, "spriteSize");

	static const auto DRAW_LAYER_COUNT = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::INTERFACE_DRAW_LAYER_COUNT).Integer;

	auto drawOrder = data.Depth / (float)DRAW_LAYER_COUNT;
	shader_->SetConstant(drawOrder, "depth");

	shader_->SetConstant(int(1), "hasTexture");

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

Opacity &Sprite::GetOpacity()
{
	return opacity_;
}

void Sprite::SetDefaultConstants(Camera* camera, const SpriteDrawData *newData)
{
	auto data = (drawData_ != nullptr ? drawData_ : newData);

	shader_->SetConstant(camera->GetMatrix(), "viewMatrix");

	shader_->SetConstant(parent_ ? parent_->GetGlobalPosition() : data->Position, "spritePosition");

	auto size = [this, data] () -> Scale2
	{
		if(isSliced_ == true)
		{
			if(parent_)
			{
				auto scale = parent_->GetTransform()->GetScale() * data->Size;
				
				return scale * (Scale2)parent_->GetSize() * textureData_.Scale;
			}
			else
			{
				return data->Size;
			}
		}
		else
		{
			if(parent_)
			{
				auto scale = parent_->GetTransform()->GetScale() * data->Size;
				if(textureData_.Texture)
					return scale * (Scale2)textureData_.Texture->GetSize() * textureData_.Scale;
				else
					return scale * (Scale2)parent_->GetSize();
			}
			else
			{
				if(textureData_.Texture)
					return (Scale2)textureData_.Texture->GetSize() * data->Size * textureData_.Scale;
				else
					return data->Size;
			}
		}
	} ();
	shader_->SetConstant(size, "spriteSize");

	Opacity opacity = parent_ ? opacity_ * parent_->GetOpacity() : data->Opacity;
	shader_->SetConstant(opacity, "opacity");

	static const auto DRAW_LAYER_COUNT = engine::ConfigManager::Get()->GetValue(engine::ConfigValues::INTERFACE_DRAW_LAYER_COUNT).Integer;

	auto drawOrder = parent_ ? (float)parent_->GetDrawOrder() / (float)DRAW_LAYER_COUNT : data->Depth;
	shader_->SetConstant(drawOrder, "depth");

	if(shader_->GetName() == "Sprite")
	{
		shader_->SetConstant(textureData_.Texture != nullptr ? 1 : 0, "hasTexture");
	}

	auto color = const_cast <Color *>(color_);
	shader_->SetConstant(color_ != nullptr ? *color : Color::WHITE, "color");

	if(isSliced_ == true)
	{
		shader_->SetConstant(color_ != nullptr ? 1 : 0, "hasColor");

		shader_->SetConstant(sliceCornerSize_, "sliceCornerSize");
	}
}

void Sprite::SetTexture(const render::Texture* texture) 
{
	textureData_.Texture = texture;
}

void Sprite::SetTexture(const Word textureName)
{
	textureData_.Texture = render::TextureManager::GetTexture(textureName);
}

void Sprite::BindDefaultTextures()
{
	if(textureData_.Texture == nullptr)
		return;
	
	shader_->BindTexture(textureData_.Texture, "diffuse");
}

void Sprite::BindTexture(const char *name)
{
	if(textureData_.Texture == nullptr)
		return;

	shader_->BindTexture(textureData_.Texture, name);
}

void Sprite::SetExtraConstants()
{
}

void Sprite::BindExtraTextures()
{
}
