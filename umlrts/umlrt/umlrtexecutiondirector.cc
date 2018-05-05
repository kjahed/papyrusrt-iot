// umlrtexecutiondirector.cc

#include "umlrtexecutiondirector.hh"
#include "umlrtdeploymentmap.hh"
#include "umlrtframeservice.hh"
#include "umlrtcommsport.hh"
#include "umlrtcommsportfarend.hh"
#include "umlrtcommunicator.hh"
#include "umlrtnanocommunicator.hh"
#include "umlrtmqttcommunicator.hh"
#include "umlrtcoapcommunicator.hh"
#include "umlrtddscommunicator.hh"
#include "umlrtcapsule.hh"
#include "umlrtcapsulepart.hh"
#include "umlrtcontroller.hh"
#include "umlrtoutsignal.hh"
#include "umlrtslot.hh"
#include "umlrtsignalregistry.hh"
#include "umlrthost.hh"
#include "basedebug.hh"
#include "basefatal.hh"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

UMLRTHost * UMLRTExecutionDirector::localhost;

static int total_msgs_sent = 0;
static int total_msgs_recv = 0;
static double total_decoding_time = 0;
static double total_encoding_time = 0;
static double total_msg_size = 0;

int UMLRTExecutionDirector::total_messages_encoded = 0;
int UMLRTExecutionDirector::total_size = 0;

void sigIntHandler(int dummy) {
    double avg_decoding_time = total_decoding_time / total_msgs_recv;
    double avg_encoding_time = total_encoding_time / total_msgs_sent;
    double avg_msg_size = (double)UMLRTExecutionDirector::total_size / (double)UMLRTExecutionDirector::total_messages_encoded;

    printf("Messages sent: %d\nMessages received: %d\nAvg decoding time: %.2fns\nAvg encoding time: %.2fns\nAvg message size: %.2f", total_msgs_sent, total_msgs_recv, avg_decoding_time, avg_encoding_time, avg_msg_size);
    exit(0);
}

/*static*/ UMLRTExecutionDirector* UMLRTExecutionDirector::instance = NULL;

/*static*/ void UMLRTExecutionDirector::init ( const char * localaddr )
{
	UMLRTExecutionDirector::instance = new UMLRTExecutionDirector(localaddr);
}

UMLRTExecutionDirector::UMLRTExecutionDirector ( const char * _localaddr )
	: UMLRTBasicThread("ExecutionDirector"), localaddr(_localaddr), abort(false)
{
    slotsCount = UMLRTDeploymentMap::getDefaultSlotList(&slots);
}    

/*static*/ bool UMLRTExecutionDirector::sendSignal ( const UMLRTCommsPort * destPort, const UMLRTSignal &signal, size_t srcPortIndex )
{
    UMLRTCommunicator::Signal sig;
    sig.destinationSlot = destPort->slot->name;
    sig.sourceSlot = signal.getSrcPort()->slot->name;
    sig.destinationPort = destPort->roleIndex;
    sig.sourcePort = srcPortIndex;
    sig.destinationInternal = destPort->border ? 0 : 1;
    sig.protocolName = signal.getSrcPort()->role()->protocol;
    sig.signalName = signal.getName();
    sig.payloadSize = signal.getPayloadSize();
    sig.payload = signal.getPayload();

    uint8_t* buffer;
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    long begin = tp.tv_sec * 1000000000 + tp.tv_nsec;
    int bufferSize = sig.encode(&buffer);
    clock_gettime(CLOCK_REALTIME, &tp);
    long end = tp.tv_sec * 1000000000 + tp.tv_nsec;
    total_encoding_time += end - begin;
    total_msg_size += bufferSize;

    UMLRTCommunicator::Message * msg = 
        UMLRTCommunicator::ProtocolMessage::make(localhost, destPort->slot->host);
    msg->payload = buffer;
    msg->payloadSize = bufferSize;

//    UMLRTHost* endpoint = UMLRTDeploymentMap::getHostForPort(destPort->slot->name, destPort->getName());
//    if( endpoint != NULL ) //proxyport
//    {
//        if( endpoint->isMQTT() )
//        {
//            msg->mqttBroker = endpoint->address;
//            msg->mqttTopic = signal.getSrcPort()->role()->protocol;
//            instance->communicators[COMMUNICATOR::MQTT]->queueMessage(msg);
//        }
//    }
//    else if( destPort->slot->host->isMQTT() )
//        instance->communicators[COMMUNICATOR::MQTT]->queueMessage( msg );
//    else
//        instance->communicators[COMMUNICATOR::NANO]->queueMessage( msg );    
//    else
      instance->communicator->queueMessage(msg);
      total_msgs_sent++;
    return true;
}

/*static*/void UMLRTExecutionDirector::spawn ( )
{
	instance->spawnThread();
}


/*static*/void  UMLRTExecutionDirector::join ( )
{
	instance->joinThread();
}

