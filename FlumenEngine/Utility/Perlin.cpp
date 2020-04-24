#include "FlumenEngine/Utility/Perlin.hpp"

#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/ShaderManager.hpp"
#include "FlumenEngine/Render/DataBuffer.hpp"
#include "FlumenCore/Utility/Utility.hpp"

Shader* Perlin::shader_ = nullptr;

Map <DataBuffer> Perlin::buffers_ = Map <DataBuffer> (2);

DataBuffer* Perlin::targetBuffer_ = nullptr;

bool Perlin::isInitialized_ = false;

enum class Stages
{
	SET_LATTICE,
	FILL_LATTICE,
	ADD_OCTAVES
};

void Perlin::Initialize()
{
	if(isInitialized_)
	{
		return;
	}

	isInitialized_ = true;

	clock_t start = clock();

	Size capacity(4096, 4096);
	Length pixelCount = capacity.x * capacity.y;

	container::Array <float> stream(pow(2, 24));

	for(int index = 0; index < stream.GetCapacity(); ++index)
	{
		auto pixel = stream.Allocate();
		*pixel = utility::GetRandom(0.0f, 1.0f);
	}

	*buffers_.Add("result") = DataBuffer(pixelCount * sizeof(float));
	*buffers_.Add("stream") = DataBuffer(stream.GetMemoryCapacity(), stream.GetStart());

	shader_ = ShaderManager::GetShaderMap().Get("Perlin");

	std::cout<<clock() - start<<"\n";
}

void Perlin::Destroy()
{
	for(auto buffer = buffers_.GetStart(); buffer != buffers_.GetEnd(); ++buffer)
	{
		buffer->Delete();
	}
}

void Perlin::SetTargetBuffer(DataBuffer* targetBuffer)
{
	targetBuffer_ = targetBuffer;
}

DataBuffer* Perlin::Generate(Size size, FocusIndex focusIndex, ContrastThreshold contrastThreshold, ContrastStrength contrastStrength, Range range)
{
	Initialize();

	clock_t start = clock();

	int blockSize = 16;
	Size computeSize(size.x / blockSize, size.y / blockSize);

	shader_->Bind();

	if(targetBuffer_ != nullptr)
	{
		targetBuffer_->Bind(0);
		targetBuffer_ = nullptr;
	}
	else
	{
		buffers_.Get("result")->Bind(0);
	}
	buffers_.Get("stream")->Bind(1);

	shader_->SetConstant(size, "size");

	auto side = size.x;
	int horizontalExponent = 0;
	while(side % 2 == 0)
	{
		side /= 2;
		horizontalExponent++;
	}

	side = size.y;
	int verticalExponent = 0;
	while(side % 2 == 0)
	{
		side /= 2;
		verticalExponent++;
	}

	int exponent = verticalExponent > horizontalExponent ? horizontalExponent : verticalExponent;
	shader_->SetConstant(exponent, "exponent");

	unsigned int octaveCount = exponent;
	shader_->SetConstant(octaveCount, "octaveCount");

	unsigned int timeSeed = utility::GetRandom(0, pow(2, 24));
	shader_->SetConstant(timeSeed, "timeSeed");

	shader_->SetConstant(focusIndex, "strongestOctave");
	shader_->SetConstant(range, "range");
	shader_->SetConstant(contrastThreshold, "contrast");
	shader_->SetConstant(contrastStrength, "contrastStrength");

	shader_->DispatchCompute(computeSize);

	shader_->Unbind();

	std::cout<<"Finished generating Perlin texture on the GPU of size "<<size.x<<"x"<<size.y<<" in time "<<clock() - start<<" nanosecs.\n";

	return GetResultBuffer();
}

void Perlin::Download(container::Matrix* container)
{
	buffers_.Get("result")->Download(container);
}

DataBuffer* Perlin::GetResultBuffer()
{
	return buffers_.Get("result");
}
