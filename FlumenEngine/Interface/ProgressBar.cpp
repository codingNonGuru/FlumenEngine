#include "FlumenEngine/Interface/ProgressBar.h"
#include "FlumenEngine/Interface/ElementFactory.h"
#include "FlumenEngine/Interface/Sprite.hpp"

const auto DEFAULT_BAR_SHADER = "SlicedSprite";

void ProgressBar::HandleConfigure()
{
    filler = ElementFactory::BuildElement <Filler>
    (
        {size_, drawOrder_ + 1, {ElementAnchors::MIDDLE_LEFT, ElementPivots::MIDDLE_LEFT, this}, {DEFAULT_BAR_SHADER}}
    );
    filler->Enable();
}

void ProgressBar::HandleUpdate()
{
    auto barWidth = GetWidth() - 2.0f * GetSprite()->GetSlice().x;
    auto newWidth = barWidth * progress + 2.0f * GetSprite()->GetSlice().x;
    filler->SetWidth(newWidth);
}

void ProgressBar::Filler::HandleUpdate() 
{
    UpdatePosition();
}

void ProgressBar::Setup(BarData data)
{
    filler->GetSprite()->SetTexture(data.FillerTexture);

    GetSprite()->SetSlice(data.CornerSlices);
    filler->GetSprite()->SetSlice(data.CornerSlices);
}