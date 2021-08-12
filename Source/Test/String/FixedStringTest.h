/**
 * @file    DateTimeTest.h
 * @author  ggomdyu
 * @since   08/02/2019
 */

#pragma once
#include <cassert>

#include "Text/FixedString.h"
#include "Diagnostics/Debug.h"

#include "../Test.h"

namespace tg
{

class FixedStringTest :
    public Test
{
/**@section Method */
public:
    void Evaluate() override
    {
        {
            FixedString256 a = "abc";
            assert(a.Capacity() == 256);
            assert(a.Length() == 3);

            a += "defabc";
            assert(!strcmp(&a[0], "abcdefabc"));
            assert(a.Length() == 9);
            assert(a[3] == 'd');
            assert(a.IndexOf('a') == 0);
            assert(a.IndexOf('a', 1) == 6);
            assert(a.IndexOf('c') == 2);
            assert(a.IndexOf('a', 0) == 0);
            assert(a.IndexOf('c', 3) == 8);
            assert(a.IndexOfAny([](char ch) { return ch == 'a' || ch == 'b'; }) == 0);
            assert(a.IndexOfAny([](char ch) { return ch == 'a' || ch == 'b'; }, 2) == 6);
            assert(a.LastIndexOf('c') == 8);
            assert(a.LastIndexOf('c', 0) == -1);
            assert(a.LastIndexOf('c', 3) == 2);
            assert(a.LastIndexOf('c', 8) == 8);
            assert(a.LastIndexOfAny([](char ch) { return ch == 'a' || ch == 'b'; }) == 7);
            assert(a.LastIndexOfAny([](char ch) { return ch == 'd' || ch == 'c'; }, 4) == 3);
            assert(a.LastIndexOfAny([](char ch) { return ch == 'a'; }) == 6);
            assert(a.LastIndexOfAny([](char ch) { return ch == 'a'; }, 0) == -1);
            assert(a.LastIndexOfAny([](char ch) { return ch == 'a'; }, 1) == 0);
            assert(a.LastIndexOfAny([](char ch) { return ch == 'f'; }) == 5);

            assert(a == std::string_view("abcdefabc"));
            assert(a != std::string_view("abcdefab"));
            assert(a != std::string_view("abcdefabcg"));
            assert(a == FixedString256("abcdefabc"));
            assert(a != FixedString256("abcdefab"));
            assert(a != FixedString256("abcdefabcg"));

            FixedString256 b = "abc";
            assert(b.Length() == 3);
            b += "234x234";
            assert(b.Length() == 10);
        }
        {
            FixedString8192 fs("HAI");
            fs = "AISIQWR";
            assert(fs.Length() == 7);
            assert(fs.CompareTo("AISIQWR") == 0);
            fs = "WRT@3";
            assert(fs.Length() == 5);
            assert(fs.CompareTo("WRT@3") == 0);
        }
    }

private:

};

}