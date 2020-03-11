#pragma once

class Shader;
class Texture;
class Camera;

class Filter
{
private:
	float timer_;

protected:
	enum class Transitions {OPEN, CLOSE, NONE} transition_;

	bool isActive_;

	Shader* shader_;

	Texture* screenTexture_;

	virtual void HandleInitialize() {}

	virtual void HandleUpdate(Camera*) {}

	float GetTimeFactor();

public:
	Filter();

	Filter(Shader*, Texture*);

	void Initialize(Shader*, Texture*);

	void Update(Camera*);

	void Open();

	void Close();
};
