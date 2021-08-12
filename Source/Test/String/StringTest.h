#pragma once
#include <cassert>

#include "Text/String.h"
#include "Diagnostics/Debug.h"

#include "../Test.h"

namespace tg
{

class StringTest :
    public Test
{
/**@section Method */
public:
    void Evaluate() override
    {
        {
            char arr[] = {'a', 'b', 'e', 'd'};
            String s(arr);
            assert(!strcmp(&s[0], arr));
            assert(s.Length() == 4);
            assert(s[0] == 'a');
            assert(s[3] == 'd');
            assert(String::IsNullOrEmpty(s) == false);
        }
        {
            String s(c, 5);
            assert(s.Length() == 5);
            assert(!strcmp(&s[0], "ccccc"));
            assert(s[0] == 'c');
            assert(s[3] == 'c');
        }
    }

private:

};

}