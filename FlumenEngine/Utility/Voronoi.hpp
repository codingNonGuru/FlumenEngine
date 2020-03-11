#pragma once

#include "FlumenCore/Conventions.hpp"

class Shader;
class DataBuffer;

class Voronoi
{
private:
	static Shader* shader_;

	static Map <DataBuffer> buffers_;

public:
	static void Initialize();

	static void Destroy();

	static void Generate(bool, Grid<float> &, Grid<unsigned int> *, unsigned int);

	static void LoadSeeds(Grid<unsigned int> &);

	static void LoadRandomTriangulation(int32_t, int32_t, uint32_t &);
};
