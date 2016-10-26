#pragma once

#ifndef FileSystem_h
#define FileSystem_h

#include <string>

struct FileHandle
{
    char* buffer;
    size_t size;
};

namespace FileSystem
{
    extern FileHandle ReadFile(std::string _path, bool _text = false);
    extern void AddDirectory(std::string _path);
    extern void AddArchive(std::string _path);
	extern std::string GetFileExtention(const std::string& _fileName);
	extern std::string GetFilename(const std::string& _fileName);
	extern std::string GetFileNameAndExtention(const std::string& _filePath);
	extern size_t GetFileSize(const std::string& _fileName);
	extern bool DoesDirectoryExist(const std::string& _name);
    extern std::string GetCurrentWorkingDirectory();
	extern bool WriteBegin(std::string _path);
	extern void Write(void* _Buffer, size_t _Size, size_t _Count, long _Offset);
	extern void WriteEnd();
}

#endif
