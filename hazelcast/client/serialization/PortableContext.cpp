//
// Created by sancar koyunlu on 5/2/13.
// Copyright (c) 2013 sancar koyunlu. All rights reserved.
//
// To change the template use AppCode | Preferences | File Templates.
//


#include "PortableContext.h"
#include "DataInput.h"
#include "SerializationService.h"
#include "ClassDefinition.h"
#include <zlib.h>

namespace hazelcast {
    namespace client {
        namespace serialization {


            bool PortableContext::isClassDefinitionExists(int classId, int version) const {
                long key = SerializationService::combineToLong(classId, version);
                return (versionedDefinitions.count(key) > 0);
            };

            boost::shared_ptr<ClassDefinition> PortableContext::lookup(int classId, int version) {
                long key = SerializationService::combineToLong(classId, version);
                return versionedDefinitions[key];

            };

            boost::shared_ptr<ClassDefinition> PortableContext::createClassDefinition(std::vector<byte>& binary) {

                std::vector<byte> decompressed = decompress(binary);

                DataInput dataInput = DataInput(decompressed);
                boost::shared_ptr<ClassDefinition> cd(new ClassDefinition);
                operator >>(dataInput, cd);
//                dataInput >> cd;
//                cd->readData(dataInput);
                cd->setBinary(binary);

                long key = service->combineToLong(cd->getClassId(), context->getVersion());

                if (versionedDefinitions.count(key) == 0) {
                    context->registerNestedDefinitions(cd);
                    versionedDefinitions[key] = cd;
                    return cd;
                }

                return versionedDefinitions[key];
            };

            void PortableContext::registerClassDefinition(boost::shared_ptr<ClassDefinition> cd) {
                if (cd->getVersion() < 0) {
                    cd->setVersion(context->getVersion());
                }
                if (!isClassDefinitionExists(cd->getClassId(), cd->getVersion())) {
                    if (cd->getBinary().size() == 0) {
                        DataOutput *output = service->pop();
                        operator<<(*output, cd);
//                        (*output) << cd;
//                        cd->writeData(*output);
                        std::vector<byte> binary = output->toByteArray();
                        compress(binary);
                        cd->setBinary(binary);
                        service->push(output);
                    }
                    long versionedClassId = service->combineToLong(cd->getClassId(), cd->getVersion());
                    versionedDefinitions[versionedClassId] = cd;
                }
            };

            void PortableContext::compress(std::vector<byte>& binary) {
                uLong ucompSize = binary.size();
                uLong compSize = compressBound(ucompSize);
                byte uncompressedTemp[binary.size()];
                for (int i = 0; i < binary.size(); i++)
                    uncompressedTemp[i] = binary[i];

                byte compressedTemp[compSize];
                int err = compress2((Bytef *) compressedTemp, &compSize, (Bytef *) uncompressedTemp, ucompSize, Z_BEST_COMPRESSION);
                switch (err) {
                    case Z_BUF_ERROR:
                        throw hazelcast::client::HazelcastException("not enough room in the output buffer at compression");
                    case Z_DATA_ERROR:
                        throw hazelcast::client::HazelcastException("data is corrupted at compression");
                    case Z_MEM_ERROR:
                        throw hazelcast::client::HazelcastException("if there was not  enough memory at compression");
                }
                std::vector<byte> compressed(compressedTemp, compressedTemp + compSize);
                binary = compressed;
            };

            std::vector<byte> PortableContext::decompress(std::vector<byte> const & binary) const {
                uLong compSize = binary.size();

                uLong ucompSize = 512;
                byte *temp = NULL;
                byte compressedTemp[binary.size()];
                for (int i = 0; i < binary.size(); i++)
                    compressedTemp[i] = binary[i];
                int err = Z_OK;
                do {
                    ucompSize *= 2;
                    delete [] temp;
                    temp = new byte[ucompSize];
                    err = uncompress((Bytef *) temp, &ucompSize, (Bytef *) compressedTemp, compSize);
                    switch (err) {
                        case Z_BUF_ERROR:
                            std::cerr << "buffer size is not enough" << std::endl;
                        case Z_DATA_ERROR:
                            std::cerr << "data is corrupted or incomplete at decompression" << std::endl;
                        case Z_MEM_ERROR:
                            std::cerr << "there was not  enough memory at decompression" << std::endl;
                    }
                } while (err != Z_OK);
                std::vector<byte> decompressed(temp, temp + ucompSize);
                delete [] temp;
                return decompressed;
            };

        }
    }
}