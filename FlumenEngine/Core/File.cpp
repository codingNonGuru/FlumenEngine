#include "FlumenEngine/Core/File.hpp"

File::File(const char* name, const char* path)
{
	name_.Add(name);
	path_.Add(path);
}
