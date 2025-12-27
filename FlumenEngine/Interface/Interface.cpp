#include "Interface.hpp"
#include "Element.hpp"
#include "Scroller.h"
#include "FlumenEngine/Core/InputHandler.hpp"
#include "FlumenEngine/Render/RenderManager.hpp"

#define MAXIMUM_ELEMENT_COUNT 16384

#define MAXIMUM_CANVAS_COUNT 256

Array <Element*> sortedElements = Array <Element*> (MAXIMUM_ELEMENT_COUNT);

void Interface::Initialize()
{
	elements_ = Map <Element*, Word> (MAXIMUM_ELEMENT_COUNT);

	hoveredElement_ = nullptr;

	isSorted_ = false;

	areScrollersSorted = false;

	InputHandler::OnInputUpdate += {this, &Interface::ProcessInput};

	canvases.Initialize(MAXIMUM_CANVAS_COUNT);
}

void Interface::ProcessInput()
{
	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		element->CheckHover();
	}

	Element* firstHoveredElement = nullptr;
	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		if(element->IsHovered() == false)
			continue;

		if(firstHoveredElement == nullptr)
		{
			firstHoveredElement = element;
		}
		else
		{
			if(element->GetDrawOrder() > firstHoveredElement->GetDrawOrder())
			{
				firstHoveredElement = element;
			}
		}
	}

	Element* firstClickableElement = nullptr;
	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		if(element->IsHovered() == false)
			continue;

		if(element->IsClickable() == false)
			continue;

		if(firstClickableElement == nullptr)
		{
			firstClickableElement = element;
		}
		else
		{
			if(element->GetDrawOrder() > firstClickableElement->GetDrawOrder())
			{
				firstClickableElement = element;
			}
		}
	}

	Element* firstScrollableElement = nullptr;
	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		if(element->IsHovered() == false)
			continue;

		if(element->IsScrollable() == false)
			continue;

		if(firstScrollableElement == nullptr)
		{
			firstScrollableElement = element;
		}
		else
		{
			if(element->GetDrawOrder() > firstScrollableElement->GetDrawOrder())
			{
				firstScrollableElement = element;
			}
		}
	}

	hoveredElement_ = firstHoveredElement;

	if(firstClickableElement != nullptr && InputHandler::GetMouse().CurrentLeft_ == true)
	{
		firstClickableElement->TriggerLeftClickEvents();
	}

	if(firstClickableElement != nullptr && InputHandler::GetMouse().CurrentRight_ == true)
	{
		firstClickableElement->TriggerRightClickEvents();
	}

	if(firstHoveredElement != nullptr)
	{
		firstHoveredElement->TriggerHoverEvents();
	}

	if(firstScrollableElement != nullptr)
	{
		if(InputHandler::GetMouse().ScrollUp_ == true)
		{
			firstScrollableElement->TriggerScrollUpEvents();
		}
		else if(InputHandler::GetMouse().ScrollDown_ == true)
		{
			firstScrollableElement->TriggerScrollDownEvents();
		}
	}
}

void Interface::Update()
{
	for(auto &canvas : canvases)
	{
		if(canvas->IsGloballyActive() == false)
			continue;

		canvas->UpdateRecursively();
	}

	for(auto &canvas : canvases)
	{
		if(canvas->IsGloballyActive() == false)
			continue;

		canvas->UpdateLateRecursively();
	}

	for(auto &canvas : canvases)
	{
		if(canvas->IsGloballyActive() == false)
			continue;

		canvas->UpdateVeryLateRecursively();
	}

	for(auto &canvas : canvases)
	{
		if(canvas->IsGloballyActive() == false)
			continue;

		canvas->UpdateExtremelyLateRecursively();
	}
}

void Interface::Render()
{
	if(!isSorted_)
	{
		Sort();
	}

	/*if(areScrollersSorted == false)
	{
		SortScrollers();
	}*/

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

void Interface::AddCanvas(Element *canvas)
{
	*canvases.Add() = canvas;
}

Element* Interface::GetHoveredElement()
{
	return hoveredElement_;
}
