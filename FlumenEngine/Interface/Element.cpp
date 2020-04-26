#include <stdlib.h>

#include "FlumenCore/Delegate/Delegate.hpp"

#include "Element.hpp"
#include "Sprite.hpp"
#include "FlumenEngine/Core/Transform.hpp"
#include "FlumenEngine/Animation/Animation.hpp"
#include "FlumenEngine/Animation/Animator.hpp"
#include "FlumenEngine/Animation/AnimationEvent.hpp"
#include "FlumenEngine/Animation/AnimationProperty.hpp"
#include "FlumenEngine/Core/InputHandler.hpp"
#include "FlumenEngine/Interface/Interface.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/ShaderManager.hpp"
#include "FlumenEngine/Render/TextureManager.hpp"

#define DEFAULT_CHILDREN_COUNT 32

Element::Element() 
{
	isActive_ = false;

	isInteractive_ = false;

	parent_ = nullptr;

	transform_ = nullptr;

	animator_ = nullptr;

	leftClickEvents_ = nullptr;

	rightClickEvents_ = nullptr;

	hoverEvents_ = nullptr;

	Interface::AddElement("DefaultName", this);
}

/*void Element::Configure(Size size, DrawOrder drawOrder, Position2 position, SpriteDescriptor spriteDescriptor, Opacity opacity)
{
	Sprite* sprite = nullptr;
	if(spriteDescriptor)
	{
		auto shader = ShaderManager::GetShaderMap().Get(spriteDescriptor.ShaderName);
		auto texture = TextureManager::GetTexture(spriteDescriptor.TextureName);

    	sprite = new Sprite(texture, shader);
	}

	isActive_ = false;

	isInteractive_ = false;

	transform_ = new Transform(position);

	parent_ = nullptr;

	animator_ = new Animator();

	sprite_ = sprite;
	if(sprite_)
	{
		sprite_->SetParent(this);
	}

	size_ = size;

	drawOrder_ = drawOrder;

	opacity_ = opacity;

	leftClickEvents_ = new Delegate();

	rightClickEvents_ = new Delegate();

	hoverEvents_ = new Delegate();

	children_.Initialize(DEFAULT_CHILDREN_COUNT);

	HandleConfigure();
}*/

void Element::Configure(Size size, DrawOrder drawOrder, PositionData positionData, SpriteDescriptor spriteDescriptor, Opacity opacity)
{
	Sprite* sprite = nullptr;
	if(spriteDescriptor)
	{
		auto shader = ShaderManager::GetShaderMap().Get(spriteDescriptor.ShaderName);
		auto texture = TextureManager::GetTexture(spriteDescriptor.TextureName);

    	sprite = new Sprite(texture, shader);
	}

	isActive_ = false;

	isInteractive_ = false;

	size_ = size;

	parent_ = positionData.Parent;

	Position2 offset = -GetPivotOffset(positionData.Pivot);
	
	if(positionData.Parent != nullptr)
	{
		offset += positionData.Parent->GetAnchorOffset(positionData.Anchor);
	}

	transform_ = new Transform(positionData.Position + offset);

	animator_ = new Animator();

	sprite_ = sprite;
	if(sprite_)
	{
		sprite_->SetParent(this);
	}

	drawOrder_ = drawOrder;

	opacity_ = opacity;

	leftClickEvents_ = new Delegate();

	rightClickEvents_ = new Delegate();

	hoverEvents_ = new Delegate();

	children_.Initialize(DEFAULT_CHILDREN_COUNT);

	HandleConfigure();
}

Word Element::GetIdentifier()
{
	return identifier_;
}

void Element::SetIdentifier(Word identifier)
{
	identifier_ = identifier;
}

Sprite* Element::GetSprite()
{
	return sprite_;
}

void Element::Initialize()
{
	auto animation = animator_->GetAnimation("Close");
	if(animation != nullptr)
	{
		closeEvent_ = animation->GetFinishEvent();
		closeEvent_->GetActions().Add(this, &Element::Disable);
	}

	HandleInitialize();
}

bool Element::CheckHover()
{
	if(!isInteractive_)
		return false;

	if(IsGloballyActive() == false)
	{
		isHovered_ = false;
		return false;
	}

	auto mousePosition = InputHandler::GetMousePosition();

	auto position = GetGlobalPosition();

	bool isInsideHorizontally = mousePosition.x > position.x - size_.x / 2 && mousePosition.x < position.x + size_.x / 2;
	bool isInsideVertically = mousePosition.y > position.y - size_.y / 2 && mousePosition.y < position.y + size_.y / 2;

	isHovered_ = isInsideHorizontally && isInsideVertically;

	return isHovered_;
}

void Element::Update()
{
	if(IsGloballyActive() == false)
		return;

	if(animator_)
	{
		animator_->Update();
	}

	HandleUpdate();
}

void Element::Render(Camera* camera)
{
	if(IsGloballyActive() == false)
		return;

	if(!sprite_)
		return;

	sprite_->Draw(camera);
}

