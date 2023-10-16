/*
 * Copyright 2023 iLogtail Authors
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

#include "InputFileAdhoc.h"

namespace logtail {

bool InputFileAdhoc::Init(const Json::Value& config) {
    mAdhocFileManager = AdhocFileManager::GetInstance();
    mJobName = "";
}

bool InputFileAdhoc::Start() {
    GetStaticFileList();
    mAdhocFileManager->AddJob(mJobName, mFilePathList);
}

bool InputFileAdhoc::Stop(bool isPipelineRemoving) {
    if (isPipelineRemoving) {
        mAdhocFileManager->DeleteJob(mJobName);
    }
}

// Init mFilePathList
void InputFileAdhoc::GetStaticFileList() {

    SortFileList();
}

void InputFileAdhoc::SortFileList() {
}

}
