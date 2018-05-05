// umlrtmqttcommunicator.cc

/*******************************************************************************

*******************************************************************************/

// UMLRTMQTTCommunicator manages all MQTT communication.

#include "basedebug.hh"
#include "basefatal.hh"
#include "umlrtqueue.hh"
#include "umlrtmqttcommunicator.hh"
#include "umlrthost.hh"

#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <time.h>

#include "mqtt/MQTTClient.h"

#define MQTT_BROKER "mqtt://127.0.0.1:1883"
#define MQTT_MAIN_TOPIC "papyrusrt/#"
#define MQTT_DEPLOYMENT_TOPIC "papyrusrt/deployment"
#define MQTT_READY_TOPIC "papyrusrt/ready"
#define MQTT_SIGNAL_TOPIC "papyrusrt/signal"

static UMLRTQueue incomingMqttMessages;
static int mqttMessageReceivedCB(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{

    UMLRTCommunicator* comm = (UMLRTCommunicator*)context;
    UMLRTCommunicator::Message* msg = UMLRTCommunicator::Message::decode((uint8_t*)message->payload);

    if( strcmp(topicName, MQTT_DEPLOYMENT_TOPIC) != 0
            && strcmp(msg->receiver, comm->getLocalHost()->name) != 0 )
        delete msg;
    else if( strcmp(topicName, MQTT_DEPLOYMENT_TOPIC) == 0 
        && UMLRTDeploymentMap::isLoaded())
        delete msg;
    else
        incomingMqttMessages.enqueue(msg);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

static char* mqttGenerateClientID(const int len) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);

	char* s = (char*) malloc(sizeof(char) * len);
    if(s == NULL)
        return NULL;

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
	return s;
}


UMLRTMQTTCommunicator::UMLRTMQTTCommunicator ( const char * _localaddr ) 
    : UMLRTCommunicator(_localaddr)
{
    clientMap = new UMLRTHashMap("mqttClientMap", UMLRTHashMap::compareString, false);
    connect(MQTT_BROKER);
}   

void UMLRTMQTTCommunicator::sendDeployment ( UMLRTHost * host )
{
    uint8_t* buffer;
    int bufferSize = UMLRTCommunicator::DeploymentMessage::make(localhost, host, &buffer);
    
    MQTTClient* client = (MQTTClient*) clientMap->getObject(MQTT_BROKER);
    if(client == NULL)
        FATAL("MQTT client for host %s not found", MQTT_BROKER);
    
    MQTTClient_publish(*client, MQTT_DEPLOYMENT_TOPIC, bufferSize, buffer, 0, 0, NULL);
    free(buffer);
}


void UMLRTMQTTCommunicator::waitForDeployment ( )
{
    while( incomingMqttMessages.count() == 0 )
        usleep(100);
    
    UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*)incomingMqttMessages.dequeue();    
    UMLRTDeploymentMap::decode( msg->payload, msg->payloadSize );
    delete msg;
}

void UMLRTMQTTCommunicator::sendReadySignal ( UMLRTHost * host )
{
    uint8_t* buffer;
    int bufferSize = UMLRTCommunicator::ReadyMessage::make(localhost, host, &buffer);
    
    MQTTClient* client = (MQTTClient*) clientMap->getObject(MQTT_BROKER);
    if(client == NULL)
        FATAL("MQTT client for host %s not found", MQTT_BROKER);
    
    MQTTClient_publish(*client, MQTT_READY_TOPIC, bufferSize, buffer, 0, 0, NULL);
    free(buffer);
}

void UMLRTMQTTCommunicator::waitForReadySignal ( UMLRTHost * host )
{
    while( incomingMqttMessages.count() == 0 )
        usleep(100);
    
    UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*)incomingMqttMessages.dequeue();    
    delete msg;
}

