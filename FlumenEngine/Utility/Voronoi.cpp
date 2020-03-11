#include "GL/glew.h"

#include "FlumenEngine/Utility/Voronoi.hpp"
#include "FlumenEngine/Render/DataBuffer.hpp"
#include "FlumenEngine/Render/ShaderManager.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "Kernel.hpp"
#include "FlumenCore/Utility/Utility.hpp"

Shader* Voronoi::shader_ = nullptr;

Map <DataBuffer> Voronoi::buffers_ = Map <DataBuffer> (6);

enum class Stages
{
	INITIALIZE,
	FLOOD,
	SWAP,
	SEPARATE,
	RELAX,
	SWAP_RESULT,
	TURN_SEED
};

Kernel kernel(30);

void Voronoi::Initialize()
{
	container::Grid<float> pixels(4096, 4096);
	for(auto pixel = pixels.GetStart(); pixel != pixels.GetEnd(); ++pixel)
		*pixel = 0.0f;

	int pixelCount = 4096 * 4096;

	*buffers_.Add("seed") = DataBuffer(pixelCount * sizeof(int));
	*buffers_.Add("neighbour") = DataBuffer(pixelCount * sizeof(int) * 2);
	*buffers_.Add("neighbourSwap") = DataBuffer(pixelCount * sizeof(int) * 2);
	*buffers_.Add("result") = DataBuffer(pixelCount * sizeof(float));
	*buffers_.Add("resultSwap") = DataBuffer(pixelCount * sizeof(float));

	kernel.Initialize(KernelTypes::GAUSS, 100.0f, 1.0f, true);
	float weights[61];
	auto kernelValues = kernel.GetValues();
	for(int x = 0; x < kernelValues.GetWidth(); ++x)
	{
		weights[x] = 0.0f;
		for(int y = 0; y < kernelValues.GetHeight(); ++y)
			weights[x] += *kernelValues.Get(x, y);
	}
	*buffers_.Add("kernel") = DataBuffer(sizeof(weights), weights);

	shader_ = ShaderManager::GetShaderMap().Get("Voronoi");
}

void Voronoi::Destroy()
{
	for(auto buffer = buffers_.GetStart(); buffer != buffers_.GetEnd(); ++buffer)
	{
		buffer->Delete();
	}
}

void Voronoi::LoadSeeds(Grid<unsigned int> &seeds)
{
	auto seedBuffer = buffers_.Get("seed");
	if(!seedBuffer)
		return;

	seedBuffer->UploadData(seeds.GetStart(), seeds.GetMemorySize());
}

void Voronoi::LoadRandomTriangulation(int32_t width, int32_t height, uint32_t &seedCount)
{
	container::Grid<unsigned int> seeds(width, height);
	for(auto pixel = seeds.GetStart(); pixel != seeds.GetEnd(); ++pixel)
		*pixel = 0;

	unsigned int seedIndex = 1;
	bool flip = true;
	float horIncr = 64.0f;
	float vertIncr = horIncr * 0.84f;
	float vertDif = float(seeds.GetHeight()) / vertIncr;
	vertDif = float(seeds.GetHeight()) - float(int(vertDif)) * vertIncr;
	vertDif *= 0.5f;

	for(float y = 10.0f; y < seeds.GetHeight(); y += vertIncr)
	{
		for(float x = (flip == true ? 1.0f : 1.0f + horIncr * 0.5f); x < seeds.GetWidth(); x += horIncr)
		{
			float angle = utility::GetRandom(0.0f, 6.2831f);
			float radius = utility::GetRandom(12.0f, 20.0f);
			int offX = float(x) + radius * cos(angle);
			int offY = float(y) + radius * sin(angle);
			*seeds(offX, offY) = seedIndex;
			seedIndex++;
		}
		flip = flip == true ? false : true;
	}
	seedCount = seedIndex;
	LoadSeeds(seeds);
}

