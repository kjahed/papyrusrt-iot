// umlrtmqttprotocol.cc

#include "umlrtmqttprotocol.hh"

UMLRTObject_field UMLRTMQTTProtocol::fields_received[] = 
{
    {
        "topic",
        &UMLRTType_charptr,
        offsetof( params_received, topic ),
        1,
        0
    },
    {
        "message",
        &UMLRTType_charptr,
        offsetof( params_received, message ),
        1,
        0
    }
};

UMLRTObject UMLRTMQTTProtocol::payload_received = 
{
    sizeof( params_received ),
    2,
    fields_received
};

const bool UMLRTMQTTProtocol::OutSignals::subscribe( const UMLRTCommsPort * srcPort, const char* topic ) const 
{
    int ret = srcPort->mqttClient->subscribe(topic);
    return ret == 0;
}

const bool UMLRTMQTTProtocol::OutSignals::unsubscribe( const UMLRTCommsPort * srcPort, const char* topic ) const 
{
    int ret = srcPort->mqttClient->unsubscribe(topic);
    return ret == 0;
}

const bool UMLRTMQTTProtocol::OutSignals::publish( const UMLRTCommsPort * srcPort, const char* topic, const char* message ) const 
{
    int ret = srcPort->mqttClient->publish(topic, (void*) message, strlen(message));
    return ret == 0;
}