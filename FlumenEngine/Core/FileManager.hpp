#pragma once

#include "FlumenCore/Conventions.hpp"

class File;

class FileManager
{
private:
	static Array <File> files_;

	static void GetFilesInSubfolder(const char*, bool);

public:
	static void Initialize();

	static Array <File>* GetFilesInFolder(const char*, bool);
};
