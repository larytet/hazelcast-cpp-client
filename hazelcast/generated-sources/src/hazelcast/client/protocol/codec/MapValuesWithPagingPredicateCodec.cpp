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



#include "hazelcast/client/protocol/codec/MapValuesWithPagingPredicateCodec.h"
#include "hazelcast/client/exception/UnexpectedMessageTypeException.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace codec {
                const MapMessageType MapValuesWithPagingPredicateCodec::RequestParameters::TYPE = HZ_MAP_VALUESWITHPAGINGPREDICATE;
                const bool MapValuesWithPagingPredicateCodec::RequestParameters::RETRYABLE = false;
                const int32_t MapValuesWithPagingPredicateCodec::ResponseParameters::TYPE = 117;
                std::auto_ptr<ClientMessage> MapValuesWithPagingPredicateCodec::RequestParameters::encode(
                        const std::string &name, 
                        const serialization::pimpl::Data &predicate) {
                    int32_t requiredDataSize = calculateDataSize(name, predicate);
                    std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                    clientMessage->setMessageType((uint16_t)MapValuesWithPagingPredicateCodec::RequestParameters::TYPE);
                    clientMessage->setRetryable(RETRYABLE);
                    clientMessage->set(name);
                    clientMessage->set(predicate);
                    clientMessage->updateFrameLength();
                    return clientMessage;
                }

                int32_t MapValuesWithPagingPredicateCodec::RequestParameters::calculateDataSize(
                        const std::string &name, 
                        const serialization::pimpl::Data &predicate) {
                    int32_t dataSize = ClientMessage::HEADER_SIZE;
                    dataSize += ClientMessage::calculateDataSize(name);
                    dataSize += ClientMessage::calculateDataSize(predicate);
                    return dataSize;
                }

                MapValuesWithPagingPredicateCodec::ResponseParameters::ResponseParameters(ClientMessage &clientMessage) {
                    if (TYPE != clientMessage.getMessageType()) {
                        throw exception::UnexpectedMessageTypeException("MapValuesWithPagingPredicateCodec::ResponseParameters::decode", clientMessage.getMessageType(), TYPE);
                    }

                    response = clientMessage.getArray<std::pair<serialization::pimpl::Data, serialization::pimpl::Data > >();
                }

                MapValuesWithPagingPredicateCodec::ResponseParameters MapValuesWithPagingPredicateCodec::ResponseParameters::decode(ClientMessage &clientMessage) {
                    return MapValuesWithPagingPredicateCodec::ResponseParameters(clientMessage);
                }

                MapValuesWithPagingPredicateCodec::ResponseParameters::ResponseParameters(const MapValuesWithPagingPredicateCodec::ResponseParameters &rhs) {
                        response = rhs.response;
                }
                //************************ EVENTS END **************************************************************************//

            }
        }
    }
}

