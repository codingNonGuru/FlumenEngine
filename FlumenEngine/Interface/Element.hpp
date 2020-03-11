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

	Delegate * clickEvents_;

	Delegate * hoverEvents_;

	virtual void HandleOpen();

	virtual void HandleClose();

	virtual void HandleConfigure();

	virtual void HandleInitialize();

	virtual void HandleUpdate();

	virtual void HandleEnable() override;

	virtual void HandleDisable() override;

	virtual void HandleSetParent(Object*) override;

public:
	Element();

	Element(Size, Transform*, Sprite*);

	Element(Size, DrawOrder, Transform*, Sprite*, Opacity = 1.0f);

	void Configure(Size, Transform*, Sprite*);

	void Configure(Size, DrawOrder, Transform*, Sprite*, Opacity = 1.0f);

	Word GetIdentifier();

	void SetIdentifier(Word);

	Sprite* GetSprite();

	void Initialize();

	bool CheckHover();

	void Update();

	virtual void Render(Camera*);

	Animator* GetAnimator();

	Size & GetSize();

	DrawOrder & GetDrawOrder();

	Opacity GetOpacity();

	Delegate & GetClickEvents();

	Delegate & GetHoverEvents();

	void SetInteractivity(bool);

	AnimationProperty* AddAnimationProperty(const char*, InterfaceElementParameters);

	void AddChild(Element*);

	Element* GetChild(Word);

	void HandleClick();

	void HandleHover();

	void Open();

	void Close();
};
