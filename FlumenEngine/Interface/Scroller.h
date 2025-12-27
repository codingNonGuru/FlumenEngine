#pragma once

#include "FlumenCore/Delegate/Delegate.hpp"

#include "FlumenEngine/Interface/Element.hpp"

class Scroller : public Element
{
    int firstElementIndex;

    int maximumVisibleCount;

    int activeChildCount;

    void HandleInitialize() override;

    void HandleLateUpdate() override;

    void HandleExtremelyLateUpdate() override;

    void HandleEnable() override;

    void HandleDisable() override;

    void HandleScrollUp() override;

    void HandleScrollDown() override;

public:
    void Setup(Element *, int, int);

    void SetActiveChildCount(int);
};