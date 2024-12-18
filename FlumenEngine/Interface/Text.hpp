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
	struct LetterData
	{
		char Letter;

		int RowIndex;
	};

	struct Row
	{
		Array <LetterData> Letters;

		float Width;

		float Height;
	};

	Font* font_;

	Phrase string_;

	Array <Row> rows_;

	Color color_;

	Float scale_;

	Alignments alignment;

	Float rowSpacingFactor_ {0.85f};

	bool isWidthLocked_ {false};

	bool isHeightLocked_ {false};

	void Assemble();

protected:
	Text();

	Text(FontDescriptor, Color = Color::BLACK);

public:
	void SetFont(FontDescriptor);

	void SetColor(Color);

	void Setup(const char*, Float = 1.0f);

	void SetScale(Float scale) {scale_ = scale;}

	virtual void Render(Camera*);

	void SetAlignment(Alignments alignment) {this->alignment = alignment;}

	Integer GetFontHeight() const;

	void LockWidth() {isWidthLocked_ = true;}

	void LockHeight() {isHeightLocked_ = true;}

	void LockWidth(int);

	void LockHeight(int);
};
