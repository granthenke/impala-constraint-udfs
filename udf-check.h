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

#ifndef CHECK_UDF_H
#define CHECK_UDF_H

#include <impala_udf/udf.h>

using namespace impala_udf;

BooleanVal CheckFail(FunctionContext* ctx, const StringVal& msg);

BooleanVal CheckFail(FunctionContext* ctx);

BooleanVal Check(FunctionContext* ctx, const StringVal& msg, int num_children, const BooleanVal* vals);

BooleanVal Check(FunctionContext* ctx, int num_children, const BooleanVal* vals);

#endif
