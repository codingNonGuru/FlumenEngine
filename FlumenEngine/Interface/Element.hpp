#pragma once

#include <glm/glm.hpp>

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Core/Object.hpp"
#include "FlumenEngine/Core/Types.hpp"
#include "FlumenEngine/Interface/ElementData.h"

#define DEFAULT_CHILD_COUNT 32

class Sprite;
class Camera;
class Transform;
class Animator;
class Animation;
class AnimationEvent;
class AnimationProperty;
class Delegate;

class Element;
class Color;

class MouseFollower
{
	friend class Element;

	Element *parent;

	Position2 offset;

	bool isActive {true};

	MouseFollower(Element *newParent, Position2 newOffset) : parent(newParent), offset(newOffset) {}

	void Update();
};

class Element : public Object
{
	friend class Interface;

	friend class ElementFactory;

	friend class MouseFollower;

	void TriggerRightClickEvents();
	
	void TriggerLeftClickEvents();

	void TriggerHoverEvents();

	bool CheckHover();

	Delegate * leftClickEvents_ {nullptr};

	Delegate * rightClickEvents_ {nullptr};

	Delegate * hoverEvents_ {nullptr};

protected:
	bool isRendering_;

	Word identifier_;

	Sprite* sprite_;

	Animator* animator_ {nullptr};

	Animation *openAnimation_ {nullptr};

	Animation *closeAnimation_ {nullptr};

	AnimationEvent* closeEvent_;

	Array <Element *> staticChildren_;

	Pool <Element *> dynamicChildren_;

	Size size_;

	DrawOrder drawOrder_;

	Opacity opacity_;

	Position2 basePosition_;

	ElementPivots pivot_;

	ElementAnchors anchor_;

	bool isUpdatingPositionConstantly_ {false};

	bool isInteractive_ {false};

	bool isClickable_ {false};

	bool isHovered_;

	MouseFollower *mouseFollower_ {nullptr};

	const Position2 *followedWorldPosition_ {nullptr};

	Scale2 staticFollowOffset_;

	Scale2 dynamicFollowOffset_;

	const Camera *camera_ {nullptr};

	virtual void HandleOpen();

	virtual void HandleClose();

	virtual void HandleConfigure();

	virtual void HandleConfigure(AdditionalElementData *);

	virtual void HandleInitialize();

	virtual void HandleUpdate();

	virtual void HandleLateUpdate();

	virtual void HandleVeryLateUpdate();

	virtual void HandleExtremelyLateUpdate();

	virtual void HandleEnable() override;

	virtual void HandleDisable() override;

	virtual void HandleSetParent(Object *) override;

	virtual void HandleLeftClick();

	virtual void HandleRightClick();

	virtual void HandleHover();

	virtual void HandleSetupAnimations();

	Position2 GetAnchorOffset(ElementAnchors);

	Position2 GetPivotOffset(ElementPivots);

	Element() {}

	void Initialize(int = DEFAULT_CHILD_COUNT);

	void Configure(Size, DrawOrder, PositionData, SpriteDescriptor = SpriteDescriptor(), Opacity = Opacity(1.0f), AdditionalElementData * = nullptr);

	void UpdateRecursively();

	void UpdateLateRecursively();

	void UpdateVeryLateRecursively();

	void UpdateExtremelyLateRecursively();

	void UpdateWorldFollow();
	
public:
	void UpdatePosition();

	void UpdatePositionConstantly() {isUpdatingPositionConstantly_ = true;}

	void SetOpacity(Opacity opacity) {opacity_ = opacity;}

	void SetOpacity(float opacity) {opacity_ = opacity;}

	Word GetIdentifier();

	void SetIdentifier(Word);

	Sprite* GetSprite();

	void Update();

	void UpdateLately();

	void UpdateVeryLately();

	void UpdateExtremelyLately();

	virtual void Render(Camera*);

	bool IsHovered() const {return isHovered_;}

	Position2 GetPosition() const;

	const Position2 &GetBasePosition() const {return basePosition_;}

	void SetPosition(Position2);

	void SetBasePosition(Position2);

	Animator* GetAnimator();

	void SetupBasicAnimations();

	Size & GetSize();

	int GetWidth() const {return size_.x;}

	int GetHeight() const {return size_.y;}

	void SetWidth(int width) {size_.x = width;}

	void SetHeight(int height) {size_.y = height;}

	void AdjustSizeToTexture();

	DrawOrder & GetDrawOrder();

	const Opacity &GetOpacity();

	Delegate & GetLeftClickEvents();

	Delegate & GetRightClickEvents();

	Delegate & GetHoverEvents();

	void SetInteractivity(bool);

	void MakeTransparentToClicks() {isClickable_ = false;}

	bool IsClickable() const {return isClickable_;}

	void SetSpriteColor(const Color &);

	void SetTextureScale(Scale2);

	void SetTextureScale(float);

	void SetTexture(Word);

	void AddAnimation(Animation *, const char *);

	AnimationProperty* AddAnimationProperty(const char*, InterfaceElementParameters, const float * = nullptr);

	void SetDynamicParent(Object *);

	void AddChild(Element *);

	void AddDynamicChild(Element *);

	void RemoveChild(Element *);

	Element* GetChild(Word);

	const Array <Element *> &GetChildren() const {return staticChildren_;}

	void Open();

	void Close();

	void FollowMouse(Position2 offset = Position2()) {mouseFollower_ = new MouseFollower(this, offset);}

	void FollowWorldPosition(const Position2 *position, const Word camera, Scale2 staticOffset = Position2(1.0f), Scale2 dynamicOffset = Position2(0.0f));

	void SetAnchorData(ElementAnchors anchor, ElementPivots pivot) {anchor_ = anchor; pivot_ = pivot;}

	void SetRendering(bool isRendering) {isRendering_ = isRendering;}
};
