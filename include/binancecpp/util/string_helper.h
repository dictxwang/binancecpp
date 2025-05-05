#ifndef _UTIL_STRING_HELPER_H_
#define _UTIL_STRING_HELPER_H_

#include <cstring>
#include <string>
#include <sstream>

class strHelper {
public:

    // split the string to array
    template <typename TYPE>
    static int splitStr(TYPE& list,
                 const std::string& str, const char* delim);

    // trim the specify char
    static std::string& trim(std::string& str, const char thechar = ' ');
    // convert type T to string
    template <typename T, typename S> 
    static const T valueOf(const S &a);

    static std::string toLower(std::string& str);
    static std::string toUpper(std::string& str);

};

template <typename TYPE> inline
int strHelper::splitStr(TYPE& list,
                const std::string& str, const char* delim)
{
    if (str.empty())
        return 0;
        
    if (delim == NULL){
        list.push_back(str);
        return 1;
    }

    unsigned int size = strlen(delim);

    std::string::size_type prepos = 0;
    std::string::size_type pos = 0;
    int count = 0;

    for(;;) 
    {
        pos = str.find(delim, pos);
        if (pos == std::string::npos){
            if (prepos < str.size()){
                list.push_back(str.substr(prepos));
                count++;
            }
            break;
        }

        list.push_back(str.substr(prepos, pos-prepos));

        count++;
        pos += size;
        prepos = pos;
    }

    return count;
}

template <typename T, typename S> inline
const T strHelper::valueOf(const S& a)
{
    std::stringstream s;
    T t;
    s << a ; 
    s >> t;
    return t;
}

std::string strHelper::toUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](char& c){
        return std::toupper(c);
    });
	return str;
}
 
std::string strHelper::toLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](char& c){
        return std::tolower(c);
    });
	return str;
}

#endif /* _UTIL_STRING_HELPER_H_ */