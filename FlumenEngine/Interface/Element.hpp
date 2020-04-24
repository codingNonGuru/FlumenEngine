#pragma once

#include <glm/glm.hpp>

#include "FlumenEngine/Core/Object.hpp"
#include "FlumenCore/Conventions.hpp"
#include "FlumenEngine/Core/Types.hpp"

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

public:
	//Element();

	Element();

	void Configure(Size, Transform*, Sprite*);

	void Configure(Size, DrawOrder, Position2, Sprite* = nullptr, Opacity = 1.0f);

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
