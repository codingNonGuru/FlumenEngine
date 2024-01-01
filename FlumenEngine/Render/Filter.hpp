#pragma once

class Shader;
namespace render
{
	class Texture;
}
class Camera;

class Filter
{
private:
	float timer_;

protected:
	enum class Transitions {OPEN, CLOSE, NONE} transition_;

	bool isActive_;

	Shader* shader_;

	render::Texture* screenTexture_;

	virtual void HandleInitialize() {}

	virtual void HandleUpdate(Camera*) {}

	float GetTimeFactor();

public:
	Filter();

	Filter(Shader*, render::Texture*);

	void Initialize(Shader*, render::Texture*);

	void Update(Camera*);

	void Open();

	void Close();
};
