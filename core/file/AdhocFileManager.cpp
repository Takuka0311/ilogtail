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

#include "AdhocFileManager.h"
#include "logger/Logger.h"
#include "common/Thread.h"
#include "config_manager/ConfigManager.h"
#include "common/FileSystemUtil.h"
#include "processor/daemon/LogProcess.h"

namespace logtail {

AdhocFileManager::AdhocFileManager() {
    mRunFlag = false;
}

void AdhocFileManager::Run() {
    if (mRunFlag) {
       return;
    } else {
        mRunFlag = true;
    }

    new Thread([this]() { ProcessLoop(); });
    LOG_INFO(sLogger, ("AdhocFileManager", "Start"));
}

void AdhocFileManager::ProcessLoop() {
    mAdhocCheckpointManager = AdhocCheckpointManager::GetInstance();

    while (mRunFlag) {
        AdhocEvent* ev = PopEventQueue();
        if (NULL == ev) {
            LOG_INFO(sLogger, ("AdhocFileManager", "All file loaded, exit"));
            mRunFlag = false;
            break;
        }
        switch (ev->mType) {
            case EVENT_START_JOB:
                ProcessStartJobEvent(ev);
                break;
            case EVENT_READ_FILE:
                ProcessReadFileEvent(ev);
                break;
            case EVENT_STOP_JOB:
                ProcessStopJobEvent(ev);
                break;
        }

        mAdhocCheckpointManager->DumpAdhocCheckpoint();
    }
    LOG_INFO(sLogger, ("AdhocFileManager", "Stop"));
}

void AdhocFileManager::ProcessStartJobEvent(AdhocEvent* ev) {
    AdhocJobCheckpointPtr jobCheckpoint
        = mAdhocCheckpointManager->CreateAdhocJobCheckpoint(ev->mJobName, ev->mFilePathList);
    if (nullptr != jobCheckpoint) {
        // push first file to queue
        AdhocEvent* newEv = new AdhocEvent(EVENT_READ_FILE, ev->mJobName, 0);
        PushEventQueue(newEv);
    }
}

void AdhocFileManager::ProcessReadFileEvent(AdhocEvent* ev) {
    std::string jobName = ev->mJobName;
    AdhocFileCheckpointPtr fileCheckpoint = mAdhocCheckpointManager->GetAdhocFileCheckpoint(jobName);
    // if job doesn't exist or is finished
    if (fileCheckpoint == nullptr) {
        return;
    }

    AdhocFileReaderKey fileReaderKey = AdhocFileReaderKey(
        jobName, fileCheckpoint->mDevInode, fileCheckpoint->mSignatureSize, fileCheckpoint->mSignatureHash);
    std::string fileReaderKeyString = fileReaderKey.ToString();
    // if reader doesn't exist, create it
    if (mAdhocFileReaderMap.find(fileReaderKeyString) == mAdhocFileReaderMap.end()) {
        std::string dirPath = ParentPath(fileCheckpoint->mRealFileName);
        std::string fileName = fileCheckpoint->mRealFileName.substr(dirPath.length());
        LogFileReaderPtr readerSharePtr(
            GetJobConfig(jobName)->CreateLogFileReader(dirPath, fileName, fileCheckpoint->mDevInode, true));
        mAdhocFileReaderMap[fileReaderKeyString] = readerSharePtr;
    }
    LogFileReaderPtr fileReader = mAdhocFileReaderMap[fileReaderKeyString];

    // Return if process queue busy
    if (!LogProcess::GetInstance()->IsValidToReadAdhocLog(fileReader->GetLogstoreKey())) {
        // Log warning and send alarm per 10s( >10s if multi jobs exist)
        if (0 == (ev->mWaitTimes % 1000)) {
            LOG_WARNING(sLogger,
                        ("read adhoc file failed, logprocess queue is full, put adhoc event to event queue again",
                         fileReader->GetHostLogPath())(fileReader->GetProjectName(), fileReader->GetCategory()));
            LogtailAlarm::GetInstance()->SendAlarm(
                PROCESS_QUEUE_BUSY_ALARM,
                std::string(
                    "read adhoc file failed, logprocess queue is full, put adhoc event to event queue again, file:")
                    + fileReader->GetHostLogPath() + " ,project:" + fileReader->GetProjectName()
                    + " ,logstore:" + fileReader->GetCategory());
        }

        ev->mWaitTimes++;
        PushEventQueue(ev);
        usleep(1000 * 10);
        return;
    }

    // read file
    LogBuffer* logBuffer = new LogBuffer;
    fileReader->ReadLog(*logBuffer, nullptr);
    if (!logBuffer->rawBuffer.empty()) {
        logBuffer->logFileReader = fileReader;
        LogProcess::GetInstance()->PushBuffer(logBuffer, 100000000);
        fileCheckpoint->mOffset = fileReader->GetLastFilePos();
        fileCheckpoint->mRealFileName = fileReader->GetHostLogPathFile();
    } else {
        delete logBuffer;
    }
    mAdhocCheckpointManager->UpdateAdhocFileCheckpoint(jobName, fileCheckpoint);

    PushEventQueue(ev);
}

void AdhocFileManager::ProcessStopJobEvent(AdhocEvent* ev) {
    mDeletedJobSet.insert(ev->mJobName);
    mAdhocCheckpointManager->DeleteAdhocJobCheckpoint(ev->mJobName);
}

void AdhocFileManager::PushEventQueue(AdhocEvent* ev) {
    mEventQueue.PushItem(ev);
}

AdhocEvent* AdhocFileManager::PopEventQueue() {
    while (mEventQueue.GetItemNumber() > 0) {
        AdhocEvent* ev;
        mEventQueue.PopItem(ev);

        if (mDeletedJobSet.find(ev->mJobName) != mDeletedJobSet.end()) {
            continue;
        }
        return ev;
    }
    return NULL;
}

void AdhocFileManager::AddJob(const std::string& jobName, std::vector<std::string>& filePathList) {
    AdhocEvent* ev = new AdhocEvent(EVENT_START_JOB, jobName, filePathList);
    PushEventQueue(ev);
    Run();
}

void AdhocFileManager::DeleteJob(const std::string& jobName) {
    AdhocEvent* stopEvent = new AdhocEvent(EVENT_STOP_JOB, jobName);
    PushEventQueue(stopEvent);
}

Config* AdhocFileManager::GetJobConfig(const std::string& jobName) {
    return ConfigManager::GetInstance()->FindConfigByName(jobName);
}

} // namespace logtail