void  UMLRTExecutionDirector::spawnThread ( )
{
	start(NULL);
}

void  UMLRTExecutionDirector::joinThread ( )
{
	UMLRTBasicThread::join();
}

void  UMLRTExecutionDirector::initProxyPorts ( UMLRTSlot& slot )
{
    UMLRTMQTTCommunicator* mqttCommunicator = (UMLRTMQTTCommunicator*) communicators[COMMUNICATOR::MQTT];

    for( int i=0; i<slot.numPorts; i++ ) 
    {
        UMLRTCommsPort port = slot.ports[i];
        UMLRTHost* endpoint = UMLRTDeploymentMap::getHostForPort(slot.name, port.getName());
        if( endpoint != NULL ) //proxyport
        {
            if( endpoint->isMQTT() ) 
            {
                mqttCommunicator->connect(endpoint);
                mqttCommunicator->subscribe(endpoint, port.role()->protocol);
            }
        }
    }
}

void  UMLRTExecutionDirector::deploy ( )
{
    for( int i=0; i<slotsCount; i++ )
    {
        // [FIXME] we should only init capsules of non-remote slots!!
        slots[i].capsuleClass->create(&slots[i]);

        const char* controllerName = UMLRTDeploymentMap::getControllerNameForCapsule(slots[i].name);
        if( controllerName != NULL )
        {
            UMLRTHost * host = UMLRTDeploymentMap::getHostForController(controllerName);
            if ( host == NULL )
                FATAL( "Missing host for controller %s", controllerName);

            if( localhost == host )
            {
                UMLRTController* controller = UMLRTDeploymentMap::getControllerFromName(controllerName);
                if( controller == NULL )
                    controller = new UMLRTController( (char*)controllerName );

                slots[i].controller = controller;
                slots[i].host = localhost;
                initProxyPorts(slots[i]);
            }
            else
            {
                slots[i].remote = true;
                slots[i].host = host;
            }

            //initProxyPorts(slots[i]);
        }
    }
}

void * UMLRTExecutionDirector::runLocal ( void * args )
{
    UMLRTController* defaultController = NULL;

    for( int i=0; i<slotsCount; i++ )
    {
        slots[i].capsuleClass->create(&slots[i]);

        const char* controllerName = UMLRTDeploymentMap::getControllerNameForCapsule(slots[i].name);
        if( controllerName != NULL )
        {
            UMLRTController* controller = UMLRTDeploymentMap::getControllerFromName(controllerName);
            if( controller == NULL )
                controller = new UMLRTController( (char*)controllerName );

            slots[i].controller = controller;
        }
        else
        {
            if( defaultController == NULL)
                defaultController = new UMLRTController( "DefaultController" );

            slots[i].controller = defaultController;
        }
    }

    UMLRTDeploymentMap::spawnAllControllers();
    UMLRTDeploymentMap::joinAllControllers();
    return NULL;
}

