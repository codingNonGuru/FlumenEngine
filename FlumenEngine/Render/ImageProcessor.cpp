#include "ImageProcessor.hpp"
#include "FlumenEngine/Render/Shader.hpp"
#include "FlumenEngine/Render/ShaderManager.hpp"
#include "FlumenEngine/Render/DataBuffer.hpp"
#include "FlumenEngine/Utility/Kernel.hpp"

Size capacity = Size(2048, 2048);

DataBuffer* outputBuffer = nullptr;

DataBuffer* blurSwapBuffer = nullptr;

DataBuffer* blurFilterBuffer = nullptr;

Kernel* blurKernel = nullptr;

bool isBlurModuleInitialized = false;

DataBuffer* ImageProcessor::Blur(DataBuffer* inputBuffer, Size size, int kernelWidth)
{
	if(isBlurModuleInitialized == false)
	{
		InitializeBlurModule();
	}

	blurKernel = new Kernel(kernelWidth);
	float strength = (float)kernelWidth * (float)kernelWidth / 2.0f;
	blurKernel->Initialize(KernelTypes::GAUSS, strength);
	auto weights = blurKernel->GetWeights();

	blurFilterBuffer->UploadData(weights.GetStart(), weights.GetMemoryCapacity());

	auto shader = ShaderManager::GetShader("Blur");
	if(shader == nullptr)
		return nullptr;

	shader->Bind();

	inputBuffer->Bind(0);
	blurSwapBuffer->Bind(1);
	outputBuffer->Bind(2);
	blurFilterBuffer->Bind(3);

	shader->SetConstant(size, "size");
	shader->SetConstant(kernelWidth, "filterSize");
	shader->SetConstant(0, "hasBorder");

	shader->SetConstant(0, "pass");
	shader->DispatchCompute(size / 16);

	shader->SetConstant(1, "pass");
	shader->DispatchCompute(size / 16);

	shader->Unbind();

	return outputBuffer;
}

DataBuffer* ImageProcessor::GetOutputBuffer()
{
	return outputBuffer;
}

void ImageProcessor::InitializeBlurModule()
{
	outputBuffer = new DataBuffer(capacity.x * capacity.y * 4, nullptr);

	blurSwapBuffer = new DataBuffer(capacity.x * capacity.y * 4, nullptr);

	blurFilterBuffer = new DataBuffer(4096, nullptr);

	isBlurModuleInitialized = true;
}
