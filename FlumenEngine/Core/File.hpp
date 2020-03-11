#pragma once

#include "FlumenCore/Conventions.hpp"

class File
{
protected:
	FileName name_;

	FilePath path_;

public:
	File() {}

	File(const char*, const char*);

	const char* GetName() {return name_;}

	const char* GetPath() {return path_;}
};
