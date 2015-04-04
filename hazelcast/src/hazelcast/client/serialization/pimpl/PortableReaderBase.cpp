//
// Created by İhsan Demir on 25/03/15.
//

#include <stdio.h>
#include "hazelcast/client/serialization/pimpl/DefaultPortableReader.h"
#include "hazelcast/client/exception/IllegalStateException.h"
#include "hazelcast/util/Bits.h"

using namespace hazelcast::client::serialization::pimpl;
using namespace hazelcast::client::serialization;

PortableReaderBase::PortableReaderBase(PortableContext &portableContext,
        DataInput &input,
        boost::shared_ptr<ClassDefinition> cd)
        : cd(cd)
        , dataInput(input)
        , serializerHolder(portableContext.getSerializerHolder())
        , objectDataInput(input, portableContext)
        , raw(false)
{
    int fieldCount;
    try {
        // final position after portable is read
        finalPosition = input.readInt();
        // field count
        fieldCount = input.readInt();
    } catch (exception::IException &e) {
        throw exception::HazelcastSerializationException("[DefaultPortableReader::DefaultPortableReader]", e.what());
    }
    if (fieldCount != cd->getFieldCount()) {
        char msg[50];
        sprintf(msg, "Field count[%d] in stream does not match %d", fieldCount, cd->getFieldCount());
        throw new exception::IllegalStateException("[DefaultPortableReader::DefaultPortableReader]", msg);
    }
    this->offset = input.position();
}

PortableReaderBase::~PortableReaderBase() {

}

int PortableReaderBase::readInt(const char *fieldName, bool skipTypeCheck) {
    setPosition(fieldName, FieldTypes::TYPE_INT, skipTypeCheck);
    return dataInput.readInt();
}

long PortableReaderBase::readLong(const char *fieldName, bool skipTypeCheck) {
    setPosition(fieldName, FieldTypes::TYPE_LONG, skipTypeCheck);
    return (long)dataInput.readLong();
}

bool PortableReaderBase::readBoolean(const char *fieldName, bool skipTypeCheck) {
    setPosition(fieldName, FieldTypes::TYPE_BOOLEAN, skipTypeCheck);
    return dataInput.readBoolean();
}

hazelcast::byte PortableReaderBase::readByte(const char *fieldName, bool skipTypeCheck) {
    setPosition(fieldName, FieldTypes::TYPE_BYTE, skipTypeCheck);
    return dataInput.readByte();
}

char PortableReaderBase::readChar(const char *fieldName, bool skipTypeCheck) {
    setPosition(fieldName, FieldTypes::TYPE_CHAR, skipTypeCheck);
    return dataInput.readChar();
}

double PortableReaderBase::readDouble(const char *fieldName, bool skipTypeCheck) {
    setPosition(fieldName, FieldTypes::TYPE_DOUBLE, skipTypeCheck);
    return dataInput.readDouble();
}

float PortableReaderBase::readFloat(const char *fieldName, bool skipTypeCheck) {
    setPosition(fieldName, FieldTypes::TYPE_FLOAT, skipTypeCheck);
    return dataInput.readFloat();
}

short PortableReaderBase::readShort(const char *fieldName, bool skipTypeCheck) {
    setPosition(fieldName, FieldTypes::TYPE_SHORT, skipTypeCheck);
    return dataInput.readShort();
}

std::string PortableReaderBase::readUTF(const char *fieldName) {
    setPosition(fieldName, FieldTypes::TYPE_UTF);
    return dataInput.readUTF();
}

std::vector<hazelcast::byte> PortableReaderBase::readByteArray(const char *fieldName) {
    setPosition(fieldName, FieldTypes::TYPE_BYTE_ARRAY);
    return dataInput.readByteArray();
}

std::vector<char> PortableReaderBase::readCharArray(const char *fieldName) {
    setPosition(fieldName, FieldTypes::TYPE_CHAR_ARRAY);
    return dataInput.readCharArray();
}

std::vector<int> PortableReaderBase::readIntArray(const char *fieldName) {
    setPosition(fieldName, FieldTypes::TYPE_INT_ARRAY);
    return dataInput.readIntArray();
}

std::vector<long> PortableReaderBase::readLongArray(const char *fieldName) {
    setPosition(fieldName, FieldTypes::TYPE_LONG_ARRAY);
    return dataInput.readLongArray();
}

std::vector<double> PortableReaderBase::readDoubleArray(const char *fieldName) {
    setPosition(fieldName, FieldTypes::TYPE_DOUBLE_ARRAY);
    return dataInput.readDoubleArray();
}

