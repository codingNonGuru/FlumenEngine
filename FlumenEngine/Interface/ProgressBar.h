#pragma once

#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Interface/BarData.h"

class ProgressBar : public Element
{
    class Filler : public Element
    {
        //void HandleConfigure() override;

        void HandleUpdate() override;    
    };

    Filler *filler;

    float progress {0.5f};

    void HandleConfigure() override;

    void HandleUpdate() override;

public:
    void Setup(BarData);

    void SetProgress(float newProgress) 
    {
        progress = newProgress;
        if(progress > 1.0f)
        {
            progress = 1.0f;
        }

        UpdatePosition();
    }
};