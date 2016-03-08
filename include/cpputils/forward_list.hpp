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

#include <forward_list>

template <typename T>
inline std::forward_list<T> operator+(const std::forward_list<T>& list1, const std::forward_list<T>& list2) {
    std::forward_list<T> out1(list1);
    std::forward_list<T> out2(list2);
    out2.splice_after(out2.cbefore_begin(), out1);
    return out2;
}

template <typename T>
inline void operator+=(std::forward_list<T>& list1, const std::forward_list<T>& list2) {
    std::forward_list<T> new_list(list2);
    new_list.splice_after(new_list.cbefore_begin(), list1);
    list1.swap(new_list);
}

template <typename T>
inline std::forward_list<T> operator<<(const std::forward_list<T>& list, const T& value) {
    return list + std::forward_list<T>{value};
}
