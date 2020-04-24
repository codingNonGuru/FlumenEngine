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

Text::Text() {}

Text::Text(FontDescriptor fontName, Color color = Color::BLACK) 
{
	font_ = FontManager::GetFont(fontName);

	color_ = color;

	alignment = Alignments::CENTER;

	//Interface::AddElement(elementName, this);
}

void Text::Setup(const char* string, Float scale)
{
	string_ = string;

	scale_ = scale;
}

void Text::SetColor(Color color)
{
	color_ = color;
}

void Text::AdjustSize()
{
	size_.x = GetTextWidth();
}

void Text::Assemble()
{
	auto& dataQueue = TextManager::GetDataQueue();

	dataQueue.Reset();

	auto textPosition = GetGlobalPosition();

	auto startPosition = Float2(0.0f, 0.0f);

	float textWidth = 0.0f;

	for(auto sign = string_.Get(); sign != string_.GetEnd(); ++sign)
	{
		auto glyph = font_->GetGlyph(*sign);

		if(!glyph)
			continue;

		auto glyphData = dataQueue.Allocate();

		if(!glyphData)
			continue;

		*glyphData = glyph->GetData();

		glyphData->Color_ = color_;

		startPosition.x += glyph->GetAdvance() * 0.5f;

		glyphData->Position_ = startPosition;

		startPosition.x += glyph->GetAdvance() * 0.5f;

		textWidth += glyph->GetAdvance();
	}

	auto widthOffset = textWidth * 0.5f;
	auto alignmentOffset = 0.0f;
	switch(alignment)
	{
		case Alignments::LEFT:
			alignmentOffset = size_.x * 0.5f - widthOffset;
			break;
		case Alignments::RIGHT:
			alignmentOffset = widthOffset - size_.x * 0.5f;
			break;
		default:
			break;
	}

	for(auto glyphData = dataQueue.GetStart(); glyphData != dataQueue.GetEnd(); ++glyphData)
	{
		glyphData->Position_.x -= widthOffset;
		glyphData->Position_.x -= alignmentOffset;

		glyphData->Position_.x *= scale_;

		glyphData->Position_.x += textPosition.x;
		glyphData->Position_.y += textPosition.y;

		glyphData->Scale_ *= scale_;
	}
}

Float Text::GetTextWidth()
{
	float textWidth = 0.0f;

	for(auto sign = string_.Get(); sign != string_.GetEnd(); ++sign)
	{
		auto glyph = font_->GetGlyph(*sign);

		if(!glyph)
			continue;

		textWidth += glyph->GetAdvance();
	}
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
