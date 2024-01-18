#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Singleton.h"

class Element;

class Interface : public core::Singleton <Interface>
{
	friend class Element;

	friend class ElementFactory;

private:
	Map <Element*, Word> elements_;

	Array <Element *> canvases;

	Element* hoveredElement_;

	bool isSorted_;

	void Sort();

	void ProcessInput();

	Element* AddElement(Word, Element*);

	void AddCanvas(Element*);

public:
	void Initialize();

	void Update();

	void Render();

	Map <Element*, Word> & GetElements();

	Element* GetElement(Word);

	Element* GetHoveredElement();
};
