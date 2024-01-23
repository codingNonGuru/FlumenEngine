#pragma once

#include <glm/glm.hpp>

#include "FlumenCore/Conventions.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace render
{
	class Texture;
}
class Element;
class Shader;
class Camera;
class Model;
class Color;

struct SpriteDrawData
{
	Position2 Position {0.0f, 0.0f};

	Scale2 Size {1.0f, 1.0f};

	Opacity Opacity {1.0f};

	DrawOrder Depth {0};
};

struct TextureData
{
	const render::Texture *Texture {nullptr};

	Position2 Offset = {0.0f, 0.0f};

	Scale2 Scale = {1.0f, 1.0f};

	TextureData() : Texture(nullptr) {}

	TextureData(render::Texture *texture) : Texture(texture) {}

	TextureData(render::Texture *texture, Position2 offset, Scale2 scale) : Texture(texture), Offset(offset), Scale(scale) {}
};

class Sprite
{
private:
	bool isActive_;

	Opacity opacity_;

	float depth_;

	TextureData textureData_;

	Shader* shader_;

	Element* parent_;

	const Color *color_ {nullptr};

	SpriteDrawData *drawData_ {nullptr};

	bool isSliced_ {false};

	Scale2 sliceCornerSize_ {16.0f, 16.0f};

	void SetDefaultConstants(Camera *, const SpriteDrawData *);

protected:
	virtual void SetExtraConstants();

	virtual void BindExtraTextures();

public:
	Sprite();

	Sprite(Shader *, TextureData = TextureData());

	Sprite(Shader *, Word);

	void Initialize(Shader *, TextureData = TextureData());

	void Draw(Camera*, const SpriteDrawData = SpriteDrawData());

	void BindDefaultTextures();

	void SetTexture(const render::Texture* texture);

	void SetTexture(const Word textureName);

	void SetParent(Element* parent) {parent_ = parent;}

	void SetColor(const Color *color) {color_ = color;}

	void SetDrawData(SpriteDrawData *drawData) {drawData_ = drawData;}

	void SetSlice(Scale2 sliceCornerSize) 
	{
		sliceCornerSize_ = sliceCornerSize;
		isSliced_ = true;
	}

	void SetTextureSize(Scale2 scale) {textureData_.Scale = scale;}

	Scale2 GetSlice() const {return sliceCornerSize_;}

	Opacity &GetOpacity();
};
