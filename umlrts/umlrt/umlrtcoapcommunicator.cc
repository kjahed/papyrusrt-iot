// umlrtcoapcommunicator.cc

/*******************************************************************************

*******************************************************************************/

// UMLRTCOAPCommunicator manages all COAP communication.

#include "basedebug.hh"
#include "basefatal.hh"
#include "umlrtqueue.hh"
#include "umlrtcoapcommunicator.hh"
#include "umlrthost.hh"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "coap/coap.h"

#define COAP_RESOURCE_CHECK_TIME 2

static UMLRTQueue incomingCoapMessages;
static uint8_t* coapTempBuffer = NULL;
static int coapTempBufferCapacity = 0;
static int coapTempBufferSize = 0;

static void coapRequestReceivedCB(coap_context_t *ctx, struct coap_resource_t *resource,
    coap_session_t *session, coap_pdu_t *request, str *token, str *query, coap_pdu_t *response) {

    coap_opt_t *block_opt;
    coap_opt_iterator_t opt_iter;
    block_opt = coap_check_option(request, COAP_OPTION_BLOCK1, &opt_iter);

    uint8_t* buffer;
    size_t bufferSize;
    coap_get_data(request, &bufferSize, &buffer);

    if( block_opt ) 
    { 
        int num = coap_opt_block_num(block_opt); 

        if( COAP_OPT_BLOCK_MORE(block_opt) )
        { 
            if(coapTempBufferCapacity < coapTempBufferSize + bufferSize)
            {
                if( coapTempBuffer == NULL )
                {
                    coapTempBufferCapacity = bufferSize;
                    coapTempBuffer = (uint8_t*) malloc(coapTempBufferCapacity);
                }
                else
                {
                    coapTempBufferCapacity = coapTempBufferCapacity + bufferSize;
                    coapTempBuffer = (uint8_t*) realloc(coapTempBuffer, coapTempBufferCapacity);
                }
            }        

            memcpy(coapTempBuffer+coapTempBufferSize, buffer, bufferSize);
            coapTempBufferSize += bufferSize;
        }
        else
        {  
            if( coapTempBuffer == NULL )
            {
                coapTempBuffer = buffer;    
                coapTempBufferSize = bufferSize;
            }
            else
            {
                if(coapTempBufferCapacity < coapTempBufferSize + bufferSize)
                {
                    if( coapTempBuffer == NULL )
                    {
                        coapTempBufferCapacity = bufferSize;
                        coapTempBuffer = (uint8_t*) malloc(coapTempBufferCapacity);
                    }
                    else
                    {
                        coapTempBufferCapacity = coapTempBufferCapacity + bufferSize;
                        coapTempBuffer = (uint8_t*) realloc(coapTempBuffer, coapTempBufferCapacity);
                    }
                }        

                memcpy(coapTempBuffer+coapTempBufferSize, buffer, bufferSize);
                coapTempBufferSize += bufferSize;
            }

            UMLRTCommunicator::Message* msg = UMLRTCommunicator::Message::decode((uint8_t*)coapTempBuffer);
            incomingCoapMessages.enqueue(msg);
            if(coapTempBuffer != buffer)
                free(coapTempBuffer);

            coapTempBuffer = NULL;
            coapTempBufferCapacity = 0;
            coapTempBufferSize = 0;
        }
    } 
    else
    {
        UMLRTCommunicator::Message* msg = UMLRTCommunicator::Message::decode((uint8_t*)buffer);
        incomingCoapMessages.enqueue(msg);
    }
}

static void coapResponseReceivedCB(struct coap_context_t *ctx,
                coap_session_t *session,
                coap_pdu_t *sent,
                coap_pdu_t *received,
                const coap_tid_t id) {

    printf("Got response\n!!");
}

UMLRTCOAPCommunicator::UMLRTCOAPCommunicator ( const char * _localaddr ) 
    : UMLRTCommunicator(_localaddr)
{
    hostSessionMap = new UMLRTHashMap("coapHostSessionMap", UMLRTHashMap::compareString, false);

    char* protocol = strtok(strdup(_localaddr), ":"); // coap://ip:port
    if( !protocol || strcmp(protocol, "coap") != 0 )
        FATAL("Invalid CoAP address: %s\n", _localaddr);
    
    char* ip = strtok(NULL, ":"); // //ip:port
    if( !ip )
        FATAL("Invalid CoAP address: %s\n", _localaddr);
    ip+=2; // remove leading //

    char* port = strtok(NULL, ":"); //:port
    if( !port )
        FATAL("Invalid CoAP address: %s\n", _localaddr);

    coapServer = new UMLRTCoapServer( "CoAP Server" );
    coap_context_t* context = coapServer->init(ip, atoi(port));
    coapServer->start(context);

    coap_address_t sourceAddress;
    coap_address_init(&sourceAddress);
	sourceAddress.addr.sin.sin_family = AF_INET;
	sourceAddress.addr.sin.sin_port = htons(0);
	sourceAddress.addr.sin.sin_addr.s_addr = inet_addr(_localaddr);

    coapContext = coap_new_context(&sourceAddress);
    coap_register_response_handler(coapContext, coapResponseReceivedCB);
}   

