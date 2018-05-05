// umlrtcommunicator.hh

/*******************************************************************************
* Copyright (c) 2014-2015 Zeligsoft (2009) Limited  and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*******************************************************************************/

#ifndef UMLRTCOMMUNICATOR_HH
#define UMLRTCOMMUNICATOR_HH

// UMLRTCommunicator is the main controller-class.

#include "umlrtqueue.hh"
#include "umlrtqueueelement.hh"

//#include "flatbuffers/flatbuffers.h"
//#include "fbschemas/signal_generated.h"
//#include "fbschemas/message_generated.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;

#include <vector>

struct UMLRTHost;
struct UMLRTObject_class;

#define COMMUNICATOR_TIMEOUT 1000

class UMLRTCommunicator
{
public:
    UMLRTCommunicator ( const char * _localaddr )
        : localaddr(_localaddr), abort(false) {};
    
    class Message : public UMLRTQueueElement 
    {
        public:
            Message ( ) : payload(NULL), payloadSize(0) {};
            ~Message ( );

            const char * sender;
            const char * receiver;
            int type;
            int payloadSize;
            uint8_t * payload;

            //optional
            const char * mqttBroker;
            const char * mqttTopic;

            bool isProtocolSignal ( );
            bool isDeploymentSignal ( );
            bool isReadySignal ( );

            int encode ( uint8_t ** buffer );
            static Message * decode ( uint8_t * buffer );

        private:
            int toFlatBuffer ( uint8_t ** buffer );
            int toJSON ( uint8_t ** buffer );
            static Message * fromFlatBuffer ( uint8_t * buffer );
            static Message * fromJSON ( uint8_t * buffer );
    };

    class DeploymentMessage : public Message
    {
        public:
            static int make ( UMLRTHost * src, UMLRTHost * dest, uint8_t ** buffer );
            static DeploymentMessage * make ( uint8_t * buffer );
    };

    class ReadyMessage : public Message
    {
        public:
            static int make ( UMLRTHost * src, UMLRTHost * dest, uint8_t ** buffer );
            static ReadyMessage * make ( uint8_t * buffer );
    };

    class ProtocolMessage : public Message
    {
        public:
            static  ProtocolMessage * make ( UMLRTHost * src, UMLRTHost * dest );
            static ProtocolMessage * make ( uint8_t * buffer );
    };

    class Signal 
    {
        public:
            const char* sourceSlot;
            const char* destinationSlot;

            int sourcePort;
            int destinationPort;

            bool destinationInternal;

            const char* protocolName;
            const char* signalName;

            uint8_t * payload;
            int payloadSize;

            int encode ( uint8_t ** buffer );
            static Signal * decode ( uint8_t * buffer );

        private:
            int toFlatBuffer ( uint8_t ** buffer );
            int toJSON ( uint8_t ** buffer );
            void toJSON_param( Document & document, 
                    Value & paramObj, const UMLRTObject_class * desc, void * data );

            //void toFlatBuffer_param( std::vector<flatbuffers::Offset<FBSchema::Obj>> & accumulator, 
            //        const UMLRTObject_class * desc, void * data );

            static bool fromJSON_param( const Value & param,
                    const UMLRTObject_class * desc, uint8_t * buffer);
            
//            static bool fromFlatBuffer_param( const FBSchema::Obj * param, 
//                    const UMLRTObject_class * desc, uint8_t * buffer);
            static Signal * fromFlatBuffer ( uint8_t * buffer );
            static Signal * fromJSON ( uint8_t * buffer );
    };

    virtual void connect ( UMLRTHost * host ) = 0;
    virtual void disconnect ( UMLRTHost * host ) = 0;
    virtual UMLRTCommunicator::Message* sendrecv() = 0;

    virtual void sendDeployment ( UMLRTHost * host ) = 0;
    virtual void sendReadySignal ( UMLRTHost * host ) = 0;
    virtual void waitForReadySignal ( UMLRTHost * host ) = 0;
    virtual void waitForDeployment ( ) = 0;
   
    inline void queueMessage ( UMLRTCommunicator::Message * msg ) 
    {
        messageQueue.enqueue(msg);
    }

    inline void setLocalHost ( UMLRTHost* host )
    {
        localhost = host;
    }

    inline UMLRTHost* getLocalHost ( )
    {
        return localhost;
    }

protected:    
    bool abort;
    const char * localaddr;
    UMLRTHost * localhost;
    UMLRTQueue messageQueue;

    void quit ( );
};
#endif // UMLRTCOMMUNICATOR_HH
