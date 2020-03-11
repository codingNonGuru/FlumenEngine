#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Utility/Color.hpp"

class Font;
class Camera;

class Text : public Element
{
	Font* font_;

	container::String <256> string_;

	Color color_;

	Float scale_;

	void Assemble();

public:
	Text();

	Text(Font*, Color = Color::BLACK);

	void Setup(const char*, Float = 1.0f);

	virtual void Render(Camera*);
};
