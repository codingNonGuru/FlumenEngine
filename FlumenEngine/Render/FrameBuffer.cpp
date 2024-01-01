#include <GL/glew.h>

#include "FrameBuffer.hpp"

#include "FlumenEngine/Render/Texture.hpp"
#include "FlumenEngine/Render/Shader.hpp"

FrameBuffer::FrameBuffer()
{
	bufferKey_ = 0;
}

FrameBuffer::FrameBuffer(Size size)
{
	size_ = size;
	bufferKey_ = 0;
}

FrameBuffer::FrameBuffer(Size size, FrameBufferAttachments attachment)
{
	Initialize(size, attachment);
}

void FrameBuffer::Initialize(Size size, FrameBufferAttachments attachment)
{
    size_ = size;

    colorTexture_ = nullptr;
    depthTexture_ = nullptr;

	textureType_ = GL_TEXTURE_2D;

	HandleInitialize(attachment);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(textureType_, 0);

    DEBUG_OPENGL
}

void FrameBuffer::HandleInitialize(FrameBufferAttachments attachment)
{
	glGenFramebuffers(1, &bufferKey_);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferKey_);

	if(attachment == FrameBufferAttachments::COLOR || attachment == FrameBufferAttachments::COLOR_AND_DEPTH)
	{
		colorTexture_ = new render::Texture(size_, TextureFormats::FOUR_FLOAT);
		colorTexture_->Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType_, colorTexture_->GetKey(), 0);
	}

	if(attachment == FrameBufferAttachments::DEPTH || attachment == FrameBufferAttachments::COLOR_AND_DEPTH)
	{
		depthTexture_ = new render::Texture(size_, TextureFormats::ONE_FLOAT);
		depthTexture_ ->Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureType_, depthTexture_ ->GetKey(), 0);
	}
}

void FrameBuffer::BindBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferKey_);

	glViewport(0, 0, size_.x, size_.y);

	DEBUG_OPENGL
}

void FrameBuffer::BindBuffer(GLenum target, GLenum attachment)
{
	glBindFramebuffer(target, bufferKey_);

	if(target == GL_DRAW_FRAMEBUFFER)
		glDrawBuffer(attachment);
	else
		glReadBuffer(attachment);

    glViewport(0, 0, size_.x, size_.y);

    DEBUG_OPENGL
}

void FrameBuffer::UnbindBuffer(GLenum target)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, bufferKey_);

    DEBUG_OPENGL
}

void FrameBuffer::BindTexture(Shader* shader, const char* textureName)
{
    shader->BindTexture(colorTexture_, textureName);
}

void FrameBuffer::UnbindTexture()
{
    glBindTexture(textureType_, 0);

    DEBUG_OPENGL
}

render::Texture* FrameBuffer::GetColorTexture()
{
	return colorTexture_;
}

render::Texture* FrameBuffer::GetDepthTexture()
{
	return depthTexture_;
}

void FrameBuffer::Clear(Color color)
{
	glClearColor(color.r_, color.g_, color.b_, color.a_);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DEBUG_OPENGL
}
