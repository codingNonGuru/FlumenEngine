#include "FlumenEngine/Interface/ElementFactory.h"
#include "FlumenEngine/Interface/Element.hpp"
#include "FlumenEngine/Interface/Text.hpp"
#include "FlumenEngine/Core/Engine.hpp"
#include "FlumenEngine/Render/Screen.hpp"
#include "FlumenEngine/Interface/Interface.hpp"

#define DEFAULT_CANVAS_ELEMENT_COUNT 512

Element * ElementFactory::BuildCanvas()
{
    auto canvas = new Element();
    canvas->Initialize(DEFAULT_CANVAS_ELEMENT_COUNT);
    canvas->Configure(Engine::GetScreen()->GetSize(), 0, {Position2()});

    Interface::Get()->AddCanvas(canvas);

    return canvas;
}