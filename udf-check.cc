// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "udf-check.h"

#include <sstream>

using namespace std;

BooleanVal CheckFail(FunctionContext* ctx, const StringVal& msg) {
    stringstream ss;
    ss << "Validating check constraint failed. " << msg.ptr;
    ctx->SetError(ss.str().c_str());
    return BooleanVal(false);
}

BooleanVal CheckFail(FunctionContext* ctx) {
    return CheckFail(ctx, StringVal(""));
}

BooleanVal Check(FunctionContext* ctx, const StringVal& msg, int num_children, const BooleanVal* vals) {
    // Check for failure and append bool values to the message
    bool failed = false;
    stringstream ss;
    ss << msg.ptr << " ( ";
    for (int32_t i = 0; i < num_children; ++i) {
        if (!vals[i].val) {
            failed = true;
        }
        ss << std::boolalpha << vals[i].val << std::noboolalpha << " ";
    }
    ss << ") ";

    if(failed) {
        return CheckFail(ctx, StringVal(ss.str().c_str()));
    } else {
        return BooleanVal(true);
    }
}

BooleanVal Check(FunctionContext* ctx, int num_children, const BooleanVal* vals) {
    return Check(ctx, StringVal(""), num_children, vals);
}