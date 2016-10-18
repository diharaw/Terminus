#include "FileSystem.h"
#include <stdio.h>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

namespace filesystem
{
    std::vector<std::string> m_directory_list;
    std::vector<std::string> m_archive_list;
	FILE* m_CurrentWriteTarget;
    
    bool find_directory(std::string _path)
    {
        {
            for (int i = 0; i < m_directory_list.size(); i++)
            {
                if (m_directory_list[i] == _path)
                    return true;
            }
            
            return false;
        }
    }
    
    bool find_archive(std::string _path)
    {
        {
            for (int i = 0; i < m_archive_list.size(); i++)
            {
                if (m_archive_list[i] == _path)
                    return true;
            }
            
            return false;
        }
    }
    
    void add_directory(std::string _path)
    {
        if (!find_directory(_path))
            m_directory_list.push_back(_path);
    }
    
    void add_archive(std::string _path)
    {
        if (!find_archive(_path))
            m_archive_list.push_back(_path);
    }
    
    FILE * open_file_from_directory(std::string _path)
    {
        const char* name = _path.c_str();
        FILE *f = fopen(name, "rb");
        return f;
    }
    
    void close_file_from_directory(FILE * _file)
    {
        fclose(_file);
    }
    
    FileHandle read_file(std::string _path, bool _text)
    {
        FileHandle file;
        
        char* buffer;
        
#ifdef __APPLE__
        std::string cwd = GetCurrentWorkingDirectory();
#endif
        
        for (int i = 0; i < m_directory_list.size(); i++)
        {
            std::string currentDirectory;
            
#ifdef __APPLE__
            currentDirectory = cwd;
#endif
            currentDirectory += m_directory_list[i] + "/";
            currentDirectory += _path;
            
            FILE* currentFile = open_file_from_directory(currentDirectory);
            
            std::cout << GetCurrentWorkingDirectory() << std::endl;
            
            if (currentFile)
            {
                fseek(currentFile, 0, SEEK_END);
                long fsize = ftell(currentFile);
                fseek(currentFile, 0, SEEK_SET);
                buffer = (char*)malloc(fsize + 1);
                fread(buffer, fsize, 1, currentFile);
                
                if (_text)
                    buffer[fsize] = '\0';
                
                close_file_from_directory(currentFile);
            
                file.buffer = buffer;
                file.size = fsize;
                
                return file;
            }
            
            // If file does not exist, the FILE handle should be NULL, so i should be able to remove the following line.
            close_file_from_directory(currentFile);
        }
        
        return file;
    }


	std::string GetFileExtention(const std::string& _fileName)
	{

		size_t i = _fileName.rfind('.', _fileName.length());
		if (i != std::string::npos)
			return(_fileName.substr(i + 1, _fileName.length() - i));

		return("");
	}

	std::string GetFilename(const std::string& _fileName)
	{
		size_t start = _fileName.find_last_of("/");
		size_t startAlt = _fileName.find_last_of("\\");

		if (start == std::string::npos)
			start = startAlt;

		size_t end = _fileName.find_last_of(".");

		return _fileName.substr(start + 1, (end - start) - 1);
	}

	std::string GetFileNameAndExtention(const std::string& _filePath)
	{
		size_t start = _filePath.find_last_of("/");
		size_t startAlt = _filePath.find_last_of("\\");

		if (start == std::string::npos)
			start = startAlt;

		return _filePath.substr(start + 1, _filePath.length());
	}

    std::string GetCurrentWorkingDirectory()
    {
#ifdef __APPLE__
        char pathbuf[PATH_MAX + 1];
        uint32_t  bufsize = sizeof(pathbuf);
        _NSGetExecutablePath(pathbuf, &bufsize);
        chdir( pathbuf );
        std::string path = std::string(pathbuf);
        
        size_t index = path.find_last_of("/");
        
        return path.substr(0, index + 1);
#else
        std::string s_cwd(getcwd(NULL,0));
        return s_cwd;
#endif
    }

#ifdef WIN32
	size_t GetFileSize(const std::string& _fileName)
	{
		struct stat st;
		if (stat(_fileName.c_str(), &st) != 0) {
			return 0;
		}
		return st.st_size;
	}

	bool DoesDirectoryExist(const std::string& _name)
	{
		struct stat st;
		if (stat(_name.c_str(), &st) == 0)
			return true;
		return false;
	}
#endif
    
	bool WriteBegin(std::string _path)
	{
		const char* path = _path.c_str();
		m_CurrentWriteTarget = fopen(path, "wb");
		if (!m_CurrentWriteTarget)
			return false;

		return true;
	}

	void Write(void* _Buffer, size_t _Size, size_t _Count, long _Offset)
	{
		if (m_CurrentWriteTarget)
		{
			fseek(m_CurrentWriteTarget, _Offset, SEEK_SET);
			fwrite(_Buffer, _Size, _Count, m_CurrentWriteTarget);
		}
	}

	void WriteEnd()
	{
		if (m_CurrentWriteTarget)
		{
			fclose(m_CurrentWriteTarget);
			m_CurrentWriteTarget = NULL;
		}
	}
}
