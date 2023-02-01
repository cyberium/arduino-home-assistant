#include "HASensor.h"
#ifndef EX_ARDUINOHA_SENSOR

#include "../HAMqtt.h"
#include "../utils/HASerializer.h"

HASensor::HASensor(const char* uniqueId) :
    HABaseDeviceType(AHATOFSTR(HAComponentSensor), uniqueId),
    _deviceClass(nullptr),
    _stateClass(nullptr),
    _forceUpdate(false),
    _hasAttributes(false),
    _icon(nullptr),
    _unitOfMeasurement(nullptr)
{

}

bool HASensor::setValue(const char* value)
{
    return publishOnDataTopic(AHATOFSTR(HAStateTopic), value, true);
}

bool HASensor::setAttributes(const char* json)
{
    if (!_hasAttributes)
        return false;
    return publishOnDataTopic(AHATOFSTR(HAAttributeTopic), json, true);
}

void HASensor::buildSerializer()
{
    if (_serializer || !uniqueId()) {
        return;
    }

    _serializer = new HASerializer(this, 12); // 10 - max properties nb
    _serializer->set(AHATOFSTR(HANameProperty), _name);
    _serializer->set(AHATOFSTR(HAUniqueIdProperty), _uniqueId);
    _serializer->set(AHATOFSTR(HAObjectIdProperty), _uniqueId);
    _serializer->set(AHATOFSTR(HADeviceClassProperty), _deviceClass);
    _serializer->set(AHATOFSTR(HAStateClassProperty), _stateClass);
    _serializer->set(AHATOFSTR(HAIconProperty), _icon);
    _serializer->set(AHATOFSTR(HAUnitOfMeasurementProperty), _unitOfMeasurement);

    // optional property
    if (_forceUpdate) {
        _serializer->set(
            AHATOFSTR(HAForceUpdateProperty),
            &_forceUpdate,
            HASerializer::BoolPropertyType
        );
    }
    _serializer->set(HASerializer::WithDevice);
    _serializer->set(HASerializer::WithAvailability);
    _serializer->topic(AHATOFSTR(HAStateTopic));
    if (_hasAttributes)
        _serializer->topic(AHATOFSTR(HAAttributeTopic));
}

void HASensor::onMqttConnected()
{
    if (!uniqueId()) {
        return;
    }

    publishConfig();
    publishAvailability();
}

#endif
