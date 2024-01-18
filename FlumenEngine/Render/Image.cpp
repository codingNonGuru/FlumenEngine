#include "FlumenEngine/Render/DataBuffer.hpp"
#include "Image.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/ShaderManager.hpp"

Image::Image()
{}

Image::Image(Size size, ImageFormats format) : format_(format)
{
	size_ = size;

	switch(format)
	{
	case ImageFormats::RGBA:
		buffer_ = new DataBuffer(size.x * size.y * 4);
		break;
	case ImageFormats::RGB:
		buffer_ = new DataBuffer(size.x * size.y * 3);
		break;
	case ImageFormats::ALPHA:
		buffer_ = new DataBuffer(size.x * size.y * 1);
		break;
	default:
		break;
	}
}

Image::Image(Size size, Color& color, ImageFormats format) : format_(format)
{
	size_ = size;

	switch(format)
	{
	case ImageFormats::RGBA:
		buffer_ = new DataBuffer(size.x * size.y * 4);
		break;
	case ImageFormats::RGB:
		buffer_ = new DataBuffer(size.x * size.y * 3);
		break;
	case ImageFormats::ALPHA:
		buffer_ = new DataBuffer(size.x * size.y * 1);
		break;
	default:
		break;
	}

	Clear(color);
}

Image::Image(Size size, DataBuffer* sourceBuffer, Size sourceSize, Size offset)
{
	size_ = size;

	buffer_ = new DataBuffer(size.x * size.y * 4);

	auto shader = ShaderManager::GetShaderMap().Get("GenerateStencil");
	if(shader)
	{
		shader->Bind();
	}

	buffer_->Bind(0);
	sourceBuffer->Bind(1);

	shader->SetConstant(size, "stencilSize");
	shader->SetConstant(sourceSize, "sourceSize");

	shader->SetConstant(offset, "offset");

	shader->DispatchCompute(size / 4);

	shader->Unbind();
}

void Image::Clear(Color& color)
{
	auto shader = ShaderManager::GetShaderMap().Get("Clear");
	shader->Bind();

	shader->SetConstant(size_, "size");
	shader->SetConstant((Float4&)color, "color");

	switch(format_)
	{
	case ImageFormats::RGBA:
		buffer_->Bind(0);
		shader->SetConstant(0, "format");
		break;
	case ImageFormats::RGB:
		buffer_->Bind(1);
		shader->SetConstant(1, "format");
		break;
	case ImageFormats::ALPHA:
		buffer_->Bind(2);
		shader->SetConstant(2, "format");
		break;
	default:
		break;
	}

	shader->DispatchCompute(size_ / 4);

	shader->Unbind();
}

void Image::Setup(Size size, Color& color)
{
	size_ = size;

	Clear(color);
}

void Image::Bind(Index index)
{
	buffer_->Bind(index);
}

Size Image::GetSize() const
{
	return size_;
}

void Image::SetSize(Size size)
{
	size_ = size;
}

void Image::Apply(Image* canvas, Float alpha, Color& color, Size offset)
{
	auto shader = ShaderManager::GetShaderMap().Get("ApplyStencil");

	shader->Bind();

	buffer_->Bind(0);
	canvas->Bind(3);

	shader->SetConstant(canvas->GetSize(), "baseSize");
	shader->SetConstant(size_, "stencilSize");
	shader->SetConstant(alpha, "alpha");
	shader->SetConstant(offset, "offset");
	shader->SetConstant((Float4&)color, "color");

	Index mode = 2;
	shader->SetConstant(mode, "mode");

	shader->DispatchCompute(size_ / 4);

	shader->Unbind();
}

Image::operator DataBuffer()
{
	return *buffer_;
}
