//
// Created by sancar koyunlu on 6/17/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_LIFECYCLE_SERVICE
#define HAZELCAST_LIFECYCLE_SERVICE

#include "hazelcast/util/HazelcastDll.h"
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>
#include <set>

namespace hazelcast {
    namespace client {
        class LifecycleListener;

        class LifecycleEvent;

        namespace spi {

            class ClientContext;

            class HAZELCAST_API LifecycleService {
            public:
                LifecycleService(ClientContext& clientContext);

                void start();

                void addLifecycleListener(LifecycleListener *lifecycleListener);

                bool removeLifecycleListener(LifecycleListener *lifecycleListener);

                bool isRunning();

                void shutdown();

                void fireLifecycleEvent(const LifecycleEvent& lifecycleEvent);
            private:
                ClientContext& clientContext;
                std::set<LifecycleListener *> listeners;
                boost::mutex listenerLock;

                boost::atomic<bool> active;

            };

        }
    }
}


#endif //__LifecycleService_H_
