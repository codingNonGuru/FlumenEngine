#pragma once

#include "FlumenCore/Conventions.hpp"

class Element;

class Interface
{
private:
	static Map <Element*, Word> elements_;

	static Element* hoveredElement_;

	static bool isSorted_;

	static void Sort();

public:
	static void Initialize();

	static void Update();

	static void Render();

	static Map <Element*, Word> & GetElements();

	static Element* GetElement(Word);

	static Element* AddElement(Word, Element*);

	static Element* GetHoveredElement();
};