void UMLRTMQTTCommunicator::connect ( UMLRTHost * host )
{
//    MQTTClient* client = (MQTTClient*) clientMap->getObject(host->address);
//    if(client != NULL)
//        return;
//
//    char* address = strdup(host->address); // mqtt://... 
//    address += 1; // qtt://...
//    strncpy(address, "tcp", 3); // tcp://...
//
//    client = new MQTTClient;
//    MQTTClient_create(client, address, mqttGenerateClientID(16),
//        MQTTCLIENT_PERSISTENCE_NONE, NULL);
//
//    MQTTClient_setCallbacks(*client, this, NULL, mqttMessageReceivedCB, NULL);
//
//    MQTTClient_connectOptions connOpts = MQTTClient_connectOptions_initializer;
//    connOpts.keepAliveInterval = 20;
//    connOpts.cleansession = 1;
//
//    int rc;
//    if ( (rc = MQTTClient_connect(*client, &connOpts)) != MQTTCLIENT_SUCCESS )
//        FATAL("Unable to connect to broker @ %s, err: %d\n", address, rc);
//
//    clientMap->insert(host->address, (void*)client);
//    host->connected = true;
}

void UMLRTMQTTCommunicator::connect ( const char * addrs )
{

    char* address = strdup(addrs); // mqtt://... 
    address += 1; // qtt://...
    strncpy(address, "tcp", 3); // tcp://...

    MQTTClient* client = new MQTTClient;
    MQTTClient_create(client, address, mqttGenerateClientID(16),
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTClient_setCallbacks(*client, this, NULL, mqttMessageReceivedCB, NULL);

    MQTTClient_connectOptions connOpts = MQTTClient_connectOptions_initializer;
    connOpts.keepAliveInterval = 20;
    connOpts.cleansession = 1;

    int rc;
    if ( (rc = MQTTClient_connect(*client, &connOpts)) != MQTTCLIENT_SUCCESS )
        FATAL("Unable to connect to broker @ %s, err: %d\n", address, rc);

    MQTTClient_subscribe(*client, MQTT_MAIN_TOPIC, 0);
    clientMap->insert(addrs, (void*)client);
}

void UMLRTMQTTCommunicator::disconnect ( UMLRTHost * host )
{
    MQTTClient* client = (MQTTClient*) clientMap->getObject(host->address);
    if( client != NULL ) {
        MQTTClient_disconnect(*client, 1000);
        MQTTClient_destroy(client);
        clientMap->remove(host->address);
    }

	host->connected = false;
}

void UMLRTMQTTCommunicator::subscribe( UMLRTHost* host, const char* topic )
{
    MQTTClient* client = (MQTTClient*) clientMap->getObject(host->address);
    if(client == NULL)
        FATAL("MQTT client for host %s not found", host->address);
    MQTTClient_subscribe(*client, topic, 0);
}

void UMLRTMQTTCommunicator::publish( UMLRTHost* host, const char* topic , uint8_t* buffer, int bufferSize )
{
    MQTTClient* client = (MQTTClient*) clientMap->getObject(host->address);
    if(client == NULL)
        FATAL("MQTT client for host %s not found", host->address);
    MQTTClient_publish(*client, topic, bufferSize, buffer, 0, 0, NULL);
}

UMLRTCommunicator::Message* UMLRTMQTTCommunicator::sendrecv()
{
	UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*) messageQueue.dequeue();
	if(msg != NULL)
	{
        uint8_t* buffer;
        int bufferSize = msg->encode(&buffer);

        MQTTClient* client = (MQTTClient*) clientMap->getObject(MQTT_BROKER);
        if(client == NULL)
            FATAL("MQTT client for host %s not found", MQTT_BROKER);

        MQTTClient_publish(*client, MQTT_SIGNAL_TOPIC, bufferSize, buffer, 0, 0, NULL);
		delete msg;
	}

	if(incomingMqttMessages.count() > 0)
		return (UMLRTCommunicator::Message*)incomingMqttMessages.dequeue();
	return NULL;
}
