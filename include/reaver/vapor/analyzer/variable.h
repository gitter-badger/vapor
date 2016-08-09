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

#pragma once

#include <memory>

#include "type.h"

namespace reaver
{
    namespace vapor
    {
        namespace analyzer { inline namespace _v1
        {
            class type;

            class variable
            {
            public:
                virtual std::shared_ptr<type> get_type() const = 0;
            };

            class delayed_variable : public variable
            {
            public:
                delayed_variable(std::shared_ptr<expression> expr) : _type{ std::make_shared<unresolved_type>(std::move(expr)) }
                {
                }

                virtual std::shared_ptr<type> get_type() const override
                {
                    return _type;
                }

            private:
                std::shared_ptr<type> _type;
            };

            inline auto make_delayed_variable(std::shared_ptr<expression> expr)
            {
                return std::make_shared<delayed_variable>(expr);
            }
        }}
    }
}

