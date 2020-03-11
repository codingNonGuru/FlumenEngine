#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Delegate/Delegate.hpp"
#include "FlumenEngine/Utility/Color.hpp"

class Camera;
class Window;
class FrameBuffer;

class RenderManager
{
private:
	static Window* window_;

	static Map <Camera*> cameras_;

	static Color backgroundColor_;

	static FrameBuffer* defaultFrameBuffer_;

	static Delegate* onInitialize_;

public:
	static void Initialize();

	static void Update();

	static void UpdateCameras();

	static void EnableDepthTesting();

	static void DisableDepthTesting();

	static void SetBlendMode();

	static void DisableBlending();

	static Camera* GetCamera(Word);

	static void AddCamera(Word, Camera*);

	static Camera* GetInterfaceCamera();

	static void SetBackgroundColor(Color);

	static Color GetBackgroundColor();

	static void SetDefaultFrameBuffer(FrameBuffer*);

	static void ClearDefaultBuffer();

	static Delegate* OnInitialize();
};
