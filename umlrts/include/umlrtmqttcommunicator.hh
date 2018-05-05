// umlrtcommunicator.hh

/*******************************************************************************
* Copyright (c) 2014-2015 Zeligsoft (2009) Limited  and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*******************************************************************************/

#ifndef UMLRTMQTTCOMMUNICATOR_HH
#define UMLRTMQTTCOMMUNICATOR_HH

// UMLRTMQTTCommunicator 

#include "umlrthashmap.hh"
#include "umlrtcommunicator.hh"

class UMLRTMQTTCommunicator : public UMLRTCommunicator
{
public:
    UMLRTMQTTCommunicator (const char * _localaddr ); 

    void connect ( UMLRTHost * host );
    void disconnect ( UMLRTHost * host );
    UMLRTCommunicator::Message* sendrecv();
    
    void sendDeployment ( UMLRTHost * host );
    void sendReadySignal ( UMLRTHost * host );
    void waitForReadySignal ( UMLRTHost * host );
    void waitForDeployment ( );

    void subscribe( UMLRTHost * host, const char* topic );
    void publish( UMLRTHost* host, const char* topic , uint8_t* buffer, int bufferSize );
private:    
    void connect ( const char * address );
    UMLRTHashMap* clientMap;
};

#endif // UMLRTMQTTCOMMUNICATOR_HH
