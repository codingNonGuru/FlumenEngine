#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Interface/ElementData.h"
#include "FlumenEngine/Interface/Interface.hpp"
#include "FlumenEngine/Interface/TextData.h"
#include "FlumenEngine/Interface/BarData.h"
#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Interface/SimpleList.h"
#include "FlumenEngine/Interface/Text.hpp"
#include "FlumenEngine/Core/Engine.hpp"
#include "FlumenEngine/Render/Screen.hpp"

class ProgressBar;

#define DEFAULT_ROW_COUNT 1

#define DEFAULT_ROW_LENGTH 128

#define DEFAULT_CANVAS_ELEMENT_COUNT 1024

class ElementFactory 
{
public:
    static Element * BuildCanvas();

    template <class ElementType>
    static ElementType *BuildCanvas();

    template <class ElementType>
    static ElementType * BuildElement(ElementData);

    template <class ElementType>
    static ElementType * BuildElement(ElementData, int);

    template <class ElementType>
    static ElementType * BuildElement(ElementData, AdditionalElementData *);

    template <class ElementType>
    static ElementType * BuildElement(ElementData, int, AdditionalElementData *);

    template <class TextType = Text>
    static TextType * BuildText(ElementData, TextData);

    template <class TextType = Text>
    static TextType * BuildRichText(ElementData, TextData);

    template <class BarType = ProgressBar>
    static BarType * BuildProgressBar(ElementData, BarData);

    template <class ListType = SimpleList>
    static ListType *BuildSimpleList(ElementData, int, ListOrientations = ListOrientations::VERTICAL, float = 0.0f);

    template <class ListType = SimpleList>
    static ListType *BuildSimpleList(ElementData, int, float = 0.0f);
};

template <class ElementType>
ElementType *ElementFactory::BuildCanvas()
{
    auto canvas = new ElementType();
    canvas->Initialize(DEFAULT_CANVAS_ELEMENT_COUNT);
    canvas->Configure(Engine::GetScreen()->GetSize(), 0, {Position2()});

    Interface::Get()->AddCanvas(canvas);

    return canvas;
}

template <class ElementType = Element>
ElementType * ElementFactory::BuildElement(ElementData data)
{
    auto element = new ElementType();
    element->Initialize(DEFAULT_CHILD_COUNT);
    element->Configure(data.Size, data.Order, data.PositionData, data.Sprite, data.Opacity);
    
    return element;
}

template <class ElementType = Element>
ElementType * ElementFactory::BuildElement(ElementData data, int childCount)
{
    auto element = new ElementType();
    element->Initialize(childCount);
    element->Configure(data.Size, data.Order, data.PositionData, data.Sprite, data.Opacity);
    
    return element;
}

template <class ElementType = Element>
ElementType * ElementFactory::BuildElement(ElementData data, int childCount, AdditionalElementData *additionalData)
{
    auto element = new ElementType();
    element->Initialize(childCount);
    element->Configure(data.Size, data.Order, data.PositionData, data.Sprite, data.Opacity, additionalData);
    
    return element;
}

template <class ElementType = Element>
ElementType * ElementFactory::BuildElement(ElementData data, AdditionalElementData *additionalData)
{
    auto element = new ElementType();
    element->Initialize(DEFAULT_CHILD_COUNT);
    element->Configure(data.Size, data.Order, data.PositionData, data.Sprite, data.Opacity, additionalData);
    
    return element;
}

template <class TextType>
TextType * ElementFactory::BuildText(ElementData elementData, TextData textData)
{
    if(textData.RowData != nullptr)
    {
        std::cout<<"Element Factory cannot BuildText with Row Data. Use BuildRichText.\n";
        abort();
    }

    auto text = new TextType(textData.Font, textData.PrimaryColor);

    text->Initialize();
    text->Configure(elementData.Size, elementData.Order, elementData.PositionData, elementData.Sprite, elementData.Opacity);
    text->Setup(textData.Content);

    return text;    
}

template <class TextType>
TextType * ElementFactory::BuildRichText(ElementData elementData, TextData textData)
{
    auto text = [&]
    {
        if(textData.RowData == nullptr)
            return new TextType(textData.Font, DEFAULT_ROW_COUNT, DEFAULT_ROW_LENGTH, textData.PrimaryColor, textData.SecondaryColor);
        else 
            return new TextType(textData.Font, textData.RowData->Count, textData.RowData->Size, textData.PrimaryColor, textData.SecondaryColor);
    } ();

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

template <class ListType>
ListType *ElementFactory::BuildSimpleList(ElementData elementData, int childCount, float distance)
{
    return BuildSimpleList(elementData, childCount, ListOrientations::VERTICAL, distance);
}