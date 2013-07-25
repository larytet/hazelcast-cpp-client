//
// Created by sancar koyunlu on 6/25/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_MultiMapIsLockedRequest
#define HAZELCAST_MultiMapIsLockedRequest


#include "../serialization/SerializationConstants.h"
#include "../serialization/Data.h"
#include "CollectionPortableHook.h"
#include "CollectionProxyId.h"
#include "Portable.h"
#include <vector>

namespace hazelcast {
    namespace client {
        namespace collection {
            class MultiMapIsLockedRequest : public Portable{
            public:
                MultiMapIsLockedRequest(const CollectionProxyId& id, const serialization::Data& key, int threadId)
                :proxyId(id)
                , key(key)
                , threadId(threadId) {

                };

                int getFactoryId() const {
                    return CollectionPortableHook::F_ID;
                };

                int getClassId() const {
                    return CollectionPortableHook::IS_LOCKED;
                };

                template<typename HzWriter>
                void writePortable(HzWriter& writer) const {
                    writer.writeInt("tid",threadId);
                    serialization::BufferedDataOutput *out = writer.getRawDataOutput();
                    key.writeData(*out);
                    proxyId.writeData(*out);
                };

                template<typename HzReader>
                void readPortable(HzReader& reader) {
                    threadId = reader.readInt("tid");
                    serialization::BufferedDataInput *in = reader.getRawDataInput();
                    key.readData(*in);
                    proxyId.readData(*in);
                };

            private:
                CollectionProxyId proxyId;
                serialization::Data key;
                int threadId;
            };

        }
    }
}
#endif //HAZELCAST_MultiMapIsLockedRequest