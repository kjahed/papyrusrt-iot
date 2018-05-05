// umlrtmqttprotocol.hh

#ifndef UMLRTMQTTPROTOCOL_HH
#define UMLRTMQTTPROTOCOL_HH

#include "umlrtprotocol.hh"
#include "umlrtsignal.hh"
#include "umlrtobjectclass.hh"

struct UMLRTCommsPort;

// Protocol for mqtt ports.

// MQTT protocol for the mqtt port
class UMLRTMQTTProtocol
{
public:
    enum SignalId { signal_received = UMLRTSignal::FIRST_PROTOCOL_SIGNAL_ID };

    struct params_received
    {
        char * topic;
        char * message;
    };

    static UMLRTObject_field fields_received[];
    static UMLRTObject payload_received;

    class InSignals {  };
    class OutSignals {
    public:
        
        const bool subscribe( const UMLRTCommsPort * srcPort, const char* topic ) const;
        const bool unsubscribe( const UMLRTCommsPort * srcPort, const char* topic ) const;
        const bool publish( const UMLRTCommsPort * srcPort, const char* topic, const char* message) const;
    };

    typedef OutSignals Base;
    typedef InSignals Conjugate;
};

class UMLRTMQTTProtocol_baserole : protected UMLRTProtocol, private UMLRTMQTTProtocol::Base
{
public:
    UMLRTMQTTProtocol_baserole( const UMLRTCommsPort *& srcPort ) : UMLRTProtocol( srcPort ) { }

    // See UMLRTMQTTProtocol.
    const bool subscribe( const char * topic ) const
    {
        return UMLRTMQTTProtocol::Base::subscribe( srcPort, topic );
    }

    const bool unsubscribe( const char * topic ) const
    {
        return UMLRTMQTTProtocol::Base::unsubscribe( srcPort, topic );
    }

    const bool publish( const char * topic, const char* message ) const
    {
        return UMLRTMQTTProtocol::Base::publish( srcPort, topic, message );
    }
};

#endif // UMLRTMQTTPROTOCOL_HH
