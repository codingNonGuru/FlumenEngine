#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Interface/TextData.h"

class Font;
class Camera;
struct FontDescriptor;

class Text : public Element
{
	friend class ElementFactory;

public:
	enum class Alignments {LEFT, CENTER, RIGHT};

private:
	Font* font_;

	Phrase string_;

	Color color_;

	Float scale_;

	Alignments alignment;

	void Assemble();

	Float GetTextWidth();

protected:
	Text();

	Text(FontDescriptor, Color = Color::BLACK);

public:
	void SetColor(Color);

	void Setup(const char*, Float = 1.0f);

	virtual void Render(Camera*);

	void SetAlignment(Alignments alignment) {this->alignment = alignment;}

	void AdjustSize();
};
