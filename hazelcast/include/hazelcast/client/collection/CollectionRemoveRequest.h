//
// Created by sancar koyunlu on 9/12/13.
// Copyright (c) 2013 hazelcast. All rights reserved.




#ifndef HAZELCAST_CollectionRemoveRequest
#define HAZELCAST_CollectionRemoveRequest

#include "hazelcast/client/collection/CollectionRequest.h"

namespace hazelcast {
    namespace client {
        namespace serialization {
            class Data;
        }
        namespace collection {
            class CollectionRemoveRequest : public CollectionRequest {
            public:
                CollectionRemoveRequest(const std::string& name, const serialization::Data& data);

                int getClassId() const;

                void writePortable(serialization::PortableWriter& writer) const;

            private:
                const serialization::Data& data;
            };
        }
    }
}

#endif //HAZELCAST_CollectionRemoveRequest