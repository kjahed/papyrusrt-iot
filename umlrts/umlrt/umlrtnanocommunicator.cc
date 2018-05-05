// umlrtnanocommunicator.cc

/*******************************************************************************

*******************************************************************************/

// UMLRTNanoCommunicator manages all external communication.

#include "basedebug.hh"
#include "basefatal.hh"
#include "umlrtnanocommunicator.hh"
#include "umlrthost.hh"

#include "nanomsg/nn.h"
#include "nanomsg/pair.h"
#include "nanomsg/reqrep.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

static UMLRTQueue incomingTcpMessages;

UMLRTNanoCommunicator::UMLRTNanoCommunicator ( const char * _localaddr )
	: UMLRTCommunicator(_localaddr)
{
    char* protocol = strtok(strdup(_localaddr), ":"); // tcp://ip:port
    char* ip = strtok(NULL, ":");
    char* port = strtok(NULL, ":");
    ip+=2;

    tcpServer = new UMLRTTcpServer( "TCP Server" );
    tcpServer->init(ip, atoi(port));
    tcpServer->start(NULL);

    //localsock = nn_socket(AF_SP, NN_PAIR);
	//if( nn_bind (localsock, localaddr) < 0 )
	//	FATAL("Cannot bind to address %s", localaddr);
}   

void UMLRTNanoCommunicator::sendDeployment ( UMLRTHost * host )
{
    uint8_t* buffer; 
    int bufferSize = UMLRTCommunicator::DeploymentMessage::make(localhost, host, &buffer); 
//	if ( nn_send (host->socket, buffer, bufferSize, 0) < 0 )
//		FATAL("Error sending deployment to host  %s\n", host->name);
    sendBuffer(host, buffer, bufferSize);
	host->deployed = true;
    free(buffer);
}


void UMLRTNanoCommunicator::waitForDeployment ( )
{
    while( incomingTcpMessages.count() == 0 )
        usleep(100);

    UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*)incomingTcpMessages.dequeue();
	UMLRTDeploymentMap::decode( msg->payload, msg->payloadSize );
    delete msg;
}

void UMLRTNanoCommunicator::sendReadySignal ( UMLRTHost * host )
{
    uint8_t* buffer;
    int bufferSize = UMLRTCommunicator::ReadyMessage::make(localhost, host, &buffer);
//	if ( nn_send (host->socket, buffer, bufferSize, 0) < 0 )
//		FATAL("Error sending ready signal to host  %s\n", host->name);
    sendBuffer(host, buffer, bufferSize);
    host->signaled = true;
    free(buffer);
}

void UMLRTNanoCommunicator::waitForReadySignal ( UMLRTHost * host )
{
    while( incomingTcpMessages.count() == 0 )
        usleep(100);

    UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*)incomingTcpMessages.dequeue();
    host->gotack = true;
    delete msg;
}

void UMLRTNanoCommunicator::sendBuffer( UMLRTHost * host, uint8_t* buffer, int bufferSize )
{
    //static int chunkSize = 128 * 1000;
    int chunkSize = bufferSize;

    if(send(host->socket, &bufferSize, sizeof(int), 0 ) < 0)
        FATAL("Error sending message to host @ %s\n", host->name);

    int left = bufferSize;
    int totalSent = 0;
    while( left > 0 )
    {
        int length = left - chunkSize >= 0 ? chunkSize : left; 
        int sent = send(host->socket , buffer+totalSent , length, 0 ); 
        //nn_send (destination->socket, buffer+totalSent, length, 0);
        if ( sent < 0 )
            FATAL("Error sending message to host @ %s\n", host->name);
    
        left -= sent;
        totalSent += sent;
    }
}

void UMLRTNanoCommunicator::connect ( UMLRTHost * host )
{
    char* protocol = strtok(strdup(host->address), ":"); // tcp://ip:port
    char* ip = strtok(NULL, ":");
    char* port = strtok(NULL, ":");
    ip+=2;

    host->socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port));
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    int ret;
    while( (ret = ::connect(host->socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0))
            usleep(101);
   
   int val = 1;
   setsockopt(host->socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));

//	host->socket = nn_socket(AF_SP, NN_PAIR);
//	if( nn_connect(host->socket, host->address) < 0 )
//		FATAL("Unable to connect to host @ %s\n", host->address);
	host->connected = true;
}

