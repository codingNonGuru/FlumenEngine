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
	render::Texture *Texture {nullptr};

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

	Color* color_ {nullptr};

	SpriteDrawData *drawData_ {nullptr};

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

	void SetTexture(render::Texture* texture);

	void SetTexture(Word textureName);

	void SetParent(Element* parent) {parent_ = parent;}

	void SetColor(Color* color) {color_ = color;}

	void SetDrawData(SpriteDrawData *drawData) {drawData_ = drawData;}

	float & GetOpacity();
};
