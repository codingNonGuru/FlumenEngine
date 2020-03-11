#include "FlumenEngine/Render/ShadowFrameBuffer.hpp"
#include "FlumenEngine/Render/Texture.hpp"

void ShadowFrameBuffer::HandleInitialize(FrameBufferAttachments attachments)
{
	glGenFramebuffers(1, &bufferKey_);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferKey_);

	depthTexture_ = new Texture(size_, TextureFormats::DEPTH_FLOAT);
	depthTexture_->Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_->GetKey(), 0);
}
