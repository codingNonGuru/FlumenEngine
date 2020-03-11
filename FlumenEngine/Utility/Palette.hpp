#pragma once

#include "FlumenCore/Conventions.hpp"

#include "FlumenEngine/Utility/Color.hpp"

class PaletteSlot
{
	Color color_;

	Weight weight_;

	float strength_;

	Range saturationRange_;

	Range valueRange_;

public:
	PaletteSlot();

	PaletteSlot(Color, Weight);

	PaletteSlot(Color, Range, Range, Weight);

	Color GetColor();

	Range GetSaturationRange();

	Range GetValueRange();

	Weight GetWeight();

	void Reset();
};

class Palette
{
	Array <PaletteSlot> slots_;

	Weight totalWeight_;

public:
	Palette()
	{
		totalWeight_ = 0.0f;
	}

	Palette(int slotCount)
	{
		slots_.Initialize(slotCount);

		totalWeight_ = 0.0f;
	}

	Color GetColor();

	Color GetColor(int);

	Color GetColor(int, bool, bool);

	void Add(Color, Weight);

	void Add(Color, Range, Range, Weight);
};
