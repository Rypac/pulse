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

#include <list>

namespace rdl { inline namespace v1 {
    template <typename T>
    using List = std::list<T>;
} }

template <typename T>
inline rdl::List<T> operator+(const rdl::List<T>& list1, const rdl::List<T>& list2) {
    rdl::List<T> out{};
    out.insert(out.end(), list1.begin(), list1.end());
    out.insert(out.end(), list2.begin(), list2.end());
    return out;
}

template <typename T>
inline void operator+=(rdl::List<T>& list1, const rdl::List<T>& list2) {
    list1.insert(list1.end(), list2.begin(), list2.end());
}
