/*
 * Copyright 2023 iLogtail Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdint>

#include "collection_pipeline/plugin/interface/Input.h"
#include "container_manager/ContainerDiscoveryOptions.h"
#include "file_server/FileDiscoveryOptions.h"
#include "file_server/FileTagOptions.h"
#include "file_server/MultilineOptions.h"
#include "file_server/reader/FileReaderOptions.h"
#include "monitor/metric_models/ReentrantMetricsRecord.h"

namespace logtail {

class InputFile : public Input {
public:
    static const std::string sName;

    static bool DeduceAndSetContainerBaseDir(ContainerInfo& containerInfo,
                                             const CollectionPipelineContext*,
                                             const FileDiscoveryOptions*);

    InputFile();

    const std::string& Name() const override { return sName; }
    bool Init(const Json::Value& config, Json::Value& optionalGoPipeline) override;
    bool Start() override;
    bool Stop(bool isPipelineRemoving) override;
    bool SupportAck() const override { return true; }

    FileDiscoveryOptions mFileDiscovery;
    bool mEnableContainerDiscovery = false;
    ContainerDiscoveryOptions mContainerDiscovery;
    FileReaderOptions mFileReader;
    MultilineOptions mMultiline;
    FileTagOptions mFileTag;
    PluginMetricManagerPtr mPluginMetricManager;
    IntGaugePtr mMonitorFileTotal;
    // others
    uint32_t mMaxCheckpointDirSearchDepth = 0;
    uint32_t mExactlyOnceConcurrency = 0;

private:
    bool CreateInnerProcessors();
    static bool SetContainerBaseDir(ContainerInfo& containerInfo, const std::string& logPath);
    static std::string GetLogPath(const FileDiscoveryOptions* fileDiscovery);

#ifdef APSARA_UNIT_TEST_MAIN
    friend class InputFileUnittest;
#endif
};

} // namespace logtail
