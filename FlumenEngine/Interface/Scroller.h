#pragma once

#include "FlumenCore/Delegate/Delegate.hpp"

#include "FlumenEngine/Interface/Element.hpp"

class LayoutGroup;

class Scroller : public Element
{
    Delegate onScrollUp;

    Delegate onScrollDown;

    int firstElementIndex;

    int listSize;

    int activeChildCount;

    void HandleUpdate() override;

    void HandleEnable() override;

    void HandleDisable() override;

    void HandleScrollUp();

    void HandleScrollDown();

public:
   void Setup(LayoutGroup *, int, int);

   void SetActiveChildCount(int);
};