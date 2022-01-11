#pragma once

#include <glm/glm.hpp>

#include "FlumenCore/Conventions.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture;
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

class Sprite
{
private:
	bool isActive_;

	float opacity_;

	float depth_;

	Texture* texture_;

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

	Sprite(Texture*, Shader*);

	void Initialize(Texture*, Shader*);

	void Draw(Camera*, const SpriteDrawData = SpriteDrawData());

	void SetTexture(Texture* texture) {texture_ = texture;}

	void SetParent(Element* parent) {parent_ = parent;}

	void SetColor(Color* color) {color_ = color;}

	float & GetOpacity();
};
