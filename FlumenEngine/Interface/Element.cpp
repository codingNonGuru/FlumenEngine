#include <stdlib.h>

#include "FlumenCore/Delegate/Delegate.hpp"
#include "FlumenCore/Delegate/Event.hpp"

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
#include "FlumenEngine/Render/Camera.hpp"
#include "FlumenEngine/Render/RenderManager.hpp"
#include "FlumenEngine/Render/Texture.hpp"

#define DEFAULT_CHILDREN_COUNT 32

#define OPEN_ANIMATION_NAME "Open"

#define CLOSE_ANIMATION_NAME "Close"

void MouseFollower::Update()
{
	if(isActive == false)
		return;

	auto mousePosition = InputHandler::GetMousePosition();

	parent->basePosition_ = offset + mousePosition;
}

void Element::Initialize(int childCount) 
{
	isActive_ = false;

	isRendering_ = true;

	parent_ = nullptr;

	transform_ = nullptr;

	staticChildren_.Initialize(childCount);

	dynamicChildren_.Initialize(4);

	Interface::Get()->AddElement("DefaultName", this);

	HandleInitialize();
}

void Element::Configure(Size size, DrawOrder drawOrder, PositionData positionData, SpriteDescriptor spriteDescriptor, Opacity opacity)
{
	Sprite* sprite = nullptr;
	if(spriteDescriptor)
	{
		auto shader = ShaderManager::GetShaderMap().Get(spriteDescriptor.ShaderName);

		auto texture = [&] -> render::Texture *
		{
			if(spriteDescriptor.HasTexture == false)
				return nullptr;

			if(spriteDescriptor.Texture == nullptr)
			{
				return render::TextureManager::GetTexture(spriteDescriptor.TextureName);
			}
			else
			{
				return spriteDescriptor.Texture;
			}
		} ();

    	sprite = new Sprite(shader, texture);
	}

	isActive_ = false;

	isInteractive_ = false;

	size_ = size;

	SetParent(positionData.Parent);

	basePosition_ = positionData.Position;

	pivot_ = positionData.Pivot;

	anchor_ = positionData.Anchor;

	transform_ = new Transform(basePosition_);

	UpdatePosition();

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

	HandleConfigure();
}

void Element::UpdatePosition()
{
	Position2 offset = -GetPivotOffset(pivot_);
	
	if(parent_ != nullptr)
	{
		auto castParent = (Element*)(parent_);
		offset += castParent->GetAnchorOffset(anchor_);
	}

	*transform_ = Transform(basePosition_ + offset);
}

void Element::FollowWorldPosition(const Position2 *position, const Word cameraName, Scale2 staticOffset, Scale2 dynamicOffset) 
{
	followedWorldPosition_ = position; 
	camera_ = RenderManager::GetCamera(cameraName);
	staticFollowOffset_ = staticOffset;
	dynamicFollowOffset_ = dynamicOffset;
}

void Element::UpdateRecursively()
{
	Update();

	for(auto &child : staticChildren_)
	{
		if(child->IsLocallyActive() == false)
			continue;

		child->UpdateRecursively();
	}

	for(auto &child : dynamicChildren_)
	{
		if(child->IsLocallyActive() == false)
			continue;

		child->UpdateRecursively();
	}
}

void Element::UpdateLateRecursively()
{
	UpdateLately();

	for(auto &child : staticChildren_)
	{
		if(child->IsLocallyActive() == false)
			continue;

		child->UpdateLateRecursively();
	}

	for(auto &child : dynamicChildren_)
	{
		if(child->IsLocallyActive() == false)
			continue;

		child->UpdateLateRecursively();
	}
}

void Element::UpdateVeryLateRecursively()
{
	UpdateVeryLately();

	for(auto &child : staticChildren_)
	{
		if(child->IsLocallyActive() == false)
			continue;

		child->UpdateVeryLateRecursively();
	}

	for(auto &child : dynamicChildren_)
	{
		if(child->IsLocallyActive() == false)
			continue;

		child->UpdateVeryLateRecursively();
	}
}

