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
    /**
     * Reads file from directory or zip file, depending on selected type.
     * @param _path Path to the file
     * @param _text Text flag. Set to true when reading text to add null terminator.
     * @return FileHandle structure.
     */
    extern FileHandle ReadFile(std::string _path, bool _text = false);
    /**
     * Add a directory containing Assets.
     * @param _path Path of directory.
     */
    extern void AddDirectory(std::string _path);
    /**
     * Add file path of a Zip archive containing Assets.
     * @param _path Path of Archive.
     */
    extern void AddArchive(std::string _path);
    /**
     * Get file extension from a path.
     * @param _fileName path of the file.
     * @return string File extension.
     */
	extern std::string GetFileExtention(const std::string& _fileName);
    /**
     * Get the file name from a path.
     * @param _fileName path of the file.
     * @return string File name.
     */
	extern std::string GetFilename(const std::string& _fileName);
    /**
     * Get the file name and extension from a path.
     * @param _fileName path of the file.
     * @return string File name with extension.
     */
	extern std::string GetFileNameAndExtention(const std::string& _filePath);
    /**
     * Get the size of a given file.
     * @param _fileName path of the file.
     * @return size_t File size.
     */
	extern size_t GetFileSize(const std::string& _fileName);
    /**
     * Checks if a given directory exists.
     * @param _name Name of directory.
     * @return bool returns true if exists.
     */
	extern bool DoesDirectoryExist(const std::string& _name);
    /**
     * Returns current working directory.
     * @return string Current working directory.
     */
    extern std::string GetCurrentWorkingDirectory();
    /**
     * Initializes a file write to the given path.
     * @param _path Path to be written to.
     * @return bool Returns true if path is valid.
     */
	extern bool WriteBegin(std::string _path);
    /**
     * Writes contents of buffer into the path given in WriteBegin.
     * @param _Buffer void* containing data to be written.
     * @param _Size Size of data
     * @param _Count Count of data instances to be written.
     * @param _Offset Offset from the beginning of the file.
     */
	extern void Write(void* _Buffer, size_t _Size, size_t _Count, long _Offset);
    /**
     * Finishes file write by closing file. Must be called after at least WriteBegin.
     */
	extern void WriteEnd();
}

#endif
