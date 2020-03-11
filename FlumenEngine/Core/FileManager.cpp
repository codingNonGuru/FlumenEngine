#include <iostream>
#include <stdio.h>
#include <dirent.h>

#include "FileManager.hpp"
#include "FlumenEngine/Core/File.hpp"

Array <File> FileManager::files_ = Array <File>();

void FileManager::Initialize()
{
	files_.Initialize(16384);
}

Array <File>* FileManager::GetFilesInFolder(const char* path, bool isRecursive)
{
	files_.Reset();

	GetFilesInSubfolder(path, isRecursive);

	return &files_;
}

void FileManager::GetFilesInSubfolder(const char* path, bool isRecursive)
{
#ifdef LINUX

	DIR *directory;
	struct dirent *file;

	directory = opendir(path);
	if (directory != nullptr)
	{
		while (file = readdir(directory))
		{
			FilePath filePath(path);
			filePath.Add("/");
			filePath.Add(file->d_name);

			if(file->d_type != DT_DIR)
			{
				*files_.Allocate() = File(file->d_name, filePath);
			}

			if(!isRecursive)
				continue;

			if(file->d_type == DT_DIR && file->d_name[0] != '.')
			{
				GetFilesInSubfolder(filePath, true);
			}
		}
	}
	closedir(directory);
#endif
}

