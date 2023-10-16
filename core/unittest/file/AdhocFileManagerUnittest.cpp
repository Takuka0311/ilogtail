// Copyright 2023 iLogtail Authors
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

#include "file/AdhocFileManager.h"
#include "unittest/Unittest.h"
#include "AppConfig.h"

namespace logtail {

std::string kTestRootDir;

class AdhocFileManagerUnittest : public ::testing::Test {
public:
    static void SetUpTestCase() {
        kTestRootDir = (bfs::path(GetProcessExecutionDir()) / "AdhocFileManagerUnittest").string();
        bfs::remove_all(kTestRootDir);
        bfs::create_directories(kTestRootDir);
        AppConfig::GetInstance()->SetLogtailSysConfDir(kTestRootDir);
    }

    static void TearDownTestCase() { bfs::remove_all(kTestRootDir); }

    void TestAdhocFileManagerUnittest();

private:
};

UNIT_TEST_CASE(AdhocFileManagerUnittest, TestAdhocFileManagerUnittest);

void AdhocFileManagerUnittest::TestAdhocFileManagerUnittest() {

}

} // namespace logtail

UNIT_TEST_MAIN