#include "Kernel.hpp"

Kernel::Kernel() {}

Kernel::Kernel(int side)
{
	side_ = side;

	values_.Initialize(2 * side + 1, 2 * side + 1);
}

int Kernel::GetSide()
{
	return side_;
}

int Kernel::GetSpread()
{
	return side_ * 2 + 1;
}

Grid <float> & Kernel::GetValues()
{
	return values_;
}

float* Kernel::get(int x, int y)
{
	return values_(x + side_, y + side_);
}

float* Kernel::operator() (int x, int y)
{
	return values_(x + side_, y + side_);
}

void Kernel::Initialize(KernelTypes type, float variable, float contrastStrength, bool normalized)
{
	sum_ = 0.0;
	for(int y = -side_; y <= side_; ++y)
		for(int x = -side_; x <= side_; ++x)
		{
			float distance = (float)x * (float)x + (float)y * (float)y;
			auto currentValue = values_(x + side_, y + side_);

			if(type == KernelTypes::GAUSS)
				*currentValue = exp(-distance / variable);
			else if(type == KernelTypes::CAUCHY)
				*currentValue = 1.0f / (1.0f + sqrt(distance) * variable);
			else if(type == KernelTypes::LAPLACE)
				*currentValue = exp(-sqrt(distance) / variable);

			*currentValue = pow(*currentValue, pow(contrastStrength, 4.0f - 8.0f * *currentValue));

			sum_ += *currentValue;
		}

	if(normalized)
	{
		//sum /= pow(2.0 * (double)side_ + 1.0, 2);
		for(int y = -side_; y <= side_; ++y)
			for(int x = -side_; x <= side_; ++x)
			{
				*values_(x + side_, y + side_) /= sum_;
			}
	}

	weights_.Initialize(side_ * 2 + 1);
	for(int x = 0; x < values_.GetWidth(); ++x)
	{
		*weights_.Allocate() = 0.0f;

		for(int y = 0; y < values_.GetHeight(); ++y)
		{
			weights_[x] += *values_.Get(x, y);
		}
	}
}

void Kernel::AddLayer(KernelTypes type, float variable, float contrastStrength, bool normalized, float weight)
{
	double sum = 0.0;
	for(int y = -side_; y <= side_; ++y)
		for(int x = -side_; x <= side_; ++x)
		{
			float distance = (float)x * (float)x + (float)y * (float)y;
			auto currentValue = values_(x + side_, y + side_);

			if(type == KernelTypes::GAUSS)
				*currentValue += exp(-distance / variable) * weight;
			else if(type == KernelTypes::CAUCHY)
				*currentValue += (1.0f / (1.0f + sqrt(distance) * variable)) * weight;
			else if(type == KernelTypes::LAPLACE)
				*currentValue += exp(-sqrt(distance) / variable) * weight;

			//*currentValue = pow(*currentValue, pow(contrastStrength, 4.0f - 8.0f * *currentValue));

			if(normalized == true)
				sum += *currentValue;
		}

	if(normalized == true)
	{
		for(int y = -side_; y <= side_; ++y)
			for(int x = -side_; x <= side_; ++x)
			{
				*values_(x + side_, y + side_) /= (float)sum;
			}
	}
}

Array <float> & Kernel::GetWeights()
{
	return weights_;
}
