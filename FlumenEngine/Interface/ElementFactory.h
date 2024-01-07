#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/ElementData.h"
#include "FlumenEngine/Interface/TextData.h"

class Element;
class Text;

class ElementFactory 
{
public:
    static Element * BuildCanvas();

    template <class ElementType>
    static ElementType * BuildElement(ElementData);

    template <class TextType = Text>
    static TextType * BuildText(ElementData, TextData);
};

template <class ElementType = Element>
ElementType * ElementFactory::BuildElement(ElementData data)
{
    auto element = new ElementType();
    element->Configure(data.Size, data.Order, data.PositionData, data.Sprite, data.Opacity);
    
    return element;
}

template <class TextType>
TextType * ElementFactory::BuildText(ElementData elementData, TextData textData)
{
    auto text = new TextType(textData.Font, textData.Color);
    text->Configure(elementData.Size, elementData.Order, elementData.PositionData, elementData.Sprite, elementData.Opacity);
    text->Setup(textData.Content);

    return text;    
}