void Element::UpdateExtremelyLateRecursively()
{
	UpdateExtremelyLately();

	for(auto &child : staticChildren_)
	{
		if(child->IsLocallyActive() == false)
			continue;

		child->UpdateExtremelyLateRecursively();
	}

	for(auto &child : dynamicChildren_)
	{
		if(child->IsLocallyActive() == false)
			continue;

		child->UpdateExtremelyLateRecursively();
	}
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

bool Element::CheckHover()
{
	isHovered_ = false;
	
	if(!isInteractive_)
		return false;

	if(IsGloballyActive() == false)
	{
		isHovered_ = false;
		return false;
	}

	auto mousePosition = InputHandler::GetMousePosition();

	auto position = GetGlobalPosition();

	auto size = Scale2(size_) * transform_->GetScale() * 0.5f;

	bool isInsideHorizontally = mousePosition.x > position.x - size.x && mousePosition.x < position.x + size.x;
	bool isInsideVertically = mousePosition.y > position.y - size.y && mousePosition.y < position.y + size.y;

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

	if(mouseFollower_)
	{
		mouseFollower_->Update();
	}

	if(followedWorldPosition_)
	{
		UpdateWorldFollow();
	}

	if(isUpdatingPositionConstantly_ == true || mouseFollower_ != nullptr)
	{
		UpdatePosition();
	}

	HandleUpdate();
}

void Element::UpdateLately()
{
	if(IsGloballyActive() == false)
		return;
		
	HandleLateUpdate();
}

void Element::UpdateVeryLately()
{
	if(IsGloballyActive() == false)
		return;
		
	HandleVeryLateUpdate();
}

void Element::UpdateExtremelyLately()
{
	if(IsGloballyActive() == false)
		return;
		
	HandleExtremelyLateUpdate();
}

void Element::UpdateWorldFollow()
{
	auto screenPosition = camera_->GetScreenPosition({followedWorldPosition_->x, followedWorldPosition_->y, 0.0f});

	auto offset = Scale2(size_) / 2.0f;

	screenPosition += offset * staticFollowOffset_;

	screenPosition += dynamicFollowOffset_ / camera_->GetZoomFactor();

	transform_->GetPosition().x = screenPosition.x;
	transform_->GetPosition().y = screenPosition.y;
}

void Element::Render(Camera* camera)
{
	if(isRendering_ == false)
		return;

	if(IsGloballyActive() == false)
		return;

	if(!sprite_)
		return;

	sprite_->Draw(camera);
}

void Element::AddAnimation(Animation *animation, const char *name)
{
	animator_->AddAnimation(animation, name);
}

AnimationProperty* Element::AddAnimationProperty(const char* animationName, InterfaceElementParameters parameter, const float *trackedValue)
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
		property->Initialize(&opacity_.Get());
		return property;
	case InterfaceElementParameters::OTHER:
		property = animation->AddProperty();
		property->Initialize(trackedValue);
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

const Opacity &Element::GetOpacity()
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

void Element::SetupBasicAnimations()
{
	openAnimation_ = animator_->AddAnimation(new Animation(1.0f), OPEN_ANIMATION_NAME);

	auto openLastEvent = openAnimation_->GetFinishEvent();
	openLastEvent->GetActions() += {this, &Element::Enable};
	
	closeAnimation_ = animator_->AddAnimation(new Animation(1.0f), CLOSE_ANIMATION_NAME);
	
	closeEvent_ = closeAnimation_->GetFinishEvent();
	closeEvent_->GetActions() += {this, &Element::Disable};

	HandleSetupAnimations();
}

Position2 Element::GetPosition() const
{
	auto &position = transform_->GetPosition();
	return Position2(position.x, position.y);
}

void Element::SetPosition(Position2 position) 
{
	transform_->GetPosition() = Position3(position.x, position.y, 0.0f);
}

void Element::SetBasePosition(Position2 position) 
{
	basePosition_ = position;
	
	UpdatePosition();
}

void Element::SetInteractivity(bool isInteractive)
{
	isInteractive_ = isInteractive;

	isClickable_ = isInteractive;
}

void Element::TriggerLeftClickEvents()
{
	if(!leftClickEvents_)
		return;

	leftClickEvents_->Invoke();

	HandleLeftClick();
}

void Element::TriggerRightClickEvents()
{
	if(!rightClickEvents_)
		return;

	rightClickEvents_->Invoke();

	HandleRightClick();
}

void Element::TriggerHoverEvents()
{
	if(!hoverEvents_)
		return;

	hoverEvents_->Invoke();

	HandleHover();
}

void Element::Open()
{
	if(animator_ == nullptr || isActive_ == true)
		return;

	isActive_ = true;

	openAnimation_->Play();

	HandleOpen();
}

void Element::Close()
{
	if(!animator_ || isActive_ == false)
		return;

	SetInteractivity(false);

	closeAnimation_->Play();

	HandleClose();
}

void Element::AddChild(Element* child)
{
	assert(staticChildren_.IsFull() == false && "Static Element child buffer is full.\n");

	auto childPointer = staticChildren_.Add();
	if(childPointer == nullptr)
		return;

	*childPointer = child;
}

void Element::AddDynamicChild(Element* child)
{
	assert(dynamicChildren_.IsFull() == false && "Dynamic element child buffer is full.\n");

	auto childPointer = dynamicChildren_.Add();
	if(childPointer == nullptr)
		return;

	*childPointer = child;
}

void Element::RemoveChild(Element* child)
{
	dynamicChildren_.Remove(child);
}


Element* Element::GetChild(Word identifier)
{
	for(auto &child : staticChildren_)
	{
		if(child->GetIdentifier() == identifier)
			return child;
	}

	return nullptr;
}

void Element::SetSpriteColor(const Color &color)
{
	sprite_->SetColor(&color);
}	

void Element::SetTextureScale(float scale)
{
	sprite_->SetTextureSize(Scale2(scale, scale));
}

void Element::SetTextureScale(Scale2 scale)
{
	sprite_->SetTextureSize(scale);
}

void Element::SetTexture(Word textureName)
{
	sprite_->SetTexture(textureName);

	AdjustSizeToTexture();

	UpdatePosition();
}

void Element::AdjustSizeToTexture()
{
	size_ = sprite_->GetTexture()->GetSize();
}

void Element::HandleOpen()
{
}

void Element::HandleClose()
{
}

void Element::HandleEnable()
{
	UpdatePosition();
}

void Element::HandleDisable()
{

}

void Element::HandleSetParent(Object* parent)
{
	if(parent == nullptr)
		return;

	auto parentAsElement = (Element *)parent;
	parentAsElement->AddChild(this);
}

void Element::HandleLeftClick() {}

void Element::HandleRightClick() {}

void Element::HandleHover() {}

void Element::HandleConfigure() {}

void Element::HandleInitialize() {}

void Element::HandleUpdate() {}

void Element::HandleLateUpdate() {}

void Element::HandleVeryLateUpdate() {}

void Element::HandleExtremelyLateUpdate() {}

void Element::HandleSetupAnimations() {}

void Element::SetDynamicParent(Object* parent)
{
	if(parent == nullptr)
		return;

	auto parentAsElement = (Element *)parent;
	parentAsElement->AddDynamicChild(this);

	if(parent_ != nullptr)
	{
		auto formerParentAsElement = (Element *)parent_;
		formerParentAsElement->RemoveChild(this);
	}

	parent_ = parent;
}

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