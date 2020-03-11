#pragma once

#include "FlumenCore/Conventions.hpp"

class Transform;
class Camera;

class Object
{
protected:
	bool isActive_;

	Transform* transform_;

	Object* parent_;

	virtual void HandleEnable();

	virtual void HandleDisable();

	virtual void HandleSetParent(Object*);

public:
	Object();

	void Enable();

	void Disable();

	bool IsGloballyActive();

	bool IsLocallyActive();

	Position3 GetLocalPosition();

	Position3 GetGlobalPosition();

	Rotation GetLocalRotation();

	Rotation GetGlobalRotation();

	Object* GetParent();

	Transform* GetTransform();

	void SetParent(Object*);

	virtual void Update();

	virtual void Render(Camera*);
};
