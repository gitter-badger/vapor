/**
 * Vapor Compiler Licence
 *
 * Copyright © 2014 Michał "Griwes" Dominiak
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation is required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 **/

#pragma once

#include <reaver/mayfly.h>

#include "vapor/lexer.h"

namespace reaver
{
    namespace vapor
    {
        namespace lexer { inline namespace _v1
        {
            auto test(std::string program, std::vector<token> expected)
            {
                return [program = std::move(program), expected = std::move(expected)]()
                {
                    std::vector<token> generated;
                    std::copy(iterator{ program.begin(), program.end() }, iterator{}, std::back_inserter(generated));

                    MAYFLY_REQUIRE(expected == generated);
                };
            }
        }}
    }
}