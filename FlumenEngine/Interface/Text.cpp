#include "Text.hpp"

#include "FlumenEngine/Interface/TextManager.hpp"
#include "FlumenEngine/Interface/Glyph.hpp"
#include "FlumenEngine/Interface/FontManager.hpp"
#include "FlumenEngine/Interface/Font.hpp"
#include "FlumenEngine/Interface/Interface.hpp"
#include "FlumenEngine/Render/DataBuffer.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/Camera.hpp"
#include "FlumenEngine/Render/Texture.hpp"
#include "FlumenEngine/Interface/TextData.h"

Text::Text() {}

Text::Text(FontDescriptor fontName, Color color) 
{
	font_ = FontManager::GetFont(fontName);

	color_ = color;

	alignment = Alignments::CENTER;

	rows_.Initialize(16);
	for(int i = 0; i < rows_.GetCapacity(); ++i)
	{
		auto row = rows_.Add();
		*row = {Array <Text::LetterData> (256), 0.0f, 0.0f};
	}
}

void Text::Setup(const char* string, Float scale)
{
	string_ = string;

	scale_ = scale;

	rows_.Reset();

	int rowIndex = 0;

	auto currentRow = rows_.Add();
	currentRow->Letters.Reset();
	currentRow->Width = 0.0f;

	for(auto sign = string_.Get(); sign != string_.GetEnd(); ++sign)
	{
		if(*sign == '\n')
		{
			currentRow = rows_.Add();
			currentRow->Letters.Reset();
			currentRow->Width = 0.0f;
			currentRow->Height = 0.0f;

			rowIndex++;
			continue;
		}
		
		*currentRow->Letters.Add() = {*sign, rowIndex};
		
		auto glyph = font_->GetGlyph(*sign);
		currentRow->Width += glyph->GetAdvance();
	}

	if(isWidthLocked_ == false)
	{
		size_.x = 0.0f;
		for(auto &row : rows_)
		{
			if(row.Width > size_.x)
			{
				size_.x = row.Width;
			}
		}
	}

	if(isHeightLocked_ == false)
	{
		if(string_.GetSize() == 0)
		{
			size_.y = 0.0f;
		}
		else
		{
			size_.y = font_->GetHeight() * float(rowIndex + 1) * rowSpacingFactor_;
		}
	}

	UpdatePosition();
}

void Text::SetFont(FontDescriptor fontName)
{
	font_ = FontManager::GetFont(fontName);
}

void Text::SetColor(Color color)
{
	color_ = color;
}

void Text::Assemble()
{
	auto& dataQueue = TextManager::GetDataQueue();

	dataQueue.Reset();

	auto textPosition = GetGlobalPosition();
	textPosition.x -= size_.x * 0.5f;
	textPosition.y -= size_.y * 0.5f;

	auto startPosition = Float2(0.0f, font_->GetHeight() * 0.5f * rowSpacingFactor_);
	for(auto &row : rows_)
	{
		auto offset = [&] ()
		{
			switch(alignment)
			{
				case Alignments::LEFT:
					return 0.0f;
				case Alignments::RIGHT:
					return size_.x - row.Width;
				default:
					return (size_.x - row.Width) * 0.5f;		
			}
		} ();

		for(auto letter : row.Letters)
		{
			auto glyph = font_->GetGlyph(letter.Letter);

			if(!glyph)
				continue;

			auto glyphData = dataQueue.Allocate();

			if(!glyphData)
				continue;

			*glyphData = glyph->GetData();

			glyphData->Color_ = color_;

			startPosition.x += glyph->GetAdvance() * 0.5f;

			glyphData->Position_ = startPosition;
			glyphData->Position_.x += offset;
			glyphData->Position_.x += textPosition.x;
			glyphData->Position_.y += textPosition.y;

			startPosition.x += glyph->GetAdvance() * 0.5f;
		}

		startPosition.x = 0.0f;
		startPosition.y += font_->GetHeight() * rowSpacingFactor_;
	}
}

Integer Text::GetFontHeight() const
{
	return font_->GetHeight();
}

void Text::LockWidth(int width) 
{
	isWidthLocked_ = true;
	size_.x = width;
}

void Text::LockHeight(int height) 
{
	isHeightLocked_ = true;
	size_.y = height;
}

void Text::Render(Camera* camera)
{
	if(!IsGloballyActive())
		return;

	Assemble();

	auto& dataQueue = TextManager::GetDataQueue();

	auto buffer = TextManager::GetBuffer();

	buffer->UploadData(dataQueue.GetStart(), dataQueue.GetMemorySize());

	auto shader = TextManager::GetShader();

	auto texture = font_->GetTexture();

	shader->Bind();

	buffer->Bind(0);

	shader->SetConstant(camera->GetMatrix(), "viewMatrix");

	Opacity opacity = opacity_;
	shader->SetConstant(opacity, "opacity");

	auto drawOrder = (float)drawOrder_ * 0.1f;
	shader->SetConstant(drawOrder, "depth");

	if(texture && shader)
	{
		shader->BindTexture(texture, "diffuse");
	}

	glDrawArrays(GL_TRIANGLES, 0, dataQueue.GetSize() * 6);

	shader->Unbind();

	if(texture)
	{
		texture->Unbind();
	}
}