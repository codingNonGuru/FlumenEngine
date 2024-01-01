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
	Position2 Position;

	Scale2 Size;

	Opacity Opacity;

	DrawOrder Depth;
};

struct TextureData
{
	render::Texture *Texture;

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

	float opacity_;

	float depth_;

	TextureData textureData_;

	Shader* shader_;

	Element* parent_;

	Color* color_;

	void SetDefaultConstants(Camera *, const SpriteDrawData *);

	void BindDefaultTextures();

protected:
	virtual void SetExtraConstants();

	virtual void BindExtraTextures();

public:
	Sprite();

	Sprite(Shader *, TextureData = TextureData());

	void Initialize(Shader *, TextureData = TextureData());

	void Draw(Camera*, const SpriteDrawData = SpriteDrawData());

	void SetTexture(render::Texture* texture) {textureData_.Texture = texture;}

	void SetParent(Element* parent) {parent_ = parent;}

	void SetColor(Color* color) {color_ = color;}

	float & GetOpacity();
};
