#pragma once

#include "FrameBuffer.hpp"

class ShadowFrameBuffer : public FrameBuffer
{
protected:
    virtual void HandleInitialize(FrameBufferAttachments) override;
};
