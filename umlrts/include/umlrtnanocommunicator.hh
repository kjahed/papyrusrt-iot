// umlrtnanocommunicator.hh

/*******************************************************************************
* Copyright (c) 2014-2015 Zeligsoft (2009) Limited  and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*******************************************************************************/

#ifndef UMLRTNANOCOMMUNICATOR_HH
#define UMLRTNANOCOMMUNICATOR_HH

#include "umlrtcommunicator.hh"
#include "umlrtbasicthread.hh"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
// UMLRTNanoCommunicator

class UMLRTNanoCommunicator : public UMLRTCommunicator
{
public:
    UMLRTNanoCommunicator ( const char * _localaddr );

    class UMLRTTcpServer : public UMLRTBasicThread
    {
    public:
        UMLRTTcpServer( const char* name ) : UMLRTBasicThread(name) {};

        void init( const char * ip, int port );
        void * run( void * args );
    private:
        int localsock;
        struct sockaddr_in localsockaddr;
        int localsockaddr_len;
        int clientsock;
    };

    void connect ( UMLRTHost * host );
    void disconnect ( UMLRTHost * host ); 
    UMLRTCommunicator::Message* sendrecv();
    void sendDeployment ( UMLRTHost * host );
    void sendReadySignal ( UMLRTHost * host );
    void waitForReadySignal ( UMLRTHost * host );
    void waitForDeployment ( );

private:    
    void sendBuffer( UMLRTHost * host, uint8_t* buffer, int bufferSize );
    int recvBuffer( uint8_t ** buffer, bool block );

    UMLRTTcpServer * tcpServer;
};

#endif // UMLRTNANOCOMMUNICATOR_HH
