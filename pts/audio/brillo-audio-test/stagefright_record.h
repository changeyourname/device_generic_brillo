//
// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef EXAMPLE_APP_STAGEFRIGHT_RECORD_H_
#define EXAMPLE_APP_STAGEFRIGHT_RECORD_H_

#include <utils/Errors.h>

namespace android {

// Record audio using stagefright and save it to a file.
status_t LibstagefrightRecordAudio(const char* filename);

}  // namespace android

#endif  // EXAMPLE_APP_STAGEFRIGHT_RECORD_H_
