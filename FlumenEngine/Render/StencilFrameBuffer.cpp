#include "FlumenEngine/Render/StencilFrameBuffer.hpp"
#include "FlumenEngine/Render/Texture.hpp"

void StencilFrameBuffer::HandleInitialize(FrameBufferAttachments attachments)
{
	glGenFramebuffers(1, &bufferKey_);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferKey_);

	colorTexture_ = new Texture(size_, TextureFormats::FOUR_FLOAT);
	colorTexture_->Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType_, colorTexture_->GetKey(), 0);
}
