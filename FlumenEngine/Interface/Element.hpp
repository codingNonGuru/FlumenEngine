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
class AnimationEvent;
class AnimationProperty;
class Delegate;

class Element;
class Color;

class MouseFollower
{
	friend class Element;

	Element *parent;

	bool isActive {true};

	MouseFollower(Element *newParent) : parent(newParent) {}

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
	Word identifier_;

	Sprite* sprite_;

	Animator* animator_ {nullptr};

	AnimationEvent* closeEvent_;

	Array <Element *> staticChildren_;

	Pool <Element *> dynamicChildren_;

	Size size_;

	DrawOrder drawOrder_;

	Opacity opacity_;

	Position2 basePosition_;

	ElementPivots pivot_;

	ElementAnchors anchor_;

	bool isInteractive_ {false};

	bool isHovered_;

	MouseFollower *mouseFollower_ {nullptr};

	virtual void HandleOpen();

	virtual void HandleClose();

	virtual void HandleConfigure();

	virtual void HandleInitialize();

	virtual void HandleUpdate();

	virtual void HandleEnable() override;

	virtual void HandleDisable() override;

	virtual void HandleSetParent(Object *) override;

	virtual void HandleLeftClick();

	virtual void HandleRightClick();

	virtual void HandleHover();

	Position2 GetAnchorOffset(ElementAnchors);

	Position2 GetPivotOffset(ElementPivots);

	void Configure(Size, DrawOrder, PositionData, SpriteDescriptor = SpriteDescriptor(), Opacity = 1.0f);

	void UpdateRecursively();

	Element(int = DEFAULT_CHILD_COUNT);
	
public:
	void UpdatePosition();

	void SetOpacity(Opacity opacity) {opacity_ = opacity;}

	Word GetIdentifier();

	void SetIdentifier(Word);

	Sprite* GetSprite();

	void Initialize();

	void Update();

	virtual void Render(Camera*);

	Position2 GetPosition() const;

	void SetPosition(Position2);

	void SetBasePosition(Position2 position) {basePosition_ = position;}

	Animator* GetAnimator();

	Size & GetSize();

	int GetWidth() const {return size_.x;}

	int GetHeight() const {return size_.y;}

	void SetWidth(int width) {size_.x = width;}

	void SetHeight(int height) {size_.y = height;}

	DrawOrder & GetDrawOrder();

	Opacity GetOpacity();

	Delegate & GetLeftClickEvents();

	Delegate & GetRightClickEvents();

	Delegate & GetHoverEvents();

	void SetInteractivity(bool);

	void SetSpriteColor(Color &);

	AnimationProperty* AddAnimationProperty(const char*, InterfaceElementParameters);

	void SetDynamicParent(Object *);

	void AddChild(Element *);

	void AddDynamicChild(Element *);

	void RemoveChild(Element *);

	Element* GetChild(Word);

	const Array <Element *> &GetChildren() const {return staticChildren_;}

	void Open();

	void Close();

	void FollowMouse() {mouseFollower_ = new MouseFollower(this);}
};
