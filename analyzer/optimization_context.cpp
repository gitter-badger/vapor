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

#include "vapor/analyzer/optimization_context.h"
#include "vapor/analyzer/expression.h"
#include "vapor/analyzer/symbol.h"

reaver::vapor::analyzer::_v1::optimization_context::~optimization_context() = default;

void reaver::vapor::analyzer::_v1::optimization_context::_handle_expressions(reaver::vapor::analyzer::_v1::expression * ptr, reaver::future<reaver::vapor::analyzer::_v1::expression *> & fut)
{
    if (_statement_futures.find(ptr) != _statement_futures.end())
    {
        return;
    }

    _statement_futures.emplace(ptr, fut.then([](auto && expr) {
        return static_cast<statement *>(expr);
    }));
}

void reaver::vapor::analyzer::_v1::optimization_context::keep_alive(reaver::vapor::analyzer::_v1::statement * ptr)
{
    std::lock_guard<std::mutex> lock{ _keep_alive_lock };
    auto inserted = _keep_alive_stmt.emplace(ptr).second;
    assert(inserted);
}

void reaver::vapor::analyzer::_v1::optimization_context::keep_alive(reaver::vapor::analyzer::_v1::variable * ptr)
{
    std::lock_guard<std::mutex> lock{ _keep_alive_lock };
    auto inserted = _keep_alive_var.emplace(ptr).second;
    assert(inserted);
}

