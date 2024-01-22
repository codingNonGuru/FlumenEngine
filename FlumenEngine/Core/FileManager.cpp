#include <iostream>
#include <stdio.h>
#include <dirent.h>

#include "FileManager.hpp"
#include "FlumenEngine/Core/File.hpp"

static const auto ROOT_FOLDER = ".";

static const auto MAXIMUM_FILE_COUNT = 16384;

using namespace engine;

FileManager::FileManager()
{
	files_.Initialize(MAXIMUM_FILE_COUNT);

	GetFilesInFolder(ROOT_FOLDER, true);
}

const Array <File> &FileManager::GetFiles()
{
	return &files_;
}

const File *FileManager::GetFile(const char *fileName)
{
	for(auto &file : files_)
	{
		if(file.HasName(fileName) == true)
			return &file;
	}

	return nullptr;
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