std::vector<float> PortableReaderBase::readFloatArray(const char *fieldName) {
    setPosition(fieldName, FieldTypes::TYPE_FLOAT_ARRAY);
    return dataInput.readFloatArray();
}

std::vector<short> PortableReaderBase::readShortArray(const char *fieldName) {
    setPosition(fieldName, FieldTypes::TYPE_SHORT_ARRAY);
    return dataInput.readShortArray();
}

void PortableReaderBase::getPortableInstance(char const *fieldName,
        Portable * &portableInstance) {
    setPosition(fieldName, FieldTypes::TYPE_PORTABLE);

    bool isNull = dataInput.readBoolean();
    int factoryId = dataInput.readInt();
    int classId = dataInput.readInt();

    checkFactoryAndClass(cd->getField(fieldName), factoryId, classId);

    if (isNull) {
        portableInstance = NULL;
    } else {
        read(dataInput, *portableInstance, factoryId, classId);
    }
}

void PortableReaderBase::getPortableInstancesArray(char const *fieldName,
        std::vector<Portable *> &portableInstances) {
    setPosition(fieldName, FieldTypes::TYPE_PORTABLE_ARRAY);

    int len = dataInput.readInt();
    int factoryId = dataInput.readInt();
    int classId = dataInput.readInt();

    checkFactoryAndClass(cd->getField(fieldName), factoryId, classId);

    if (len > 0) {
        int offset = dataInput.position();
        for (int i = 0; i < len; i++) {
            dataInput.position(offset + i * util::Bits::INT_SIZE_IN_BYTES);
            int start = dataInput.readInt();
            dataInput.position(start);

            read(dataInput, *(portableInstances[i]), factoryId, classId);
        }
    }
}


void PortableReaderBase::setPosition(char const *fieldName, FieldType const& fieldType,
        bool skipTypeCheck) {
    dataInput.position(readPosition(fieldName, fieldType, skipTypeCheck));
}

int PortableReaderBase::readPosition(const char *fieldName,
        FieldType const& fieldType, bool skipTypeCheck) {
    if (raw) {
        throw exception::HazelcastSerializationException("PortableReader::getPosition ", "Cannot read Portable fields after getRawDataInput() is called!");
    }
    if (!cd->hasField(fieldName)) {
        // TODO: if no field def found, java client reads nested position:
        // readNestedPosition(fieldName, type);
        throw exception::HazelcastSerializationException("PortableReader::getPosition ", "Don't have a field named " + std::string(fieldName));
    }

    if (!skipTypeCheck && cd->getFieldType(fieldName) != fieldType) {
        throw exception::HazelcastSerializationException("PortableReader::getPosition ", "Field type did not matched for " + std::string(fieldName));
    }

    dataInput.position(offset + cd->getField(fieldName).getIndex() * util::Bits::INT_SIZE_IN_BYTES);
    int pos = dataInput.readInt();

    dataInput.position(pos);
    short len = dataInput.readShort();

    // name + len + type
    return pos + util::Bits::SHORT_SIZE_IN_BYTES + len + 1;
}

hazelcast::client::serialization::ObjectDataInput&PortableReaderBase::getRawDataInput() {
    if (!raw) {
        dataInput.position(offset + cd->getFieldCount() * util::Bits::INT_SIZE_IN_BYTES);
        int pos = dataInput.readInt();
        dataInput.position(pos);
    }
    raw = true;
    return objectDataInput;
}

void PortableReaderBase::read(DataInput& dataInput, Portable& object, int factoryId, int classId) const {
    serializerHolder.getPortableSerializer().read(dataInput, object, factoryId, classId);
}

void PortableReaderBase::end() {
    dataInput.position(finalPosition);
}

void PortableReaderBase::checkFactoryAndClass(FieldDefinition fd, int factoryId, int classId) const {
    if (factoryId != fd.getFactoryId()) {
        char msg[100];
        sprintf(msg, "Invalid factoryId! Expected: %d, Current: %d", fd.getFactoryId(), factoryId);
        throw exception::HazelcastSerializationException("DefaultPortableReader::checkFactoryAndClass ", std::string(msg));
    }
    if (classId != fd.getClassId()) {
        char msg[100];
        sprintf(msg, "Invalid classId! Expected: %d, Current: %d", fd.getClassId(), classId);
        throw exception::HazelcastSerializationException("DefaultPortableReader::checkFactoryAndClass ", std::string(msg));
    }
}


