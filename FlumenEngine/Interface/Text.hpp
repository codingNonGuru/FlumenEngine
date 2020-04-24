#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Utility/Color.hpp"

class Font;
class Camera;

struct FontDescriptor
{
	Word Name;

	FontDescriptor() {}

	FontDescriptor(Word name, Word size) {Name << name << size;}

	operator Word() {return Name;}

	operator const char*() {return Name;}
};

class Text : public Element
{
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

public:
	Text();

	Text(FontDescriptor, Color = Color::BLACK);

	void SetColor(Color);

	void Setup(const char*, Float = 1.0f);

	virtual void Render(Camera*);

	void SetAlignment(Alignments alignment) {this->alignment = alignment;}

	void AdjustSize();
};
