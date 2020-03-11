#pragma once

#include "FrameBuffer.hpp"

class StencilFrameBuffer : public FrameBuffer
{
public:
    virtual void HandleInitialize(FrameBufferAttachments) override;
};
