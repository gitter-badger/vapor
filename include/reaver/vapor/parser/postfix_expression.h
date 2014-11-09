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
#include "vapor/parser/expression.h"

namespace reaver
{
    namespace vapor
    {
        namespace parser { inline namespace _v1
        {
            struct expression_list;

            struct postfix_expression
            {
                class range range;
                boost::variant<id_expression, boost::recursive_wrapper<expression_list>> base_expression;
                boost::optional<lexer::token_type> bracket_type;
                boost::optional<boost::recursive_wrapper<expression_list>> argument;
            };

            template<typename Context>
            postfix_expression parse_postfix_expression(Context & ctx)
            {
                auto closing = [](lexer::token_type type)
                {
                    using namespace lexer;
                    switch (type)
                    {
                        case token_type::angle_bracket_open: return token_type::angle_bracket_close;
                        case token_type::round_bracket_open: return token_type::round_bracket_close;
                        case token_type::square_bracket_open: return token_type::square_bracket_close;
                        case token_type::curly_bracket_open: return token_type::curly_bracket_close;
                        default: throw exception(logger::crash) << "invalid opening bracket type";
                    }
                };

                postfix_expression ret;

                position start, end;

                if (peek(ctx, lexer::token_type::round_bracket_open))
                {
                    start = expect(ctx, lexer::token_type::round_bracket_open).range.start();
                    ret.base_expression = parse_expression_list(ctx);
                    end = expect(ctx, lexer::token_type::round_bracket_close).range.end();;
                }

                else
                {
                    ret.base_expression = parse_id_expression(ctx);
                    auto & range = boost::get<id_expression &>(ret.base_expression).range;
                    start = range.start();
                    end = range.end();
                }

                for (auto && type : { lexer::token_type::round_bracket_open, lexer::token_type::square_bracket_open, lexer::token_type::curly_bracket_open,
                    lexer::token_type::angle_bracket_open })
                {
                    if (peek(ctx, type))
                    {
                        ret.bracket_type = type;
                        expect(ctx, type);

                        break;
                    }
                }

                if (ret.bracket_type)
                {
                    if (!peek(ctx, closing(*ret.bracket_type)))
                    {
                        ret.argument = parse_expression_list(ctx);
                    }
                    end = expect(ctx, closing(*ret.bracket_type)).range.end();
                }

                ret.range = { start, end };

                return ret;
            }

            void print(const expression_list & list, std::ostream & os, std::size_t indent);

            void print(const postfix_expression & expr, std::ostream & os, std::size_t indent = 0)
            {
                auto in = std::string(indent, ' ');

                os << in << "`postfix-expression` at " << expr.range << '\n';
                if (expr.bracket_type)
                {
                    os << in << "bracket type: `" << lexer::token_types[+*expr.bracket_type] << "`\n";
                }

                os << in << "{\n";

                visit([&](const auto & value) -> unit { print(value, os, indent + 4); return {}; }, expr.base_expression);
                if (expr.argument)
                {
                    print(expr.argument->get(), os, indent + 4);
                }

                os << in << "}\n";
            }
        }}
    }
}
