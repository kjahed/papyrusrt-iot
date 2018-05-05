// umlrtcoapcommunicator.hh

/*******************************************************************************
* Copyright (c) 2014-2015 Zeligsoft (2009) Limited  and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*******************************************************************************/

#ifndef UMLRTCOAPCOMMUNICATOR_HH
#define UMLRTCOAPCOMMUNICATOR_HH

// UMLRTCOAPCommunicator 

#include "umlrthashmap.hh"
#include "umlrtcommunicator.hh"
#include "umlrtbasicthread.hh"

struct coap_context_t;

class UMLRTCOAPCommunicator : public UMLRTCommunicator
{
public:
    UMLRTCOAPCommunicator (const char * _localaddr ); 

    class UMLRTCoapServer : public UMLRTBasicThread
    {
    public:
        UMLRTCoapServer( const char* name ) : UMLRTBasicThread(name) {};

        coap_context_t * init( const char * ip, int port );
        void * run( void * args );
    };

    void connect ( UMLRTHost * host );
    void disconnect ( UMLRTHost * host );
    UMLRTCommunicator::Message* sendrecv();
    
    void sendDeployment ( UMLRTHost * host );
    void sendReadySignal ( UMLRTHost * host );
    void waitForReadySignal ( UMLRTHost * host );
    void waitForDeployment ( );
    
private:
    void sendBuffer ( UMLRTHost * host, uint8_t* buffer, int bufferSize );

    UMLRTCoapServer * coapServer;
    coap_context_t * coapContext;
    UMLRTHashMap * hostSessionMap;
};

#endif // UMLRTCOAPCOMMUNICATOR_HH