AnimationProperty* Element::AddAnimationProperty(const char* animationName, InterfaceElementParameters parameter)
{
	auto animation = animator_->GetAnimation(animationName);
	if(!animation)
		return nullptr;

	AnimationProperty * property = nullptr;
	switch(parameter)
	{
	case InterfaceElementParameters::POSITION_X:
		property = animation->AddProperty();
		property->Initialize(&transform_->GetPosition().x);
		return property;
	case InterfaceElementParameters::POSITION_Y:
		property = animation->AddProperty();
		property->Initialize(&transform_->GetPosition().y);
		return property;
	case InterfaceElementParameters::OPACITY:
		property = animation->AddProperty();
		property->Initialize(&opacity_);
		return property;
	}

	return nullptr;
}

Size & Element::GetSize()
{
	return size_;
}

DrawOrder & Element::GetDrawOrder()
{
	return drawOrder_;
}

Opacity Element::GetOpacity()
{
	return opacity_;
}

Delegate & Element::GetLeftClickEvents()
{
	return *leftClickEvents_;
}

Delegate & Element::GetRightClickEvents()
{
	return *rightClickEvents_;
}

Delegate & Element::GetHoverEvents()
{
	return *hoverEvents_;
}

Animator* Element::GetAnimator()
{
	return animator_;
}

void Element::SetInteractivity(bool isInteractive)
{
	isInteractive_ = isInteractive;
}

void Element::TriggerLeftClickEvents()
{
	if(!leftClickEvents_)
		return;

	leftClickEvents_->Invoke();
}

void Element::TriggerRightClickEvents()
{
	if(!rightClickEvents_)
		return;

	rightClickEvents_->Invoke();
}

void Element::TriggerHoverEvents()
{
	if(!hoverEvents_)
		return;

	hoverEvents_->Invoke();
}

void Element::Open()
{
	if(!animator_ || isActive_)
		return;

	isActive_ = true;

	animator_->Play("Open");

	HandleOpen();
}

void Element::Close()
{
	if(!animator_ || !isActive_)
		return;

	animator_->Play("Close");

	HandleClose();
}

void Element::AddChild(Element* child)
{
	auto childPointer = children_.Allocate();
	if(childPointer == nullptr)
		return;

	*childPointer = child;
}

Element* Element::GetChild(Word identifier)
{
	for(auto childIterator = children_.GetStart(); childIterator != children_.GetEnd(); ++childIterator)
	{
		auto child = *childIterator;
		if(child == nullptr)
			continue;

		if(child->GetIdentifier() == identifier)
			return child;
	}

	return nullptr;
}

void Element::HandleOpen()
{
}

void Element::HandleClose()
{
}

void Element::HandleEnable()
{
}

void Element::HandleDisable()
{

}

void Element::HandleSetParent(Object* parent)
{
	auto parentAsElement = (Element*)parent;
	parentAsElement->AddChild(this);
}

void Element::HandleConfigure() {}

void Element::HandleInitialize() {}

void Element::HandleUpdate() {}

Position2 Element::GetAnchorOffset(ElementAnchors anchor)
{
	Position2 position = Position2(0.0f, 0.0f);//transform_->GetPosition();
	Direction2 sizeOffset = Float2(size_) * 0.5f;
	switch(anchor)
	{
		case ElementAnchors::UPPER_LEFT:
			return {position.x - sizeOffset.x, position.y - sizeOffset.y};
		case ElementAnchors::UPPER_CENTER:
			return {position.x, position.y - sizeOffset.y};
		case ElementAnchors::UPPER_RIGHT:
			return {position.x + sizeOffset.x, position.y - sizeOffset.y};
		case ElementAnchors::MIDDLE_LEFT:
			return {position.x - sizeOffset.x, position.y};
		case ElementAnchors::MIDDLE_CENTER:
			return {position.x, position.y};
		case ElementAnchors::MIDDLE_RIGHT:
			return {position.x + sizeOffset.x, position.y};
		case ElementAnchors::LOWER_LEFT:
			return {position.x - sizeOffset.x, position.y + sizeOffset.y};
		case ElementAnchors::LOWER_CENTER:
			return {position.x, position.y + sizeOffset.y};
		case ElementAnchors::LOWER_RIGHT:
			return {position.x + sizeOffset.x, position.y + sizeOffset.y};
	}
}

Position2 Element::GetPivotOffset(ElementPivots pivot)
{
	Position2 position = Position2(0.0f, 0.0f);//transform_->GetPosition();
	Direction2 sizeOffset = Float2(size_) * 0.5f;
	switch(pivot)
	{
		case ElementPivots::UPPER_LEFT:
			return {position.x - sizeOffset.x, position.y - sizeOffset.y};
		case ElementPivots::UPPER_CENTER:
			return {position.x, position.y - sizeOffset.y};
		case ElementPivots::UPPER_RIGHT:
			return {position.x + sizeOffset.x, position.y - sizeOffset.y};
		case ElementPivots::MIDDLE_LEFT:
			return {position.x - sizeOffset.x, position.y};
		case ElementPivots::MIDDLE_CENTER:
			return {position.x, position.y};
		case ElementPivots::MIDDLE_RIGHT:
			return {position.x + sizeOffset.x, position.y};
		case ElementPivots::LOWER_LEFT:
			return {position.x - sizeOffset.x, position.y + sizeOffset.y};
		case ElementPivots::LOWER_CENTER:
			return {position.x, position.y + sizeOffset.y};
		case ElementPivots::LOWER_RIGHT:
			return {position.x + sizeOffset.x, position.y + sizeOffset.y};
	}
}