void * UMLRTExecutionDirector::run ( void * args )
{
    signal(SIGINT, sigIntHandler);

    // if no address was provided, this is a local single threaded run
    if(localaddr == NULL)
        return runLocal(args);

    //communicators[COMMUNICATOR::NANO] = new UMLRTNanoCommunicator(localaddr);
    //communicators[COMMUNICATOR::MQTT] = new UMLRTMQTTCommunicator(localaddr);
    //communicators[COMMUNICATOR::COAP] = new UMLRTCOAPCommunicator(localaddr);
    if( strncmp(localaddr, "mqtt", 4) == 0)
        communicator = new UMLRTMQTTCommunicator(localaddr);
    else if( strncmp(localaddr, "coap", 4) == 0 )
        communicator = new UMLRTCOAPCommunicator(localaddr);
    else if( strncmp(localaddr, "dds", 3) == 0 )
        communicator = new UMLRTDDSCommunicator(localaddr);
    else
        communicator = new UMLRTNanoCommunicator(localaddr);

    // client mode: wait for parent to send deployment
    if( !UMLRTDeploymentMap::isLoaded() )
        communicator->waitForDeployment();

    localhost = UMLRTDeploymentMap::getHostFromAddress( localaddr );
    if( localhost == NULL )
        FATAL( "Local host with address '%s' not found in deployment plan", localaddr );
   
    //for(int i=0; i<3; i++)
    //    communicators[i]->setLocalHost(localhost);
    communicator->setLocalHost(localhost);
    deploy();

    // send deployment to children
    for( int i=0; i<slotsCount; i++ )
    {
        if( !slots[i].remote )
        {
            for( int j=0; j<slots[i].numParts; j++ )
            {
                for( int k=0; k<slots[i].parts[j].numSlot; k++ )
                {
                    UMLRTSlot * childSlot = slots[i].parts[j].slots[k];
                    
                    if( childSlot->remote )
                    {
                        UMLRTHost * childHost = childSlot->host;

                         if( !childHost->connected )
                            communicator->connect(childHost);
                            
                         if( !childHost->deployed )
                         {
                            communicator->sendDeployment( childHost );
                            communicator->waitForReadySignal( childHost );
                         }
                    }
                }
            }
        }
    }

    // ack deployment
    for( int i=0; i<slotsCount; i++ )
    {
        if( slots[i].remote )
        {
            UMLRTHost * parentHost = slots[i].host;

            for( int j=0; j<slots[i].numParts; j++ )
            {
                for( int k=0; k<slots[i].parts[j].numSlot; k++ )
                {
                    UMLRTSlot * childSlot = slots[i].parts[j].slots[k];
                    
                    if( !childSlot->remote )
                    {
                         if( !parentHost->connected )
                            communicator->connect( parentHost );
                        
                         if( !parentHost->signaled )
                            communicator->sendReadySignal( parentHost);
                    }
                }
            }
        }
    }

    for( int i=0; i<slotsCount; i++ )
    {
        if ( !slots[i].remote )
        {
            //connect to remote ports
            for ( int j=0; j<slots[i].numPorts; j++ )
            {
                for(int k=0; k < slots[i].ports[j].numFarEnd; k++)
                {
                    if(slots[i].ports[j].farEnds[k].port != NULL)
                    {
                        UMLRTSlot * farEndSlot = slots[i].ports[j].farEnds[k].port->slot;
                        if( farEndSlot->remote )
                        {
                            if( !farEndSlot->host->connected )
                                communicator->connect(farEndSlot->host);
                        }
                    }
                }
            }
        }
        else
        {
            //connect to remote ports
            for ( int j=0; j<slots[i].numPorts; j++ )
            {
                for(int k=0; k < slots[i].ports[j].numFarEnd; k++)
                {
                    if(slots[i].ports[j].farEnds[k].port != NULL)
                    {
                        UMLRTSlot * farEndSlot = slots[i].ports[j].farEnds[k].port->slot;
                        if( !farEndSlot->remote )
                        {
                            if( !slots[i].host->connected )
                                communicator->connect(slots[i].host);
                        }
                    }
                }
            }
        }
    }

    UMLRTDeploymentMap::spawnAllControllers();

    while(!abort)
    {
        for(int i=0; i<1; i++)
        {
            UMLRTCommunicator::Message * msg = communicator->sendrecv();//communicators[i]->sendrecv();
            if(msg == NULL)
            {
                usleep(100);
                continue;
            }

            total_msgs_recv++;

            struct timespec tp;
            clock_gettime(CLOCK_REALTIME, &tp);
            long begin = tp.tv_sec * 1000000000 + tp.tv_nsec;
            UMLRTCommunicator::Signal * sig = UMLRTCommunicator::Signal::decode(msg->payload);
            clock_gettime(CLOCK_REALTIME, &tp);
            long end = tp.tv_sec * 1000000000 + tp.tv_nsec;
            total_decoding_time += end - begin;

            UMLRTSlot* destSlot = UMLRTDeploymentMap::getSlotFromName(sig->destinationSlot);
            if(destSlot == NULL)
             FATAL("Invalid destination slot %s", sig->destinationSlot);

            UMLRTSlot* srcSlot = UMLRTDeploymentMap::getSlotFromName(sig->sourceSlot);
            if(srcSlot == NULL)
             FATAL("Invalid source slot %s", sig->sourceSlot);

            int signalID = UMLRTSignalRegistry::getRegistry().getInSignalID(sig->protocolName, sig->signalName);
            if(signalID == -1)
             FATAL("Unable to resolve signal %s of protocol %s\n", sig->signalName, sig->protocolName);

            UMLRTObject * payloadObj = UMLRTSignalRegistry::getRegistry().getInSignalPayloadObject(sig->protocolName, sig->signalName);
            if(payloadObj->sizeOf == 0)
             payloadObj = NULL;

            UMLRTOutSignal* signal = new UMLRTOutSignal;
            if(payloadObj != NULL)
                signal->initialize (sig->signalName, signalID, &srcSlot->ports[sig->sourcePort], payloadObj, sig->payload);
            else
                signal->initialize (sig->signalName, signalID, &srcSlot->ports[sig->sourcePort]);

           if(destSlot->capsule == NULL)
                FATAL("No capsule in destination slot");

            if(destSlot->controller == NULL)
                FATAL("No controller for capsule %s", destSlot->capsule->getName());

            const UMLRTCommsPort * destPort;
            if(sig->destinationInternal)
                destPort = destSlot->capsule->getInternalPorts()[sig->destinationPort];
            else
                destPort = destSlot->capsule->getBorderPorts()[sig->destinationPort];

            bool ok = destSlot->controller->deliver(destPort, *signal,  sig->sourcePort);
            if(!ok)
                FATAL("Error delivering signal to controller");

            delete sig;
            delete msg;
        } 
    }

    UMLRTDeploymentMap::joinAllControllers();

	return NULL;
}
