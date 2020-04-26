#include "FlumenEngine/Interface/ElementFactory.h"
#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Interface/Text.hpp"
#include "FlumenEngine/Core/Engine.hpp"
#include "FlumenEngine/Render/Screen.hpp"

Element * ElementFactory::BuildCanvas()
{
    auto canvas = new Element();
    canvas->Configure(Engine::GetScreen()->GetSize(), 0, {Position2(0.0f, 0.0f)});

    return canvas;
}

// Text * ElementFactory::BuildText(ElementData elementData, TextData textData)
// {
//     auto text = new Text(textData.Font, textData.Color);
//     text->Configure(elementData.Size, elementData.Order, elementData.PositionData, elementData.Sprite, elementData.Opacity);
//     text->Setup(textData.Content);

//     return text;    
// }