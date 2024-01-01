#include "Interface.hpp"
#include "Element.hpp"
#include "FlumenEngine/Core/InputHandler.hpp"
#include "FlumenEngine/Render/RenderManager.hpp"

#define MAXIMUM_ELEMENT_COUNT 4096

Map <Element*, Word> Interface::elements_ = Map <Element*, Word> (MAXIMUM_ELEMENT_COUNT);

Element* Interface::hoveredElement_ = nullptr;

bool Interface::isSorted_ = false;

Array <Element*> sortedElements = Array <Element*> (MAXIMUM_ELEMENT_COUNT);

void Interface::Initialize() 
{
	InputHandler::OnInputUpdate += &Interface::ProcessInput;
}

void Interface::ProcessInput()
{
	Element* firstElement = nullptr;
	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		if(!element->CheckHover())
			continue;

		if(!firstElement)
		{
			firstElement = element;
		}
		else
		{
			if(element->GetDrawOrder() > firstElement->GetDrawOrder())
			{
				firstElement = element;
			}
		}
	}

	hoveredElement_ = firstElement;

	if(firstElement && InputHandler::GetMouse().CurrentLeft_)
	{
		firstElement->TriggerLeftClickEvents();
	}

	if(firstElement && InputHandler::GetMouse().CurrentRight_)
	{
		firstElement->TriggerRightClickEvents();
	}

	if(firstElement)
	{
		firstElement->TriggerHoverEvents();
	}
}

void Interface::Update()
{
	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		element->Update();
	}
}

void Interface::Render()
{
	if(!isSorted_)
	{
		Sort();
	}

	auto camera = RenderManager::GetInterfaceCamera();

	for(auto elementIterator = sortedElements.GetStart(); elementIterator != sortedElements.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		element->Render(camera);
	}
}

void Interface::Sort()
{
	for(int i = 0; i < sortedElements.GetSize(); ++i)
	{
		for(auto elementIterator = sortedElements.GetStart(); elementIterator != sortedElements.GetEnd(); ++elementIterator)
		{
			auto element = *elementIterator;

			if(elementIterator == sortedElements.GetEnd() - 1)
				continue;

			auto nextElement = *(elementIterator + 1);

			if(element->GetDrawOrder() < nextElement->GetDrawOrder())
				continue;

			*elementIterator = *(elementIterator + 1);
			*(elementIterator + 1) = element;
		}
	}

	isSorted_ = true;
}

Element* Interface::GetElement(Word name)
{
	auto elementPointer = elements_.Get(name);
	if(!elementPointer)
		return nullptr;

	return *elementPointer;
}

Map <Element*, Word> & Interface::GetElements()
{
	return elements_;
}

Element* Interface::AddElement(Word name, Element* element)
{
	auto elementPointer = elements_.Add(name);
	if(!elementPointer)
		return nullptr;

	*elementPointer = element;

	element->SetIdentifier(name);

	isSorted_ = false;

	*sortedElements.Add() = element;

	return element;
}

Element* Interface::GetHoveredElement()
{
	return hoveredElement_;
}
