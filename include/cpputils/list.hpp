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

template <typename T>
inline std::list<T> operator+(const std::list<T>& list1, const std::list<T>& list2) {
    std::list<T> out(list1);
    out.insert(out.end(), list2.begin(), list2.end());
    return out;
}

template <typename T>
inline void operator+=(std::list<T>& list1, const std::list<T>& list2) {
    list1.insert(list1.end(), list2.begin(), list2.end());
}

template <typename T>
inline std::list<T> operator<<(const std::list<T>& list, const T& value) {
    std::list<T> out(list);
    out.emplace_back(value);
    return out;
}
