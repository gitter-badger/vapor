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

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include "vapor/range.h"
#include "vapor/parser/helpers.h"

namespace reaver
{
    namespace vapor
    {
        namespace parser { inline namespace _v1
        {
            template<lexer::token_type TokenType>
            struct literal
            {
                class range range;
                lexer::token value;
                boost::optional<lexer::token> suffix;
            };

            using string_literal = literal<lexer::token_type::string>;
            using integer_literal = literal<lexer::token_type::integer>;

            template<lexer::token_type TokenType, typename Context>
            auto parse_literal(Context & ctx)
            {
                literal<TokenType> ret;

                if (peek(ctx, TokenType))
                {
                    ret.value = std::move(expect(ctx, TokenType));

                    auto start = ret.value.range.start();
                    auto end = ret.value.range.end();

                    if (peek(ctx, lexer::suffix(TokenType)))
                    {
                        ret.suffix = std::move(expect(ctx, lexer::suffix(TokenType)));
                        end = ret.suffix->range.end();
                    }

                    ret.range = { start, end };
                }

                return ret;
            }

            template<lexer::token_type TokenType>
            void print(const literal<TokenType> & lit, std::ostream & os, std::size_t indent = 0)
            {
                auto in = std::string(indent, ' ');
                os << in << '`' << lexer::token_types[+lit.value.type] << "` literal value: " << lit.value << '\n';
                if (lit.suffix)
                {
                    os << in << "literal suffix: " << *lit.suffix << '\n';
                }
            }

            void print(const lexer::token & tok, std::ostream & os, std::size_t indent = 0)
            {
                auto in = std::string(indent, ' ');
                os << in << tok << '\n';
            }
        }}
    }
}