void UMLRTNanoCommunicator::disconnect ( UMLRTHost * host )
{
//	if(host->socket != 0)
//		nn_shutdown(host->socket, 0);
	host->connected = false;
}

UMLRTCommunicator::Message* UMLRTNanoCommunicator::sendrecv()
{
    //static int chunkSize = 128 * 1000;

	UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*) messageQueue.dequeue();
	if(msg != NULL)
	{
        uint8_t* buffer;
        int bufferSize = msg->encode(&buffer);
        int chunkSize = bufferSize;
        UMLRTHost* destination = UMLRTDeploymentMap::getHostFromName(msg->receiver);
        if(destination == NULL)
            FATAL("No such destination host %s\n", msg->receiver);

        sendBuffer(destination, buffer, bufferSize);
//        if( nn_send (destination->socket, &bufferSize, sizeof(int), 0) < 0 )
//             FATAL("Error sending message size to host @ %s\n", destination->name);
//
//        int left = bufferSize;
//        int totalSent = 0;
//        while( left > 0 )
//        {
//            int length = left - chunkSize >= 0 ? chunkSize : left; 
//            int sent = nn_send (destination->socket, buffer+totalSent, length, 0);
//    	    if ( sent < 0 )
//			    FATAL("Error sending message %s to host @ %s\n", buffer, destination->name);
//	    
//            left -= sent;
//            totalSent += sent;
//        }

        free(buffer);
		delete msg;
	}

    if(incomingTcpMessages.count() > 0)
        return (UMLRTCommunicator::Message*)incomingTcpMessages.dequeue();
//    int bufferSize;
//    if( nn_recv(localsock, &bufferSize, sizeof(int), NN_DONTWAIT) > 0 )
//    {
//        int left = bufferSize;
//        int totalReceived = 0;
//        int chunkSize = bufferSize;
//        uint8_t* buffer = (uint8_t*) malloc(bufferSize);
//        while( left > 0 )
//        {
//            int length = left - chunkSize >= 0 ? chunkSize : left;
//            int recv = nn_recv (localsock, buffer+totalReceived, length, 0);
//            if(recv < 0)
//                FATAL("Error receving message from socket, err: %s\n", nn_strerror(nn_errno()));
//            
//            left -= recv;
//            totalReceived += recv;
//        }
//         
//        UMLRTCommunicator::Message* msg = UMLRTCommunicator::Message::decode(buffer);
//        free(buffer);
//		return msg;
//    }

	return NULL;
}

void UMLRTNanoCommunicator::UMLRTTcpServer::init( const char * ip, int port ) 
{
    int opt = 1;
    localsock = socket(AF_INET, SOCK_STREAM, 0);
	if(localsock < 0)
        FATAL("Error creating socket\n");

    setsockopt(localsock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    localsockaddr.sin_family = AF_INET;
    localsockaddr.sin_addr.s_addr = inet_addr(ip);
    localsockaddr.sin_port = htons(port);
    localsockaddr_len = sizeof(localsockaddr);

    int ret = bind(localsock, (struct sockaddr *)&localsockaddr, sizeof(localsockaddr));
    if(ret < 0)
        FATAL("Error binding socket\n");
    ret = listen(localsock, 3);
    if(ret < 0)
        FATAL("Error listening socket\n");
}

void * UMLRTNanoCommunicator::UMLRTTcpServer::run( void * args )
{
    clientsock = accept(localsock, (struct sockaddr *)&localsockaddr, 
                   (socklen_t*)&localsockaddr_len);
    if(clientsock < 0)
        FATAL("Error acceptign connection \n");
     
   int val = 1;
   setsockopt(clientsock, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));

   while(true)
   { 
        int bufferSize;
        int recv = read(clientsock, &bufferSize, sizeof(int));

        int chunkSize = bufferSize;
        int left = bufferSize;
        int totalReceived = 0;
        uint8_t* buffer = (uint8_t*) malloc(bufferSize);

        while( left > 0 )
        {
            int length = left - chunkSize >= 0 ? chunkSize : left;
            int recv = read(clientsock, buffer+totalReceived, length);
            if(recv < 0)
                FATAL("Error reading from client socket\n");

            left -= recv;
            totalReceived += recv;
        }

        UMLRTCommunicator::Message* msg = UMLRTCommunicator::Message::decode(buffer);
        incomingTcpMessages.enqueue(msg);
        free(buffer);
   }
}
