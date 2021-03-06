/*
 * Copyright (c) 2008-2017, Hazelcast, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//
// Created by sancar koyunlu on 20/08/14.
//

#ifndef HAZELCAST_HeartBeater
#define HAZELCAST_HeartBeater

#include "hazelcast/util/HazelcastDll.h"
#include "hazelcast/util/AtomicBoolean.h"

#if  defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(push)
#pragma warning(disable: 4251) //for dll export
#endif

namespace hazelcast {
    namespace util {
        class ThreadArgs;

        class Thread;
    }
    namespace client {
        namespace spi{
            class ClientContext;
        }

        namespace connection {
            class HAZELCAST_API HeartBeater {
            public:
                HeartBeater(spi::ClientContext& clientContext);

                static void staticStart(util::ThreadArgs& args);

                void shutdown();

            private:
                void run(util::Thread *currentThread);

                util::AtomicBoolean live;
                spi::ClientContext& clientContext;
                int heartBeatIntervalSeconds;
                int heartBeatTimeoutSeconds;
            };
        }
    }
}

#if  defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(pop)
#endif

#endif //HAZELCAST_HeartBeater
