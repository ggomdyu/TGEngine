/**
 * @file    PathTest.h
 * @author  ggomdyu
 * @since   08/02/2019
 */

#pragma once
#include <cassert>

#include "IO/Path.h"
#include "IO/File.h"
#include "Diagnostics/Debug.h"

#include "../Test.h"

namespace tgon
{

class PathTest :
    public Test
{
/**@section Method */
public:
    void Evaluate() override
    {
        assert(Path::GetPathRoot("/Users/chajunho/") == "/");
        assert(Path::GetPathRoot("/") == "/");
        assert(Path::GetPathRoot("/Users") == "/");
        assert(Path::GetPathRoot("") == "");
        assert(Path::GetPathRoot("C:/") == "");
        assert(Path::GetPathRoot("C:") == "");
        assert(Path::GetPathRoot("C://") == "");
        assert(Path::GetPathRoot("C:\\") == "");
        assert(Path::GetPathRoot("\\Users\\chajunho\\") == "");
        assert(Path::GetPathRoot("\\") == "");
        assert(Path::HasExtension("/Users/chajunho/Desktop/1.png") == true);
        assert(Path::HasExtension("/Users/chajunho/Desktop/1") == false);
        assert(Path::HasExtension("/Users/chajunho/Desktop/") == false);
        assert(Path::HasExtension("/Users/chajunho/Desktop") == false);
        assert(Path::HasExtension("1.png") == true);
        assert(Path::HasExtension(".png") == true);
        assert(Path::HasExtension("png") == false);
        assert(Path::GetExtension("/Users/chajunho/Desktop/1.png") == ".png");
        assert(Path::GetExtension("/Users/chajunho/Desktop/1") == "");
        assert(Path::GetExtension("1.png") == ".png");
        assert(Path::GetExtension("1") == "");
        assert(Path::GetExtension("") == "");
        assert(Path::GetFileName("/Users/chajunho/Desktop/1.png") == "1.png");
        assert(Path::GetFileName("") == "");
        assert(Path::GetFileNameWithoutExtension("") == "");
        assert(Path::GetFileNameWithoutExtension("/Users/chajunho/Desktop/123") == "123");
        assert(Path::GetFileNameWithoutExtension("/Users/chajunho/Desktop/1") == "1");
        assert(Path::GetFileNameWithoutExtension("/Users/123123.png") == "123123");
        assert(Path::GetFileNameWithoutExtension("/Users/chajunho/Desktop/1.png") == "1");
        assert(Path::GetFileNameWithoutExtension("/Users/chajunho/Desktop/1.1.png") == "1.1");
        assert(Path::GetFileNameWithoutExtension("1") == "1");
        assert(Path::GetFileNameWithoutExtension("1.png") == "1");
        assert(Path::GetFileNameWithoutExtension("123123") == "123123");
        assert(Path::ChangeExtension("/Users/chajunho/Desktop/1.png", "ext") == "/Users/chajunho/Desktop/1.ext");
        assert(Path::ChangeExtension(".png", "ext") == ".ext");
        assert(Path::ChangeExtension("/Users/chajunho/Desktop", "ext") == "/Users/chajunho/Desktop.ext");
        assert(Path::ChangeExtension("/Users/chajunho/Desktop/", "ext") == "/Users/chajunho/Desktop/.ext");
        assert(Path::ChangeExtension("1.png", "ext") == "1.ext");
        assert(Path::ChangeExtension("png", "ext") == "png.ext");
        assert(Path::ChangeExtension("", "ext") == "");
        assert(Path::ChangeExtension("", "..ext") == "");
        assert(Path::ChangeExtension("", "") == "");
        assert(Path::ChangeExtension("a", ".ext") == "a.ext");
        assert(Path::ChangeExtension("a", "ext") == "a.ext");
        assert(Path::ChangeExtension("a", "..ext") == "a..ext");
#if TGON_PLATFORM_WINDOWS
        assert(Path::Combine("dec", "col") == "dec\\col");
        assert(Path::Combine("dec/", "col") == "dec/col");
        assert(Path::Combine("dec", "col/") == "dec\\col/");
        assert(Path::Combine("dec/", "col/.png") == "dec/col/.png");
        assert(Path::Combine("C:/col/2.png", "C:/col/.png") == "C:/col/.png");
        assert(Path::Combine("col/2.png", "") == "col/2.png");
        assert(Path::Combine("", "col/2.png") == "col/2.png");
        assert(Path::Combine("/User/Sys", "ibs") == "/User/Sys\\ibs");
        assert(Path::Combine("/User/Sys", "/ibs") == "/ibs");
        assert(Path::Combine("/User/Sys/", "/ibs") == "/ibs");
        assert(Path::Combine("", "") == "");
        assert(Path::Combine("/User/Sys", "C:/ibs") == "C:/ibs");
        assert(Path::Combine("C:/User/Sys", "C:/ibs") == "C:/ibs");
        
        assert(Path::GetPathRoot("/Users/chajunho/") == "\\");
        assert(Path::GetPathRoot("/") == "\\");
        assert(Path::GetPathRoot("/Users") == "\\");
        assert(Path::GetPathRoot("") == "");
        assert(Path::GetPathRoot("C:/") == "C:\\");
        assert(Path::GetPathRoot("C:") == "C:");
        assert(Path::GetPathRoot("C://") == "C:\\");
        assert(Path::GetPathRoot("C:\\") == "C:\\");
        assert(Path::GetPathRoot("\\Users\\chajunho\\") == "\\");
        assert(Path::GetPathRoot("\\") == "\\");
#elif TGON_PLATFORM_MACOS
        assert(Path::Combine("dec", "col") == "dec/col");
        assert(Path::Combine("dec/", "col") == "dec/col");
        assert(Path::Combine("dec", "col/") == "dec/col/");
        assert(Path::Combine("dec/", "col/.png") == "dec/col/.png");
        assert(Path::Combine("col/2.png", "") == "col/2.png");
        assert(Path::Combine("", "col/2.png") == "col/2.png");
        assert(Path::Combine("/User/Sys", "ibs") == "/User/Sys/ibs");
        assert(Path::Combine("/User/Sys", "/ibs") == "/ibs");
        assert(Path::Combine("/User/Sys/", "/ibs") == "/ibs");
        assert(Path::Combine("", "") == "");
        assert(Path::Combine("C:/col/2.png", "C:/col/.png") == "C:/col/2.png/C:/col/.png");
        assert(Path::Combine("/User/Sys", "C:/ibs") == "/User/Sys/C:/ibs");
        assert(Path::Combine("C:/User/Sys", "C:/ibs") == "C:/User/Sys/C:/ibs");
        
        assert(Path::GetPathRoot("/Users/chajunho/") == "/");
        assert(Path::GetPathRoot("/") == "/");
        assert(Path::GetPathRoot("/Users") == "/");
        assert(Path::GetPathRoot("") == "");
        assert(Path::GetPathRoot("C:/") == "");
        assert(Path::GetPathRoot("C:") == "");
        assert(Path::GetPathRoot("C://") == "");
        assert(Path::GetPathRoot("C:\\") == "");
        assert(Path::GetPathRoot("\\Users\\chajunho\\") == "");
        assert(Path::GetPathRoot("\\") == "");
#endif
        assert(Path::GetDirectoryName("C:/1.png") == "C:");
        assert(Path::GetDirectoryName("C:\\1.png") == "");
        assert(Path::GetDirectoryName("C:/Dir/1.png") == "C:/Dir");
        assert(Path::GetDirectoryName("/Users/chajunho/Desktop/1.png") == "/Users/chajunho/Desktop");
        assert(Path::GetDirectoryName("") == "");
        assert(Path::GetDirectoryName("abc.png") == "");
        assert(Path::GetDirectoryName("abc") == "");
        assert(Path::GetFullPath("") == "");
        assert(Path::GetFullPath("/") == "/");
    }

private:

};

} /* namespace tgon */
