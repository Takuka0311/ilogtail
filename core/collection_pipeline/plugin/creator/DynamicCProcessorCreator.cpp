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

#include "collection_pipeline/plugin/creator/DynamicCProcessorCreator.h"

#include "collection_pipeline/plugin/instance/ProcessorInstance.h"
#include "common/DynamicLibHelper.h"
#include "plugin/processor/DynamicCProcessorProxy.h"

namespace logtail {

DynamicCProcessorCreator::DynamicCProcessorCreator(const processor_interface_t* plugin, void* handle)
    : mPlugin(plugin), _handle(handle) {
}

DynamicCProcessorCreator::~DynamicCProcessorCreator() {
    if (_handle) {
        DynamicLibLoader::CloseLib(_handle);
    }
}

std::unique_ptr<PluginInstance> DynamicCProcessorCreator::Create(const PluginInstance::PluginMeta& pluginMeta) {
    DynamicCProcessorProxy* plugin = new DynamicCProcessorProxy(mPlugin->name);
    plugin->SetCProcessor(mPlugin);
    return std::make_unique<ProcessorInstance>(plugin, pluginMeta);
}

} // namespace logtail
