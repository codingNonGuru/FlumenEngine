#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Core/Types.hpp"
#include "FlumenEngine/Utility/Color.hpp"

class Shader;
class Texture;

class FrameBuffer
{
protected:
	FrameBufferTypes type_;

	Size size_;

	Index bufferKey_, depthBufferKey_;

	Texture* colorTexture_;

	Texture* depthTexture_;

	GLenum textureType_;

	virtual void HandleInitialize(FrameBufferAttachments);

public:
	FrameBuffer();

    FrameBuffer(Size);

    FrameBuffer(Size, FrameBufferAttachments);

    void Initialize(Size, FrameBufferAttachments);

    void BindBuffer();

    void BindBuffer(GLenum, GLenum);

    void UnbindBuffer(GLenum);

    void BindTexture(Shader*, const char*);

    void UnbindTexture();

    Texture* GetColorTexture();

    Texture* GetDepthTexture();

    void Clear(Color);
};
