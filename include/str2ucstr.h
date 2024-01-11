/* copyright Louis C Seifert III 2024 all rights reserved
 * MiT license, and I make no promises as the the suitability, please include this notice
 * it would be really great and neato if I am allowed to continue eating and breathing, 
 * which unfortunately means that I need to get paid, which is hard do do when 
 * your unemployed and no one knows your name. 
 * so pass this around use it if you like, if you don't like and want to complain go to
 * your hiring manager and complain with this "man that Louis guy makes things too easy"
 * and show them a copy of this notice! They will know exactly what your talking about.
 * Ok don't you will probably look more insane than I sound. 
 * Other than that, feel free to use it redistribute it, make copoes learn from it whatever.
 * Your still reading this? Wow, impressive. your not like any regular programmer 
 * most of them skip the comments!!! They say that good comments are the best way to make 
 * maintainable code. So I am writing good comments in order to impress, even though
 * this comment exceeds the total length of the source  outside of preprocessor directives.
 */

#ifndef STRING_TO_UNIQUE_CSTR_PNTR
#define STRING_TO_UNIQUE_CSTR_PNTR
#define STRLIM 1024
#include <cstring>
#include <memory>
#include <exception>
namespace ucs
{
class StrLimException : public std::exception
{
	virtual const char* what() const throw()
	{
		return("STRLIM is not #defined and/or was exceeded");
	}
};
/*
 * helper function to change a string into a c_style array and make a copy in a unique pointer
 * defaults to preventing strings over 1024 chars long unless you specify STRLIM as higher
 */
std::unique_ptr<char[]> str2ucstr(std::string input_string)
{
        //todo move to helper functions library?
        if (input_string.length() > STRLIM)
	{
                throw StrLimException();
        }
        //add one char for null terminator
        char* rv=new char[input_string.length()+1];
        std::strcpy(rv,input_string.c_str());
        std::unique_ptr<char[]> retval(rv);
        return(retval);
}
}//namespace
#endif /*STRING TO UNIQUE_STR_PTR*/
