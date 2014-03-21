//
//  ClassDefinition.cpp
//  Server
//
//  Created by sancar koyunlu on 1/10/13.
//  Copyright (c) 2013 sancar koyunlu. All rights reserved.
//
#include "hazelcast/client/serialization/pimpl/ClassDefinition.h"
#include "hazelcast/client/serialization/pimpl/DataInput.h"
#include "hazelcast/client/serialization/pimpl/DataOutput.h"
#include "hazelcast/client/exception/IOException.h"

namespace hazelcast {
    namespace client {
        namespace serialization {
            namespace pimpl {
                ClassDefinition::ClassDefinition()
                : factoryId(0), classId(0), version(-1)
                , binary(new std::vector<byte>) {
                };

                ClassDefinition::ClassDefinition(int factoryId, int classId, int version)
                : factoryId(factoryId), classId(classId), version(version)
                , binary(new std::vector<byte>) {
                };

                void ClassDefinition::add(FieldDefinition &fd) {
                    fieldDefinitions.push_back(fd);
                    fieldDefinitionsMap[fd.getName()] = fd;
                };

                void ClassDefinition::add(boost::shared_ptr<ClassDefinition> cd) {
                    nestedClassDefinitions.push_back(cd);
                };

                bool ClassDefinition::isFieldDefinitionExists(const char *name) {
                    return (fieldDefinitionsMap.count(name) > 0);
                }

                const FieldDefinition &ClassDefinition::get(const char *name) {
                    return fieldDefinitionsMap[name];
                };

                std::vector<boost::shared_ptr<ClassDefinition> > &ClassDefinition::getNestedClassDefinitions() {
                    return nestedClassDefinitions;
                };

                bool ClassDefinition::hasField(const char *fieldName) const {
                    return fieldDefinitionsMap.count(fieldName) != 0;
                };

                FieldType ClassDefinition::getFieldType(const char *fieldName) const {
                    if (hasField(fieldName)) {
                        return fieldDefinitionsMap.at(fieldName).getType();
                    } else {
                        throw exception::IOException("ClassDefinition::getFieldType", "field does not exist");
                    }
                }

                int ClassDefinition::getFieldCount() const {
                    return (int) fieldDefinitions.size();
                };


                int ClassDefinition::getFactoryId() const {
                    return factoryId;
                }

                int ClassDefinition::getClassId() const {
                    return classId;
                };

                int ClassDefinition::getVersion() const {
                    return version;
                };


                void ClassDefinition::setVersion(int version) {
                    this->version = version;
                };

                const std::vector<byte> &ClassDefinition::getBinary() const {
                    return *(binary.get());
                };

                void ClassDefinition::setBinary(std::auto_ptr < std::vector<byte> > binary) {
                    this->binary.reset(binary.release());
                };

                void ClassDefinition::writeData(DataOutput &dataOutput) {
                    dataOutput.writeInt(factoryId);
                    dataOutput.writeInt(classId);
                    dataOutput.writeInt(version);
                    dataOutput.writeInt(fieldDefinitions.size());
		    std::vector<FieldDefinition>::iterator it;
                    for (it = fieldDefinitions.begin() ; it != fieldDefinitions.end(); ++it) {
                        it->writeData(dataOutput);
                    }

                    dataOutput.writeInt(nestedClassDefinitions.size());
		    std::vector<boost::shared_ptr<ClassDefinition>  >::iterator cdIt;
                    for (cdIt = nestedClassDefinitions.begin(); cdIt != nestedClassDefinitions.end(); ++cdIt) {
                        (*cdIt)->writeData(dataOutput);
                    }
                };

                void ClassDefinition::readData(DataInput &dataInput) {
                    factoryId = dataInput.readInt();
                    classId = dataInput.readInt();
                    version = dataInput.readInt();
                    int size = dataInput.readInt();
                    for (int i = 0; i < size; i++) {
                        FieldDefinition fieldDefinition;
                        fieldDefinition.readData(dataInput);
                        add(fieldDefinition);
                    }
                    size = dataInput.readInt();
                    for (int i = 0; i < size; i++) {
                        boost::shared_ptr<ClassDefinition> classDefinition(new ClassDefinition());
                        classDefinition->readData(dataInput);
                        add(classDefinition);
                    }
                };
            }
        }
    }
}
