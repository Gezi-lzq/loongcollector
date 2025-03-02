/*
 * Copyright 2022 iLogtail Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <exception>
#include <string>

namespace logtail {

class ExceptionBase : public std::exception {
    std::string mMessage;

public:
    ExceptionBase(const std::string& message = "") throw() : mMessage(message) {}

    const char* what() const throw() { return mMessage.c_str(); }

    std::string GetExceptionMessage() const { return mMessage; }

    std::string ToString() const { return mMessage; }
};

} // namespace logtail
