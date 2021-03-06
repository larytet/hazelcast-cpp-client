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
// Created by sancar koyunlu on 21/08/14.
//


#ifndef HAZELCAST_CallFuture
#define HAZELCAST_CallFuture

#include "hazelcast/util/HazelcastDll.h"

#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace hazelcast {
    namespace client {
        namespace protocol {
            class ClientMessage;
        }

        namespace spi{
            class InvocationService;
        }

        namespace connection {
            class CallPromise;

            class Connection;

            class CallFuture {
            public:
                CallFuture();

                CallFuture(boost::shared_ptr<CallPromise> promise, boost::shared_ptr<Connection> connection, int heartBeatTimeout, spi::InvocationService* invocationService);

                CallFuture(const CallFuture &rhs);

                CallFuture &operator=(const CallFuture &rhs);

                std::auto_ptr<protocol::ClientMessage> get();

                std::auto_ptr<protocol::ClientMessage> get(time_t timeoutInSeconds);

                int64_t getCallId() const;

                const Connection &getConnection() const;
            private:
                boost::shared_ptr<CallPromise> promise;
                boost::shared_ptr<Connection> connection;
                spi::InvocationService* invocationService;
                int heartBeatTimeout;
            };

        }
    }
}

#endif //HAZELCAST_CallFuture
