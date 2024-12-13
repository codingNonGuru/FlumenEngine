#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/ElementData.h"
#include "FlumenEngine/Interface/TextData.h"
#include "FlumenEngine/Interface/BarData.h"
#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Interface/SimpleList.h"

class Text;
class ProgressBar;

class ElementFactory 
{
public:
    static Element * BuildCanvas();

    template <class ElementType>
    static ElementType * BuildElement(ElementData, int);

    template <class TextType = Text>
    static TextType * BuildText(ElementData, TextData);

    template <class BarType = ProgressBar>
    static BarType * BuildProgressBar(ElementData, BarData);

    template <class ListType = SimpleList>
    static ListType *BuildSimpleList(ElementData, int, ListOrientations = ListOrientations::VERTICAL, float = 0.0f);
};

template <class ElementType = Element>
ElementType * ElementFactory::BuildElement(ElementData data, int childCount = DEFAULT_CHILD_COUNT)
{
    auto element = new ElementType();
    element->Initialize(childCount);
    element->Configure(data.Size, data.Order, data.PositionData, data.Sprite, data.Opacity);
    
    return element;
}

template <class TextType>
TextType * ElementFactory::BuildText(ElementData elementData, TextData textData)
{
    auto text = new TextType(textData.Font, textData.Color);
    text->Initialize();
    text->Configure(elementData.Size, elementData.Order, elementData.PositionData, elementData.Sprite, elementData.Opacity);
    text->Setup(textData.Content);

    return text;    
}

template <class BarType>
BarType * ElementFactory::BuildProgressBar(ElementData elementData, BarData barData)
{
    auto bar = new BarType();
    bar->Initialize();
    bar->Configure(elementData.Size, elementData.Order, elementData.PositionData, elementData.Sprite, elementData.Opacity);
    bar->Setup(barData);

    return bar;    
}

template <class ListType>
ListType *ElementFactory::BuildSimpleList(ElementData elementData, int childCount, ListOrientations orientation, float distance)
{
    auto list = new ListType();
    list->Initialize(childCount);
    list->Configure(elementData.Size, elementData.Order, elementData.PositionData, elementData.Sprite, elementData.Opacity);
    list->Setup(orientation, distance);

    return list;    
}