void Voronoi::Generate(bool isDownload, container::Grid<float> &result, container::Grid<unsigned int> *finalSeeds, unsigned int relaxPassCount)
{
	clock_t start = clock();
	clock_t floodTime, relaxTime, allocateTime, downloadTime;
	allocateTime = clock();
	allocateTime = clock() - allocateTime;

	shader_->Bind();

	buffers_.Get("seed")->Bind(0);
	buffers_.Get("neighbour")->Bind(1);
	buffers_.Get("neighbourSwap")->Bind(2);
	buffers_.Get("result")->Bind(3);
	buffers_.Get("resultSwap")->Bind(4);

	Size size = Size(result.GetWidth(), result.GetHeight());
	int blockSize = 16;
	Size computeSize = Size(size.x / blockSize, size.y / blockSize);

	//Initialize
	shader_->SetConstant(size, "size");
	shader_->SetConstant(size, "sizeInt");
	auto stage = (int)Stages::INITIALIZE;
 	shader_->SetConstant(stage, "stage");
 	shader_->DispatchCompute(computeSize);

	floodTime = clock();
	int floodPassCount = 25;
	for(int pass = 0; pass < floodPassCount; ++pass)
	{
		int step = pass < 18 ? pass / 2 : 0;
		int stepLength = pow(2, step);

		shader_->SetConstant(stepLength, "stepLength");

		//Flood
		stage = (int)Stages::FLOOD;
		shader_->SetConstant(stage, "stage");
		shader_->DispatchCompute(computeSize);

		//Swap
		stage = (int)Stages::SWAP;
		shader_->SetConstant(stage, "stage");
		shader_->DispatchCompute(computeSize);
	}
	glFinish();
	floodTime = clock() - floodTime;

	//Separate
	stage = (int)Stages::SEPARATE;
	shader_->SetConstant(stage, "stage");
	shader_->DispatchCompute(computeSize);

	buffers_.Get("kernel")->Bind(5);
	stage = (int)Stages::SWAP;
	shader_->SetConstant(stage, "stage");
	unsigned int kernelSpread = kernel.GetSpread();
	shader_->SetConstant(kernelSpread, "kernelSpread");
	unsigned int kernelSide = kernel.GetSide();
	shader_->SetConstant(kernelSide, "kernelSide");

	relaxTime = clock();
	for(unsigned int mode = 0; mode < 2; ++mode)
	{
		shader_->SetConstant(mode, "kernelMode");

		//Relax
		stage = (int)Stages::RELAX;
		shader_->SetConstant(stage, "stage");
		shader_->DispatchCompute(computeSize);

		//Swap
		stage = (int)Stages::SWAP_RESULT;
		shader_->SetConstant(stage, "stage");
		shader_->DispatchCompute(computeSize);
	}

	stage = (int)Stages::FLOOD;
	shader_->SetConstant(stage, "stage");
	shader_->DispatchCompute(computeSize);

	shader_->Unbind();

	glFinish();
	relaxTime = clock() - relaxTime;

	if(isDownload)
	{
		downloadTime = clock();

		buffers_.Get("result")->Download(&result);

		if(finalSeeds != nullptr)
		{
			buffers_.Get("seed")->Download(finalSeeds);
		}

		glFinish();
		downloadTime = clock() - downloadTime;
	}

	std::cout<<"Finished generating Voronoi texture on the GPU of size "<<result.GetWidth()<<"x"<<result.GetHeight()<<" in time "<<clock() - start<<" nanosecs.\n";
	std::cout<<"   Allocation phase took "<<allocateTime<<" nanosecs.\n";
	std::cout<<"   Flood phase took "<<floodTime<<" nanosecs."<<" One pass lasts "<<floodTime / floodPassCount<<" nanosecs.\n";
	std::cout<<"   Relax phase took "<<relaxTime<<" nanosecs."<<" One pass lasts "<<relaxTime / relaxPassCount<<" nanosecs.\n";
	if(isDownload)
		std::cout<<"   Download phase took "<<downloadTime<<" nanosecs.\n";
}
