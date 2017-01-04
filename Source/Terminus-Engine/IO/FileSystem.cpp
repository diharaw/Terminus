#include "FileSystem.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>

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
    
    FILE * open_file_from_directory(std::string _path, bool _text)
    {
        const char* name = _path.c_str();

#ifdef __APPLE__
        FILE *f;
        
        if(_text)
            f = fopen(name, "r");
        else
            f = fopen(name, "rb");
#else
		FILE *f = fopen(name, "rb");
#endif
        return f;
    }
    
    void close_file_from_directory(FILE * _file)
    {
        fclose(_file);
    }
    
    FileHandle read_file(std::string _path, bool _text, bool _absolute)
    {
        FileHandle file;
        
        char* buffer;

		file.buffer = nullptr;
		file.size = 0;
        
#ifdef __APPLE__
        std::string cwd = get_current_working_directory();
#endif
        
        for (int i = 0; i < m_directory_list.size(); i++)
        {
            std::string currentDirectory;
      
			if (!_absolute)
			{
#ifdef __APPLE__
				currentDirectory = cwd;
#endif
				currentDirectory += m_directory_list[i] + "/";
				currentDirectory += _path;
			}
			else
			{
				currentDirectory = _path;
			}
            
            FILE* currentFile = open_file_from_directory(currentDirectory, _text);
            
            std::cout << get_current_working_directory() << std::endl;
            
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
            //close_file_from_directory(currentFile);
        }
        
        return file;
    }


	std::string get_file_extention(const std::string& _fileName)
	{

		size_t i = _fileName.rfind('.', _fileName.length());
		if (i != std::string::npos)
			return(_fileName.substr(i + 1, _fileName.length() - i));

		return("");
	}

	std::string get_filename(const std::string& _fileName)
	{
		size_t start = _fileName.find_last_of("/");
		size_t startAlt = _fileName.find_last_of("\\");

		if (start == std::string::npos)
			start = startAlt;

		size_t end = _fileName.find_last_of(".");

		return _fileName.substr(start + 1, (end - start) - 1);
	}

	std::string get_file_name_and_extention(const std::string& _filePath)
	{
		size_t start = _filePath.find_last_of("/");
		size_t startAlt = _filePath.find_last_of("\\");

		if (start == std::string::npos)
			start = startAlt;

		return _filePath.substr(start + 1, _filePath.length());
	}

	std::string get_file_path(const std::string& _filePath)
	{
		size_t start = _filePath.find_last_of("/");
		size_t startAlt = _filePath.find_last_of("\\");

		if (start == std::string::npos)
			start = startAlt;

		return _filePath.substr(0, start + 1);
	}

    std::string get_current_working_directory()
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
	size_t get_file_size(const std::string& _fileName)
	{
		struct stat st;
		if (stat(_fileName.c_str(), &st) != 0) {
			return 0;
		}
		return st.st_size;
	}

	bool does_directory_exist(const std::string& _name)
	{
		struct stat st;
		if (stat(_name.c_str(), &st) == 0)
			return true;
		return false;
	}
#endif
    
	bool write_begin(std::string _path)
	{
		const char* path = _path.c_str();
		m_CurrentWriteTarget = fopen(path, "wb");
		if (!m_CurrentWriteTarget)
			return false;

		return true;
	}

	void write(void* _Buffer, size_t _Size, size_t _Count, long _Offset)
	{
		if (m_CurrentWriteTarget)
		{
			fseek(m_CurrentWriteTarget, _Offset, SEEK_SET);
			fwrite(_Buffer, _Size, _Count, m_CurrentWriteTarget);
		}
	}

	void write_end()
	{
		if (m_CurrentWriteTarget)
		{
			fclose(m_CurrentWriteTarget);
			m_CurrentWriteTarget = NULL;
		}
	}

	void copy_file(std::string input, std::string output)
	{
		std::ifstream source(input, std::ios::binary);
		std::ofstream dest(output, std::ios::binary);

		std::istreambuf_iterator<char> begin_source(source);
		std::istreambuf_iterator<char> end_source;
		std::ostreambuf_iterator<char> begin_dest(dest);
		std::copy(begin_source, end_source, begin_dest);

		source.close();
		dest.close();
	}
}
