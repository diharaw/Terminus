#pragma once

#ifndef STRINGUTILITY_H
#define STRINGUTILITY_H

#include <string>
#include <vector>
#include <sstream>

namespace Terminus
{
    using StringVector = std::vector<std::string>;
    
    namespace StringUtility
    {
        inline int find(std::string _keyword, std::string _source, int _startIndex = -1)
        {
            std::string::size_type n;
            
            if (_startIndex == -1)
                n = _source.find(_keyword);
            else
                n = _source.find(_keyword, _startIndex);
            
            if (n == std::string::npos)
                return -1;
            else
                return n;
        }
        
        inline StringVector find_line(std::string _keyword, std::string _source)
        {
            StringVector lineList;
            std::string line;
            std::istringstream sourceStream(_source);
            
            while (std::getline(sourceStream, line))
            {
                int col = find(_keyword, line);
                
                if (col != -1)
                {
                    lineList.push_back(line);
                }
            }
            
            return lineList;
        }
        
        inline StringVector delimit(std::string _delimiter, std::string _source)
        {
            StringVector list;
            std::string token;
            std::string::size_type n = 0;
            
            while ((n = _source.find(_delimiter)) != std::string::npos)
            {
                token = _source.substr(0, n);
                list.push_back(token);
                _source.erase(0, n + _delimiter.length());
            }
            
            list.push_back(_source);
            
            return list;
        }
        
        inline std::string substring_by_ifdef(std::string _define, std::string _source)
        {
            std::string substring;
            std::string defineString = "#ifdef";
            defineString += " ";
            defineString += _define;
            
            int startIndex = find(defineString, _source);
            int endIndex;
            
            if (startIndex != -1)
            {
                endIndex = find("#endif", _source, startIndex + defineString.length());
                substring = _source.substr(startIndex + defineString.length(), endIndex - (startIndex + defineString.length()));
                
                return substring;
            }
            
            return substring;
        }
        
        inline void replace_substring(std::string _sourceSubstring, std::string _destSubstring, std::string& _source)
        {
            int line = find(_sourceSubstring, _source);
            _source.replace(line, _sourceSubstring.length(), _destSubstring);
        }
        
    };
}

#endif