void UMLRTCOAPCommunicator::sendBuffer ( UMLRTHost * host, uint8_t* buffer, int bufferSize )
{
    coap_session_t* session = (coap_session_t*) hostSessionMap->getObject(host->name);
    if( session == NULL )
        FATAL("No CoAP session with host %s\n", host->name);

    int left = bufferSize;
    coap_block_t block = { .num = 0, .m = 1, .szx = 6 };
    unsigned char buf[4];

    while( left > 0 )
    {
        coap_pdu_t *request = coap_new_pdu(session);
        request->tid = coap_new_message_id(session);
        request->type = COAP_MESSAGE_NON;
        request->code = COAP_REQUEST_POST;
    
       if( left <= (1 << (block.szx + 4)) )
          block.m = 0;
          
        coap_add_option(request, COAP_OPTION_BLOCK1,
            coap_encode_var_bytes(buf, (block.num << 4) | (block.m << 3) | block.szx), 
            buf);
       
        coap_add_block(request, bufferSize, buffer, block.num, block.szx);    
        left -= (1 << (block.szx + 4));
        block.num++;

        if( coap_send(session, request) == COAP_INVALID_TID )
            FATAL("Error sending message to host %s\n", host->name);
    }
}

void UMLRTCOAPCommunicator::sendDeployment ( UMLRTHost * host )
{
    uint8_t* buffer;
    int bufferSize = UMLRTCommunicator::DeploymentMessage::make(localhost, host, &buffer);
    sendBuffer(host, buffer, bufferSize);
    free(buffer);
}


void UMLRTCOAPCommunicator::waitForDeployment ( )
{
    while( incomingCoapMessages.count() == 0 )
        usleep(100);
    
    UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*)incomingCoapMessages.dequeue();    
    UMLRTDeploymentMap::decode( msg->payload, msg->payloadSize );
    delete msg;
}

void UMLRTCOAPCommunicator::sendReadySignal ( UMLRTHost * host )
{
    uint8_t* buffer;
    int bufferSize = UMLRTCommunicator::ReadyMessage::make(localhost, host, &buffer);
    sendBuffer(host, buffer, bufferSize); 
    free(buffer);
}

void UMLRTCOAPCommunicator::waitForReadySignal ( UMLRTHost * host )
{
    while( incomingCoapMessages.count() == 0 )
        usleep(100);
    
    UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*)incomingCoapMessages.dequeue();    
    delete msg;
}

void UMLRTCOAPCommunicator::connect ( UMLRTHost * host )
{
    char* protocol = strtok(strdup(host->address), ":");
    char* ip = strtok(NULL, ":"); 
    char* port = strtok(NULL, ":");
    ip+=2;

    coap_address_t destinationAddress;
    coap_address_init(&destinationAddress);
	destinationAddress.addr.sin.sin_family = AF_INET;
	destinationAddress.addr.sin.sin_port = htons(atoi(port));
	destinationAddress.addr.sin.sin_addr.s_addr = inet_addr(ip);
    coap_session_t* session = coap_new_client_session(coapContext, NULL, &destinationAddress, COAP_PROTO_UDP);
    hostSessionMap->insert(host->name, (void*)session);
    host->connected = true;
}

void UMLRTCOAPCommunicator::disconnect ( UMLRTHost * host )
{

	host->connected = false;
}


UMLRTCommunicator::Message* UMLRTCOAPCommunicator::sendrecv()
{
	UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*) messageQueue.dequeue();
	if(msg != NULL)
	{
        uint8_t* buffer;
        int bufferSize = msg->encode(&buffer);

        UMLRTHost* destination = UMLRTDeploymentMap::getHostFromName(msg->receiver);
        if( destination == NULL )
            FATAL("Destination host %s not found\n", msg->receiver);

        sendBuffer(destination, buffer, bufferSize);
        //free(buffer);
		delete msg;
	}

	if(incomingCoapMessages.count() > 0)
		return (UMLRTCommunicator::Message*)incomingCoapMessages.dequeue();
	return NULL;
}


coap_context_t* UMLRTCOAPCommunicator::UMLRTCoapServer::init ( const char * ip, int port )
{
    coap_context_t* context;
    coap_address_t serverAddress;
    coap_address_init(&serverAddress);
    serverAddress.addr.sin.sin_family = AF_INET;
    serverAddress.addr.sin.sin_addr.s_addr = inet_addr(ip);
    serverAddress.addr.sin.sin_port = htons(port);

    context = coap_new_context(&serverAddress);
    if( !context )
        FATAL("Error creating CoAP context\n");

    coap_resource_t* root = coap_resource_init(NULL, 0, 0);
    coap_register_handler(root, COAP_REQUEST_POST, coapRequestReceivedCB);
    coap_add_resource(context, root);
    return context;
}

void * UMLRTCOAPCommunicator::UMLRTCoapServer::run ( void * args )
{
    coap_context_t* context = (coap_context_t*) args;
    
    unsigned wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;

    while ( true ) {
        int result = coap_run_once(context, wait_ms);
        if ( result < 0 ) {
            break;
        } else if ( (unsigned)result < wait_ms ) {
            wait_ms -= result;
        } else {
            wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;
        }
    }
}
