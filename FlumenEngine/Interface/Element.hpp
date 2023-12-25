#pragma once

#include <glm/glm.hpp>

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Core/Object.hpp"
#include "FlumenEngine/Core/Types.hpp"
#include "FlumenEngine/Interface/ElementData.h"

class Sprite;
class Camera;
class Transform;
class Animator;
class AnimationEvent;
class AnimationProperty;
class Delegate;

class Element : public Object
{
	friend class Interface;

	friend class ElementFactory;

	void TriggerRightClickEvents();
	
	void TriggerLeftClickEvents();

	void TriggerHoverEvents();

	bool CheckHover();

	Delegate * leftClickEvents_;

	Delegate * rightClickEvents_;

	Delegate * hoverEvents_;

protected:
	Word identifier_;

	Sprite* sprite_;

	Animator* animator_;

	AnimationEvent* closeEvent_;

	Array <Element*> children_;

	Size size_;

	DrawOrder drawOrder_;

	Opacity opacity_;

	Position2 basePosition_;

	ElementPivots pivot_;

	ElementAnchors anchor_;

	bool isInteractive_;

	bool isHovered_;

	virtual void HandleOpen();

	virtual void HandleClose();

	virtual void HandleConfigure();

	virtual void HandleInitialize();

	virtual void HandleUpdate();

	virtual void HandleEnable() override;

	virtual void HandleDisable() override;

	virtual void HandleSetParent(Object*) override;

	Position2 GetAnchorOffset(ElementAnchors);

	Position2 GetPivotOffset(ElementPivots);

	void Configure(Size, DrawOrder, PositionData, SpriteDescriptor = SpriteDescriptor(), Opacity = 1.0f);

	void UpdatePosition();

	Element();
	
public:
	void SetOpacity(Opacity opacity) {opacity_ = opacity;}

	Word GetIdentifier();

	void SetIdentifier(Word);

	Sprite* GetSprite();

	void Initialize();

	void Update();

	virtual void Render(Camera*);

	Animator* GetAnimator();

	Size & GetSize();

	DrawOrder & GetDrawOrder();

	Opacity GetOpacity();

	Delegate & GetLeftClickEvents();

	Delegate & GetRightClickEvents();

	Delegate & GetHoverEvents();

	void SetInteractivity(bool);

	AnimationProperty* AddAnimationProperty(const char*, InterfaceElementParameters);

	void AddChild(Element*);

	Element* GetChild(Word);

	void Open();

	void Close();
};
