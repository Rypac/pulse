/******************************************************************************
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Ryan Davis <ryan.davis.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 ******************************************************************************/

#pragma once

#include <vector>

namespace rdl { inline namespace v1 {
    template <typename T>
    using Vector = std::vector<T>;
} }

template <typename T>
inline rdl::Vector<T> operator+(const rdl::Vector<T>& v1, const rdl::Vector<T>& v2) {
    rdl::Vector<T> out{};
    out.reserve(v1.size() + v2.size());
    out.insert(out.end(), v1.begin(), v1.end());
    out.insert(out.end(), v2.begin(), v2.end());
    return out;
}

template <typename T>
inline void operator+=(rdl::Vector<T>& v1, const rdl::Vector<T>& v2) {
    v1.reserve(v1.size() + v2.size());
    v1.insert(v1.end(), v2.begin(), v2.end());
}
