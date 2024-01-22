#pragma once

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Singleton.h"

class File;

namespace engine
{
	class FileManager : public core::Singleton <FileManager>
	{
		friend class core::Singleton <FileManager>;

		Array <File> files_;

		void GetFilesInSubfolder(const char*, bool);

		FileManager();

	public:
		Array <File> *GetFilesInFolder(const char*, bool);

		const Array <File> &GetFiles();

		const File *GetFile(const char *);
	};
}
