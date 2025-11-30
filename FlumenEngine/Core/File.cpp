#include <fstream>
#include <iostream>

#include "FlumenEngine/Core/File.hpp"

File::File(const char* name, const char* path)
{
	name_.Add(name);
	path_.Add(path);
}

bool File::HasName(FileName name) const
{
	return name_ == name;
}

const char *currentFileContent = nullptr;

const char *File::ReadContent() const
{
	std::ifstream fileStream(path_);

	static std::string fileContent;
	fileContent.clear();

	std::string line;

	while(std::getline(fileStream, line))
	{
		fileContent += line + "\n";
	}

	fileStream.close();

	currentFileContent = fileContent.c_str();

	return currentFileContent;
}