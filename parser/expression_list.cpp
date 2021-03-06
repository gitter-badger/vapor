/**
 * Vapor Compiler Licence
 *
 * Copyright © 2015 Michał "Griwes" Dominiak
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

#include "vapor/parser/expression_list.h"
#include "vapor/parser/lambda_expression.h"

reaver::vapor::parser::_v1::expression_list reaver::vapor::parser::_v1::parse_expression_list(reaver::vapor::parser::_v1::context & ctx)
{
    expression_list ret;

    // TODO: verify that this actually works as intended
    // the way I originally used the operator stack can't possibly work when the expression context changes
    // and I failed to notice that originally

    auto stack = std::move(ctx.operator_stack);
    ctx.operator_stack.clear();

    ret.expressions.push_back(parse_expression(ctx));

    if (peek(ctx, lexer::token_type::comma))
    {
        expect(ctx, lexer::token_type::comma);
        ret.expressions.push_back(parse_expression(ctx));
    }

    ret.range = { ret.expressions.front().range.start(), ret.expressions.back().range.end() };

    assert(ctx.operator_stack.empty());
    ctx.operator_stack = std::move(stack);

    return ret;
}

void reaver::vapor::parser::_v1::print(const reaver::vapor::parser::_v1::expression_list & list, std::ostream & os, std::size_t indent)
{
    auto in = std::string(indent, ' ');

    os << in << "`expression-list` at " << list.range << '\n';

    for (auto && expression : list.expressions)
    {
        os << in << "{\n";
        print(expression, os, indent + 4);
        os << in << "}\n";
    }
}
