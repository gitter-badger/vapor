/**
 * Vapor Compiler Licence
 *
 * Copyright © 2016 Michał "Griwes" Dominiak
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
#include "vapor/analyzer/function.h"
#include "vapor/analyzer/block.h"
#include "vapor/analyzer/return.h"
#include "vapor/analyzer/symbol.h"

reaver::future<> reaver::vapor::analyzer::_v1::function::simplify(reaver::vapor::analyzer::_v1::optimization_context & ctx)
{
    if (_body)
    {
        return _body->simplify(ctx)
            .then([&](auto && simplified) {
                _body = dynamic_cast<block *>(simplified);
            });
    }

    return make_ready_future();
}

reaver::future<reaver::vapor::analyzer::_v1::expression *> reaver::vapor::analyzer::_v1::function::simplify(reaver::vapor::analyzer::_v1::optimization_context & ctx, std::vector<variable *> args)
{
    if (_body)
    {
        auto returns = _body->get_returns();

        assert(_body->has_return_expression() || returns.size());
        auto var = _body->has_return_expression() ? _body->get_return_expression()->get_variable() : returns.front()->get_returned_variable();
        auto begin = _body->has_return_expression() ? returns.begin() : returns.begin() + 1;

        if (!var->is_constant())
        {
            return make_ready_future<expression *>(nullptr);
        }

        if (std::all_of(begin, returns.end(), [](auto && ret) { return ret->get_returned_variable()->is_constant(); })
            && std::all_of(begin, returns.end(), [&](auto && ret) { return ret->get_returned_variable()->is_equal(var); }))
        {
            return make_ready_future(make_variable_ref_expression(var).release());
        }

        return make_ready_future<expression *>(nullptr);
    }

    if (_compile_time_eval)
    {
        return make_ready_future((*_compile_time_eval)(ctx, args));
    }

    assert(0);
}

