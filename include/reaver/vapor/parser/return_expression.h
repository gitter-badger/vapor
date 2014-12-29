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

#include "vapor/range.h"
#include "vapor/parser/expression.h"

namespace reaver
{
    namespace vapor
    {
        namespace parser { inline namespace _v1
        {
            struct return_expression
            {
                class range range;
                expression return_value;
            };

            template<typename Context>
            auto parse_return_expression(Context & ctx)
            {
                return_expression ret;

                auto start = expect(ctx, lexer::token_type::return_).range.start();
                ret.return_value = parse_expression(ctx);

                ret.range = { start, ret.return_value.range.end() };

                return ret;
            }

            void print(const return_expression & ret, std::ostream & os, std::size_t indent = 0)
            {
                auto in = std::string(indent, ' ');

                os << in << "`return-expression` at " << ret.range << '\n';
                os << in << "{\n";
                print(ret.return_value, os, indent + 4);
                os << in << "}\n";
            }
        }}
    }
}