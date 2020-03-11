#include <stdlib.h>

#include "FlumenEngine/Utility/Palette.hpp"
#include "FlumenCore/Utility/Utility.hpp"

PaletteSlot::PaletteSlot()
{
	weight_ = 1.0f;

	Reset();
}

PaletteSlot::PaletteSlot(Color color, Weight weight = 1.0f) : color_(color), weight_(weight)
{
	Reset();
}

PaletteSlot::PaletteSlot(Color color, Range saturationRange, Range valueRange, Weight weight = 1.0f) : color_(color), saturationRange_(saturationRange), valueRange_(valueRange), weight_(weight)
{
	strength_ = 1.0f;
}

Color PaletteSlot::GetColor()
{
	return color_;
}

Range PaletteSlot::GetSaturationRange()
{
	return saturationRange_;
}

Range PaletteSlot::GetValueRange()
{
	return valueRange_;
}

Weight PaletteSlot::GetWeight()
{
	return weight_;
}

void PaletteSlot::Reset()
{
	strength_ = 1.0f;

	saturationRange_ = Range(0.0f, 1.0f);

	valueRange_ = Range(0.0f, 1.0f);
}

Color Palette::GetColor()
{
	float finalWeight = utility::GetRandom(0.0f, totalWeight_);

	Index index = 0;
	float startWeight = 0.0f;
	for(auto slot = slots_.GetStart(); slot != slots_.GetEnd(); ++slot, ++index)
	{
		bool isInsideRange = (finalWeight >= startWeight) && (finalWeight <= startWeight + slot->GetWeight());

		startWeight += slot->GetWeight();

		if(!isInsideRange)
			continue;

		return GetColor(index, true, true);
	}
}

Color Palette::GetColor(int index)
{
	return slots_[index].GetColor();
}

Color Palette::GetColor(int index, bool hasSaturation, bool hasValue)
{
	auto slot = slots_[index];
	auto color = slot.GetColor();

	if(hasSaturation)
	{
		float saturation = utility::GetRandom(slot.GetSaturationRange().x, slot.GetSaturationRange().y);

		color = Color::AddSaturation(color, saturation);
	}

	if(hasValue)
	{
		float value = utility::GetRandom(slot.GetValueRange().x, slot.GetValueRange().y);

		color = Color::Lighten(color, value);
	}

	return color;
}

void Palette::Add(Color color, Weight weight = 1.0f)
{
	auto slot = slots_.Allocate();
	if(!slot)
	{
		return;
	}

	*slot = PaletteSlot(color, weight);

	totalWeight_ += weight;
}

void Palette::Add(Color color, Range saturationRange, Range valueRange, Weight weight = 1.0f)
{
	auto slot = slots_.Allocate();
	if(!slot)
	{
		return;
	}

	*slot = PaletteSlot(color, saturationRange, valueRange, weight);

	totalWeight_ += weight;
}
