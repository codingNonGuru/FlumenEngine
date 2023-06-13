#include "Object.hpp"
#include "FlumenEngine/Core/Transform.hpp"

Object::Object()
{
	isActive_ = false;
	
	parent_ = nullptr;

	transform_ = nullptr;
}

void Object::Enable()
{
	isActive_ = true;

	HandleEnable();
}

void Object::Disable()
{
	isActive_ = false;

	HandleDisable();
}

void Object::HandleEnable() {}

void Object::HandleDisable() {}

void Object::HandleSetParent(Object* parent) {}

bool Object::IsLocallyActive()
{
	return isActive_;
}

bool Object::IsGloballyActive()
{
	bool isActive = isActive_;
	if(isActive == false)
		return false;

	Object* parent = parent_;

	while(true)
	{
		if(parent == nullptr)
			break;

		if(parent->IsLocallyActive() == false)
		{
			return false;
		}

		parent = parent->parent_;
	}

	return true;
}

Position3 Object::GetLocalPosition()
{
	return transform_->GetPosition();
}

Position3 Object::GetGlobalPosition()
{
	//if(!isAttached_)
		//return transform_->position_;

	Object* parent = parent_;
	Position3 position = transform_->GetPosition();

	while(true)
	{
		if(!parent)
			break;

		auto parentTransform = parent->GetTransform();

		float rotation = parentTransform->GetRotation();
		float s = sin(rotation);
		float c = cos(rotation);

		float x = position.x * c - position.y * s;
		float y = position.x * s + position.y * c;

		auto parentPosition = parentTransform->GetPosition();
		position.x = parentPosition.x + x;
		position.y = parentPosition.y + y;
		position.z = 0.0f;

		parent = parent->GetParent();
	}

	return position;
}

Rotation Object::GetLocalRotation()
{
	return transform_->GetRotation();
}

Rotation Object::GetGlobalRotation()
{
	Rotation rotation = transform_->GetRotation();

	if(parent_ == nullptr)
		return rotation;

	Object* parent = parent_;
	while(true)
	{
		if(parent)
		{
			rotation += parent->GetTransform()->GetRotation();

			parent = parent->GetParent();
		}
		else
		{
			break;
		}
	}

	return rotation;
}

Object* Object::GetParent()
{
	return parent_;
}

Transform* Object::GetTransform()
{
	return transform_;
}

void Object::SetParent(Object* parent)
{
	parent_ = parent;

	HandleSetParent(parent);
}

void Object::Update()
{

}

void Object::Render(Camera* camera)
{
}
