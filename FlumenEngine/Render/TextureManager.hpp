#pragma once

#include "FlumenCore/Conventions.hpp"

namespace render
{
	class Texture;

	class TextureManager
	{
	private:
		static Map <Texture*> textures_;

	public:
		static void LoadTextures();

		static Map <Texture*> & GetTextures();

		static Texture* GetTexture(const char*);

		static void AddTexture(Texture*, const char*);
	};
}