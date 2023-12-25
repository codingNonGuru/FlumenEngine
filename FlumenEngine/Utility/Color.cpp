#include <iostream>

#include "FlumenEngine/Utility/Color.hpp"

Color Color::BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);

Color Color::WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);

Color Color::RED = Color(1.0f, 0.0f, 0.0f, 1.0f);

Color Color::BLUE = Color(0.0f, 0.0f, 1.0f, 1.0f);

Color Color::GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);

Color Color::DARK_GREEN = Color(0.0f, 0.5f, 0.0f, 1.0f);

Color Color::YELLOW = Color(1.0f, 1.0f, 0.0f, 1.0f);

Color Color::CYAN = Color(0.0f, 1.0f, 1.0f, 1.0f);

Color Color::MAGENTA = Color(1.0f, 0.0f, 1.0f, 1.0f);

float Color::GetSaturation(Color color)
{
	Color result;

	float average = (color.r_ + color.g_ + color.b_) / 3.0f;
	float maxRate;
	if(color.r_ > average)
		result.r_ = abs(average - color.r_) / (1.0f - average);
	else
		result.r_ = abs(average - color.r_) / average;

	if(color.g_ > average)
		result.g_ = abs(average - color.g_) / (1.0f - average);
	else
		result.g_ = abs(average - color.g_) / average;

	if(color.b_ > average)
		result.b_ = abs(average - color.b_) / (1.0f - average);
	else
		result.b_ = abs(average - color.b_) / average;

	if(result.r_ > result.g_)
		maxRate = result.r_;
	else
		maxRate = result.g_;
	if(result.b_ > maxRate)
		maxRate = result.b_;

	return maxRate;
}

Color Color::SetSaturation(Color color, float rate)
{
	Color result;

	float average = (color.r_ + color.g_ + color.b_) / 3.0f;
	float maxRate;
	if(color.r_ > average)
		result.r_ = abs(average - color.r_) / (1.0f - average);
	else
		result.r_ = abs(average - color.r_) / average;

	if(color.g_ > average)
		result.g_ = abs(average - color.g_) / (1.0f - average);
	else
		result.g_ = abs(average - color.g_) / average;

	if(color.b_ > average)
		result.b_ = abs(average - color.b_) / (1.0f - average);
	else
		result.b_ = abs(average - color.b_) / average;

	if(result.r_ > result.g_)
		maxRate = result.r_;
	else
		maxRate = result.g_;
	if(result.b_ > maxRate)
		maxRate = result.b_;

	result.r_ = abs(((average - color.r_) / maxRate) * rate - average);
	if(result.r_ > 1.0f)
		result.r_ = 1.0f;
	result.g_ = abs(((average - color.g_) / maxRate) * rate - average);
	if(result.g_ > 1.0f)
		result.g_ = 1.0f;
	result.b_ = abs(((average - color.b_) / maxRate) * rate - average);
	if(result.b_ > 1.0f)
		result.b_ = 1.0f;

	result.a_ = color.a_;

	return result;
}

Color Color::AddSaturation(Color color, float amount) 
{
	Color result;

	if(color.r_ == color.g_ && color.g_ == color.b_)
		return color;

	float average = (color.r_ + color.g_ + color.b_) / 3.0f; 
	float maxRate;
	if(color.r_ > average)
		result.r_ = abs(average - color.r_) / (1.0f - average); 
	else
		result.r_ = abs(average - color.r_) / average;

	if(color.g_ > average)
		result.g_ = abs(average - color.g_) / (1.0f - average); 
	else
		result.g_ = abs(average - color.g_) / average;

	if(color.b_ > average)
		result.b_ = abs(average - color.b_) / (1.0f - average); 
	else
		result.b_ = abs(average - color.b_) / average;

	if(result.r_ > result.g_)
		maxRate = result.r_; 
	else
		maxRate = result.g_;
	if(result.b_ > maxRate)
		maxRate = result.b_;

	float rate = amount + maxRate; 
	if(rate > 1.0f)
		rate = 0.9999f;
	if(rate < 0.0f)
		rate = 0.0f;

	result.r_ = abs(((average - color.r_) / maxRate) * rate - average);
	if(result.r_ > 1.0f)
		result.r_ = 1.0f;
	result.g_ = abs(((average - color.g_) / maxRate) * rate - average);
	if(result.g_ > 1.0f)
		result.g_ = 1.0f;
	result.b_ = abs(((average - color.b_) / maxRate) * rate - average);
	if(result.b_ > 1.0f)
		result.b_ = 1.0f;

	result.a_ = color.a_;

	return result;
}

Color Color::Lighten(Color color, float rate)
{
	Color result;

	if(rate < 0.0f)
	{
		result = color * (1.0f + rate);
	}
	else
	{
		result = (Color(1.0f) - color) * rate + color;

		//(Color(1.0f) - color) * rate + color;
	}

	result.a_ = color.a_;

	return result;
}

Color::Color() {}

Color::Color(float value) : r_(value), g_(value), b_(value), a_(value)
{
}

Color::Color(float r, float g, float b) : r_(r), g_(g), b_(b), a_(1.0f)
{
}

Color::Color(float r, float g, float b, float a) : r_(r), g_(g), b_(b), a_(a)
{
}

Color::Color(Float4 data) : r_(data.r), g_(data.g), b_(data.b), a_(data.a)
{	
}

Color Color::operator + (const Color& other)
{
	Color result;

	result.r_ = this->r_ + other.r_;
	result.g_ = this->g_ + other.g_;
	result.b_ = this->b_ + other.b_;
	result.a_ = this->a_ + other.a_;

	return result;
}

Color Color::operator * (float factor)
{
	Color result;

	result.r_ = r_ * factor;
	result.g_ = g_ * factor;
	result.b_ = b_ * factor;
	result.a_ = a_ * factor;

	return result;
}

Color Color::operator - (Color& other)
{
	Color result;

	result.r_ = r_ - other.r_;
	result.g_ = g_ - other.g_;
	result.b_ = b_ - other.b_;
	result.a_ = a_ - other.a_;

	return result;
}

Color::operator Float4 ()
{
	Float4 result;

	result.r = r_;
	result.g = g_;
	result.b = b_;
	result.a = a_;

	return result;
}
