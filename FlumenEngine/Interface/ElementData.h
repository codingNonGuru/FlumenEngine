#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Core/Types.hpp"

class Element;

struct SpriteDescriptor
{
private:
	bool isInitialized;

public:
	Word TextureName;

	Word ShaderName;

	SpriteDescriptor() : isInitialized(false) {}

	SpriteDescriptor(Word shaderName) : TextureName(), ShaderName(shaderName), isInitialized(true) {}

	SpriteDescriptor(Word textureName, Word shaderName) : TextureName(textureName), ShaderName(shaderName), isInitialized(true) {}

	operator bool() {return isInitialized;}
};

struct PositionData
{
	Position2 Position;

	ElementAnchors Anchor;

	ElementPivots Pivot;

	Element *Parent;

    PositionData(const PositionData &otherData) : Position(otherData.Position), Anchor(otherData.Anchor), Pivot(otherData.Pivot), Parent(otherData.Parent) {}

	PositionData(Position2 position, ElementAnchors anchor, ElementPivots pivot, Element *parent) : Position(position), Anchor(anchor), Pivot(pivot), Parent(parent) {}

	PositionData(Position2 position, ElementAnchors anchor, Element *parent) : Position(position), Anchor(anchor), Pivot (ElementPivots::MIDDLE_CENTER), Parent(parent) {}

	PositionData(Position2 position, Element *parent) : Position(position), Anchor(ElementAnchors::MIDDLE_CENTER), Pivot(ElementPivots::MIDDLE_CENTER), Parent(parent) {}

	PositionData(Position2 position, ElementAnchors anchor) : Position(position), Anchor(anchor), Pivot(ElementPivots::MIDDLE_CENTER), Parent(nullptr) {}

	PositionData(Position2 position) : Position(position), Anchor(ElementAnchors::MIDDLE_CENTER), Pivot(ElementPivots::MIDDLE_CENTER), Parent(nullptr) {}

    PositionData() : Position(Position2(0.0f, 0.0f)), Anchor(ElementAnchors::MIDDLE_CENTER), Pivot(ElementPivots::MIDDLE_CENTER), Parent(nullptr) {}
};

struct ElementData
{
    typedef Size ElementSize;
    ElementSize Size;

    DrawOrder Order;

    typedef PositionData ElementPositionData;
    ElementPositionData PositionData;

    SpriteDescriptor Sprite;
    
    typedef Opacity ElementOpacity;
    Opacity Opacity;

    ElementData(ElementSize size, DrawOrder order, ElementPositionData positionData, SpriteDescriptor sprite, ElementOpacity opacity) :
        Size(size), Order(order), PositionData(positionData), Sprite(sprite), Opacity(opacity)
    {}

    ElementData(ElementSize size, DrawOrder order, ElementPositionData positionData, SpriteDescriptor sprite) :
        Size(size), Order(order), PositionData(positionData), Sprite(sprite), Opacity(ElementOpacity(1.0f))
    {}

    ElementData(ElementSize size, DrawOrder order, ElementPositionData positionData, ElementOpacity opacity) :
        Size(size), Order(order), PositionData(positionData), Sprite(SpriteDescriptor()), Opacity(opacity)
    {}

    ElementData(ElementSize size, DrawOrder order, ElementPositionData positionData) :
        Size(size), Order(order), PositionData(positionData), Sprite(SpriteDescriptor()), Opacity(ElementOpacity(1.0f))
    {}
};