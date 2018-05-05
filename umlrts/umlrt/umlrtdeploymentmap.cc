// umlrtdeploymentmap.cc

/*******************************************************************************

*******************************************************************************/

#include "umlrtcapsule.hh"
#include "umlrtdeploymentmap.hh"
#include "umlrtcontroller.hh"
#include "umlrtslot.hh"
#include "umlrthost.hh"
#include "umlrthashmap.hh"
#include "basefatal.hh"
#include "basedebug.hh"
#include "basedebugtype.hh"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "osutil.hh"

//#include "flatbuffers/flatbuffers.h"
//#include "flatbuffers/idl.h"
//#include "flatbuffers/util.h"
//#include "fbschemas/deployment_generated.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;


UMLRTHashMap * UMLRTDeploymentMap::capsuleToControllerMap = NULL;
UMLRTHashMap * UMLRTDeploymentMap::controllerToHostMap = NULL;
UMLRTHashMap * UMLRTDeploymentMap::capsuleToPortHostMap = NULL;
UMLRTHashMap * UMLRTDeploymentMap::controllerNameMap = NULL;
UMLRTHashMap * UMLRTDeploymentMap::capsuleNameMap = NULL;
UMLRTHashMap * UMLRTDeploymentMap::hostNameMap = NULL;
UMLRTHashMap * UMLRTDeploymentMap::slotNameMap = NULL;

UMLRTSlot * UMLRTDeploymentMap::defaultSlotList = NULL;
int UMLRTDeploymentMap::numDefaultSlotList = 0;

uint8_t * UMLRTDeploymentMap::payload = NULL;
int UMLRTDeploymentMap::payloadSize = 0;

// The 3 maps above are created dynamically and persist forever.
// capsuleToControllerMap
//   - created during the reading of the top.controllers file.
//   - 'objects' in capsuleToControllerMap are maps themselves (and also persist forever).
//   - these 'object-maps' will map a 'capsule class' to a 'controller' for an individual capsule instance.
//   - these 'object-maps' will generally contain one element with key == NULL, indicating the default controller for all capsule classes
//     (for the capsule instance) not explicitly mapped elsewhere in the map.
//   - one 'object' in each map 'capsuleToControllerMap' have
// controllerNameMap
//   - is created (and populated) during controller construction and elements persist forever (as dynamic controller creation/deletion is not supported).
// capsuleNameMap
//   - is created (and populated) during capsule construction. Elements are deleted as capsules are destroyed.
//   - elements associated with the static sub-structure of Top persist forever (as these capsules are never destroyed).

/*static*/ UMLRTHashMap * UMLRTDeploymentMap::getControllerNameMap()
{
    if (controllerNameMap == NULL)
    {
        controllerNameMap = new UMLRTHashMap("controllerNameMap", UMLRTHashMap::compareString, false/*objectIsString*/);
    }
    return controllerNameMap;
}

/*static*/ UMLRTHashMap * UMLRTDeploymentMap::getCapsuleToControllerMap()
{
    if (capsuleToControllerMap == NULL)
    {
        capsuleToControllerMap = new UMLRTHashMap("capsuleToControllerMap", UMLRTHashMap::compareString, false/*objectIsString*/);
    }
    return capsuleToControllerMap;
}

/*static*/ UMLRTHashMap * UMLRTDeploymentMap::getControllerToHostMap()
{
    if (controllerToHostMap == NULL)
    {
        controllerToHostMap = new UMLRTHashMap("controllerToHostMap", UMLRTHashMap::compareString, false/*objectIsString*/);
    }
    return controllerToHostMap;
}


/*static*/ UMLRTHashMap * UMLRTDeploymentMap::getCapsuleToPortHostMap()
{
    if (capsuleToPortHostMap == NULL)
    {
        capsuleToPortHostMap = new UMLRTHashMap("capsuleToHostPortMap", UMLRTHashMap::compareString, false/*objectIsString*/);
    }
    return capsuleToPortHostMap;
}

/*static*/ UMLRTHashMap * UMLRTDeploymentMap::getCapsuleNameMap()
{
    if (capsuleNameMap == NULL)
    {
        capsuleNameMap = new UMLRTHashMap("capsuleNameMap", UMLRTHashMap::compareString, false/*objectIsString*/);
    }
    return capsuleNameMap;
}

/*static*/ UMLRTHashMap * UMLRTDeploymentMap::getHostNameMap()
{
    if (hostNameMap == NULL)
    {
        hostNameMap = new UMLRTHashMap("hostNameMap", UMLRTHashMap::compareString, false/*objectIsString*/);
    }
    return hostNameMap;
}

/*static*/ UMLRTHashMap * UMLRTDeploymentMap::getSlotNameMap()
{
    if (slotNameMap == NULL)
    {
        slotNameMap = new UMLRTHashMap("slotNameMap", UMLRTHashMap::compareString, false/*objectIsString*/);
    }
    return slotNameMap;
}

// Add a controller by name.
/*static*/ bool UMLRTDeploymentMap::addController( const char * controllername, UMLRTController * controller )
{
    if (getControllerFromName(controllername) != NULL)
    {
        FATAL("ERROR: Controller %s has already registered.", controllername);
    }
    BDEBUG(BD_CONTROLLER, "add controller(%s) p(%p)\n", controllername, controller );

    getControllerNameMap()->insert( controllername, (void *)controller);

    return true;
}

// Add a capsule by name.
/*static*/ void UMLRTDeploymentMap::addCapsule( const char * capsulename, UMLRTCapsule * capsule )
{
    getCapsuleNameMap()->insert( capsulename, (void *)capsule);
}

// Add a controller by name.
/*static*/ bool UMLRTDeploymentMap::addHost( const char * hostName, UMLRTHost * host )
{
    if (getControllerFromName(hostName) != NULL)
    {
        FATAL("ERROR: Controller %s has already registered.", hostName);
    }
    BDEBUG(BD_CONTROLLER, "add host(%s) p(%p)\n", hostName, host );

    getHostNameMap()->insert( hostName, (void *)host);

    return true;
}

// Add a slot by name.
/*static*/  bool UMLRTDeploymentMap::addSlot ( const char * slotName, UMLRTSlot * slot )
{
    if (getSlotFromName(slotName) != NULL)
    {
        FATAL("ERROR: Slot %s has already registered.", slotName);
    }
    BDEBUG(BD_CONTROLLER, "add slot(%s) p(%p)\n", slotName, slot );

    getSlotNameMap()->insert( slotName, (void *)slot);

    return true;
}

/*static*/ const UMLRTCapsule * UMLRTDeploymentMap::getCapsuleFromName ( const char * capsulename )
{
    return (const UMLRTCapsule *)getCapsuleNameMap()->getObject(capsulename);
}

/*static*/ UMLRTController * UMLRTDeploymentMap::getControllerFromName( const char * controllerName )
{
    return (UMLRTController *)getControllerNameMap()->getObject(controllerName);
}

/*static*/ int UMLRTDeploymentMap::getDefaultSlotList ( UMLRTSlot * * slot_p )
{
    *slot_p = defaultSlotList;

    return numDefaultSlotList;
}

/*static*/ UMLRTHost * UMLRTDeploymentMap::getHostFromName( const char * hostName )
{
    return (UMLRTHost *)getHostNameMap()->getObject(hostName);
}

/*static*/ UMLRTHost * UMLRTDeploymentMap::getHostFromAddress( const char * hostAddress )
{
    getHostNameMap()->lock();
    UMLRTHashMap::Iterator iter = getHostNameMap()->getIterator();

    while (iter != iter.end())
    {
        UMLRTHost * host = (UMLRTHost *)((char *)iter.getObject());
        if( strcmp(hostAddress, host->address) == 0 )
        {
            getHostNameMap()->unlock();
            return host;
        }

        iter = iter.next();
    }
    getHostNameMap()->unlock();
    return NULL;
}

/*static*/ UMLRTSlot * UMLRTDeploymentMap::getSlotFromName( const char * slotName )
{
    return (UMLRTSlot *)getSlotNameMap()->getObject(slotName);
}

/*static*/ UMLRTController * UMLRTDeploymentMap::getFirstController()
{
    return (UMLRTController *)getControllerNameMap()->getFirstObject();
}

/*static*/ const char * UMLRTDeploymentMap::getControllerNameForCapsule ( const char * capsuleName )
{
    void * controllerName = getCapsuleToControllerMap()->getObject(capsuleName);
    if (controllerName != NULL)
        return (const char *) controllerName;

    return NULL;
}

// Return the controller assigned to this capsule. Returns NULL if no controller is assigned or doesn't exist.
/*static*/ UMLRTController * UMLRTDeploymentMap::getControllerForCapsule ( const char * capsuleName )
{
    const char * controllerName = getControllerNameForCapsule( capsuleName );

    BDEBUG(BD_CONTROLLERMAP, "getControllerForCapsule: capsule(%s)  map-controller(%s)\n",
            capsuleName,
            controllerName ? controllerName : "-no controller-");

    return (UMLRTController *)getControllerFromName(controllerName);
}

/*static*/ UMLRTHashMap* UMLRTDeploymentMap::getPortToHostMapForCapsule ( const char * capsuleName )
{
    void * map = getCapsuleToPortHostMap()->getObject(capsuleName);
    if (map != NULL)
        return (UMLRTHashMap*) map;

    return NULL;
}

/*static*/ void UMLRTDeploymentMap::enqueueAbortAllControllers ( )
{
    getControllerNameMap()->lock();
    UMLRTHashMap::Iterator iter = getControllerNameMap()->getIterator();

    while (iter != iter.end())
    {
        UMLRTController * controller = (UMLRTController *)iter.getObject();
        BDEBUG(BD_CONTROLLERMAP, "enqueueAbortAllControllers: enqueue abort controller %s\n", controller->name());
        controller->enqueueAbort();
        iter = iter.next();
    }
    getControllerNameMap()->unlock();
}

/*static*/ void UMLRTDeploymentMap::enqueueExitAllControllers ( void * exitValue )
{
    getControllerNameMap()->lock();
    UMLRTHashMap::Iterator iter = getControllerNameMap()->getIterator();

    while (iter != iter.end())
    {
        UMLRTController * controller = (UMLRTController *)iter.getObject();
        BDEBUG(BD_CONTROLLERMAP, "enqueueExitAllControllers: enqueue exit controller %s\n", controller->name());
        controller->enqueueExit(exitValue);
        iter = iter.next();
    }
    getControllerNameMap()->unlock();
}

/*static*/ void UMLRTDeploymentMap::spawnAllControllers ( )
{
    getControllerNameMap()->lock();
    UMLRTHashMap::Iterator iter = getControllerNameMap()->getIterator();

    while (iter != iter.end())
    {
        UMLRTController * controller = (UMLRTController *)iter.getObject();
        controller->spawn();
        iter = iter.next();
    }
    getControllerNameMap()->unlock();
}

/*static*/ void UMLRTDeploymentMap::joinAllControllers ( )
{
    getControllerNameMap()->lock();
    UMLRTHashMap::Iterator iter = getControllerNameMap()->getIterator();

    while (iter != iter.end())
    {
        UMLRTController * controller = (UMLRTController *)iter.getObject();
        controller->join();
        iter = iter.next();
    }
    getControllerNameMap()->unlock();
}

/*static*/ void UMLRTDeploymentMap::addCapsuleToController ( char * capsuleName, char * controllerName )
{
    UMLRTController * controller = (UMLRTController *)getCapsuleToControllerMap()->getObject(capsuleName);

    if (controller != NULL)
        FATAL("capsule-to-controller-map already had an entry for capsule '%s'", capsuleName);
    else
        getCapsuleToControllerMap()->insert(strdup(capsuleName), (void *)strdup(controllerName));
}

/*static*/ void UMLRTDeploymentMap::addControllerToHost ( char * controllerName, char * hostName )
{
    UMLRTHost * host = (UMLRTHost *)getControllerToHostMap()->getObject(controllerName);

    if (host != NULL)
        FATAL("controller-to-host-map already had an entry for controller '%s'", controllerName);
    else
        getControllerToHostMap()->insert(strdup(controllerName), (void *)strdup(hostName));
}


/*static*/ void UMLRTDeploymentMap::addPortToHost ( char* capsuleName, char * portName, char * hostName )
{
    UMLRTHashMap* portMap = getPortToHostMapForCapsule(capsuleName);
    if(portMap == NULL)
    {
        portMap = new UMLRTHashMap("portToHostMap", UMLRTHashMap::compareString, false);
        getCapsuleToPortHostMap()->insert(strdup(capsuleName), (void*)portMap);
    }

    UMLRTHost * host = (UMLRTHost *)portMap->getObject(portName);
    if (host != NULL)
        FATAL("port-to-host-map already had an entry for port '%s'", portName);
    else
        portMap->insert(strdup(portName), (void *)strdup(hostName));
}

/*static*/ const char * UMLRTDeploymentMap::getHostNameForController ( const char * controllerName )
{
    void * hostName = getControllerToHostMap()->getObject(controllerName);
    if (hostName != NULL)
        return (const char *) hostName;

    return NULL;
}

/*static*/ UMLRTHost * UMLRTDeploymentMap::getHostForController ( const char * controllerName )
{
    const char * hostName = getHostNameForController( controllerName );

    BDEBUG(BD_CONTROLLERMAP, "getHostForController: controller(%s) map-host(%s)\n",
            controllerName,
            hostName ? hostName : "-no host-");

    return (UMLRTHost *)getHostFromName(hostName);
}


/*static*/ const char * UMLRTDeploymentMap::getHostNameForPort ( const char* capsuleName, const char * portName )
{
    UMLRTHashMap* portMap = getPortToHostMapForCapsule(capsuleName);
    if(portMap == NULL)
        return NULL;

    void * hostName = portMap->getObject(portName);
    if (hostName != NULL)
        return (const char *) hostName;

    return NULL;
}


/*static*/ UMLRTHost * UMLRTDeploymentMap::getHostForPort ( const char* capsuleName, const char * portName )
{
    const char * hostName = getHostNameForPort(capsuleName, portName);
    if(hostName == NULL)
        return NULL;

    BDEBUG(BD_CONTROLLERMAP, "getHostForPort: port(%s) map-host(%s)\n",
            portName,
            hostName ? hostName : "-no host-");

    return (UMLRTHost *)getHostFromName(hostName);
}

// Return the host where this capsule is running. Returns NULL if no host is assigned or doesn't exist.
/*static*/ UMLRTHost * UMLRTDeploymentMap::getHostForCapsule ( const char * capsuleName )
{
    UMLRTHost* host = NULL;
    const char* controller = getControllerNameForCapsule( capsuleName );

    if( controller != NULL )
        host = getHostForController(controller);

    return host;
}

// Remove a capsule instance from the list.
/*static*/ void UMLRTDeploymentMap::removeCapsule ( const char * capsuleName, UMLRTCapsule * capsule )
{
    getCapsuleNameMap()->remove(capsuleName);
}

// Remove a controller instance from the list.
/*static*/ void UMLRTDeploymentMap::removeController ( const char * controllerName )
{
    getControllerNameMap()->remove(controllerName);
}

// Remove a controller instance from the list.
/*static*/ void UMLRTDeploymentMap::removeHost ( const char * hostName )
{
    getHostNameMap()->remove(hostName);
}

// Assign static capsule controllers based on the run-time map.
/*static*/ void UMLRTDeploymentMap::setDefaultSlotList ( UMLRTSlot * slots, size_t size )
{
    for (size_t i = 0; i < size; ++i)
    {
        addSlot( slots[i].name , &slots[i] );

        UMLRTController * controller = getControllerForCapsule(slots[i].name);

        BDEBUG(BD_CONTROLLERMAP, "setDefaultSlotList: slot[%d] slot-controller(%s) map-controller(%s)\n",
                i,
                slots[i].controller ? slots[i].controller->name() : "-none in slot-",
                controller ? controller->name() : "-no mapped controller-");

        // Only reassign static capsules (i.e. controller already assigned) whose controllers were specified in the run-time map.
        if ((slots[i].controller != NULL) && (controller != NULL))
        {
            // Reassign the capsule according to the run-time map collected.
            slots[i].controller = controller;
        }
    }
    defaultSlotList = slots;
    numDefaultSlotList = size;
}

///*static*/ bool UMLRTDeploymentMap::fromFile( const char* fileName )
//{
//    std::string schemafile;
//    std::string jsonfile;
//
//    if( flatbuffers::LoadFile(UMLRTDEPLOYMENTMAP_SCHEMA_FILE, false, &schemafile)
//        && flatbuffers::LoadFile(fileName, false, &jsonfile) )
//    {
//        flatbuffers::Parser parser;
//        if( parser.Parse(schemafile.c_str(), NULL) 
//                && parser.Parse(jsonfile.c_str(), NULL) )
//        {
//            decode( parser.builder_.GetBufferPointer(), parser.builder_.GetSize());
//            return true;
//        }
//    }
//
//    return false;
//}

/*static*/ bool UMLRTDeploymentMap::fromFile( const char* fileName )
{
    std::ifstream file(fileName);
    if(!file.is_open())
        return false;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonStr = buffer.str();

    payloadSize = jsonStr.length() + 1;
    payload = (uint8_t*) malloc(payloadSize);
    memcpy(payload, jsonStr.c_str(), payloadSize);

    decode(payload, payloadSize);
    return true;
}

/*static*/ void UMLRTDeploymentMap::decode( uint8_t* buffer , size_t bufferSize )
{
    Document document;
    document.Parse((char*)buffer);

    if( !document.IsObject() ) 
        goto die;

    if( document.HasMember("hosts") )
    {
        const Value& hosts = document["hosts"];
        if( !hosts.IsArray() )
            goto die;

        for( SizeType i=0; i< hosts.Size(); i++ ) 
        {
            if( !hosts[i].IsObject() )
                goto die;

            const char* hostName = hosts[i]["name"].GetString();
            const char* hostAddress = hosts[i]["address"].GetString();

            UMLRTHost* host = UMLRTDeploymentMap::getHostFromName( hostName );
            if( host != NULL )
            {
                if( strcmp( host->address, hostAddress ) != 0 )
                    FATAL( "Duplicate hostname: %s\n", hostName );
            }
            else
            {
                new UMLRTHost( (char*)hostName, (char*)hostAddress );
            }
        }
    }

    if( document.HasMember("controllers") )
    {
        const Value& controllers = document["controllers"];
        if( !controllers.IsArray() )
            goto die;

        for( SizeType i=0; i< controllers.Size(); i++ ) 
        {
            if( !controllers[i].IsObject() )
                goto die;

            const char* controllerName = controllers[i]["name"].GetString();
            const char* hostName = controllers[i]["host"].GetString();

            UMLRTHost* host = UMLRTDeploymentMap::getHostFromName( hostName );
            if(host == NULL)
                FATAL("No such host: %s\n", hostName);

            UMLRTDeploymentMap::addControllerToHost( (char*)controllerName, (char*)hostName );
        }
    }

    if( document.HasMember("capsules") )
    {
        const Value& capsules = document["capsules"];
        if( !capsules.IsArray() )
            goto die;

        for( SizeType i=0; i< capsules.Size(); i++ ) 
        {
            if( !capsules[i].IsObject() )
                goto die;

            const char* capsuleName = capsules[i]["name"].GetString();
            const char* controllerName = capsules[i]["controller"].GetString();
            UMLRTDeploymentMap::addCapsuleToController( (char*)capsuleName, (char*)controllerName );

            if(capsules[i].HasMember("proxyports"))
            {
                const Value& proxyports = capsules[i]["proxyports"];
                if( !proxyports.IsArray() )
                    goto die;

                for( SizeType j=0; j< proxyports.Size(); j++ )
                {
                    const char* portName = proxyports[j]["name"].GetString();
                    const char* portEndPoint = proxyports[j]["endpoint"].GetString();
                    UMLRTDeploymentMap::addPortToHost( (char*)capsuleName, (char*)portName, (char*)portEndPoint );
                }
            }
        }
    }

    return;

    die:
    FATAL("Error decoding deployment map\n");
}

///*static*/ void UMLRTDeploymentMap::decode( uint8_t* flatbuffer, size_t flatbufferSize )
//{  
//    auto deploymentObj = FBSchema::GetDeployment( flatbuffer );
//    auto hosts = deploymentObj->hosts( );
//    auto controllers = deploymentObj->controllers( );
//    auto capsules = deploymentObj->capsules( );
//
//    auto numHosts = hosts->Length( );
//    auto numControllers = controllers->Length( );
//    auto numCapsules = capsules->Length( );
//    for( int i=0; i<numHosts; i++ )
//    {
//        auto hostObj = hosts->Get( i );
//        auto hostName = hostObj->name( )->c_str( );
//        auto hostAddress = hostObj->address( )->c_str( );
//
//        UMLRTHost* host = UMLRTDeploymentMap::getHostFromName( hostName );
//        if( host != NULL )
//        {
//            if( strcmp( host->address, hostAddress ) != 0 )
//                FATAL( "Duplicate hostname: %s\n", hostName );
//        }
//        else
//        {
//            new UMLRTHost( (char*)hostName, (char*)hostAddress );
//        }
//    }
//
//    for( int i=0; i<numControllers; i++ )
//    {
//        auto contObj = controllers->Get(i);
//        auto controllerName = contObj->name()->c_str();
//        auto hostName = contObj->host()->c_str();
//
//        UMLRTHost* host = UMLRTDeploymentMap::getHostFromName( hostName );
//        if(host == NULL)
//            FATAL("No such host: %s\n", hostName);
//
//        UMLRTDeploymentMap::addControllerToHost( (char*)controllerName, (char*)hostName );
//    }
//
//    for( int i=0; i<numCapsules; i++ )
//    {
//        auto capObj = capsules->Get(i);
//        auto capsuleName = capObj->name()->c_str();
//        auto controllerName = capObj->controller()->c_str();
//        UMLRTDeploymentMap::addCapsuleToController( (char*)capsuleName, (char*)controllerName );
//
//        auto proxyports = capObj->proxyports( );
//        if( proxyports != NULL ) {
//            auto numProxyPorts = proxyports->Length( );
//
//            for( int j=0; j<numProxyPorts; j++ )
//            {
//                auto portObj = proxyports->Get(j);
//                auto portName = portObj->name()->c_str();
//                auto portEndPoint = portObj->endpoint()->c_str();
//                UMLRTDeploymentMap::addPortToHost( (char*)capsuleName, (char*)portName, (char*)portEndPoint );
//            }
//        }
//    }
//
//    payload = (uint8_t*) malloc(flatbufferSize);
//    memcpy(payload, flatbuffer, flatbufferSize);
//    payloadSize = flatbufferSize;
//}

/*static*/ int UMLRTDeploymentMap::encode( uint8_t ** buffer )
{
    if( payload == NULL )
        FATAL("Deployment map not encoded yet\n");

    *buffer = (uint8_t*) malloc(payloadSize);
    memcpy(*buffer, payload, payloadSize);
    return payloadSize;
}

/*static*/ bool UMLRTDeploymentMap::isLoaded( )
{
    return payload != NULL;
}

// Debug output the the capsule, controller and capsule-to-controller maps.
/*static*/ void UMLRTDeploymentMap::debugOutputControllerList ( )
{
    BDEBUG(BD_MODEL, "Controller list: { <controller name>, <instance address> }\n");

    getControllerNameMap()->lock();
    UMLRTHashMap::Iterator iter = getControllerNameMap()->getIterator();

    if (iter == iter.end())
    {
        BDEBUG(BD_MODEL, "    No controllers.\n");
    }
    else
    {
        while (iter != iter.end())
        {
            BDEBUG(BD_MODEL, "    { %s, %p }\n", ((UMLRTController *)((char *)iter.getObject()))->name(), (UMLRTController *)((char *)iter.getObject()));
            iter = iter.next();
        }
    }
    getControllerNameMap()->unlock();
}

/*static*/ void UMLRTDeploymentMap::debugOutputCapsuleList ( )
{
    BDEBUG(BD_MODEL, "Capsule list: { <capsule name>, <capsule role>, <instance address>, <capsule class>, <assigned controller> }\n");

    getCapsuleNameMap()->lock();
    UMLRTHashMap::Iterator iter = getCapsuleNameMap()->getIterator();

    if (iter == iter.end())
    {
        BDEBUG(BD_MODEL, "    No capsules.\n");
    }
    else
    {
        while (iter != iter.end())
        {
            UMLRTCapsule * capsule = (UMLRTCapsule *)iter.getObject();
            BDEBUG(BD_MODEL, "    { %s, %s, %p, %s, %s }\n",
                    capsule->name(), capsule->getName(), capsule, capsule->getTypeName(),
                    capsule->getSlot()->controller ? capsule->getSlot()->controller->name() : "no owning controller");
            iter = iter.next();
        }
    }
    getCapsuleNameMap()->unlock();
}

/*static*/ void UMLRTDeploymentMap::debugOutputHostList ( )
{
    BDEBUG(BD_MODEL, "Host list: { <host name>, <instance address> }\n");

    getHostNameMap()->lock();
    UMLRTHashMap::Iterator iter = getHostNameMap()->getIterator();

    if (iter == iter.end())
    {
        BDEBUG(BD_MODEL, "    No hosts.\n");
    }
    else
    {
        while (iter != iter.end())
        {
            BDEBUG(BD_MODEL, "    { %s, %p }\n", ((UMLRTHost *)((char *)iter.getObject()))->name, (UMLRTHost *)((char *)iter.getObject()));
            iter = iter.next();
        }
    }
    getHostNameMap()->unlock();
}

/*static*/ void UMLRTDeploymentMap::debugOutputSlotList ( )
{
    BDEBUG(BD_MODEL, "Host list: { <slot name>, <instance address> }\n");

    getSlotNameMap()->lock();
    UMLRTHashMap::Iterator iter = getSlotNameMap()->getIterator();

    if (iter == iter.end())
    {
        BDEBUG(BD_MODEL, "    No slots.\n");
    }
    else
    {
        while (iter != iter.end())
        {
            BDEBUG(BD_MODEL, "    { %s, %p }\n", ((UMLRTSlot *)((char *)iter.getObject()))->name, (UMLRTSlot *)((char *)iter.getObject()));
            iter = iter.next();
        }
    }
    getSlotNameMap()->unlock();
}

// Debug output of capsule-to-controller map.
/*static*/ void UMLRTDeploymentMap::debugOutputCaspuleToControllerMap ( )
{
    BDEBUG(BD_MODEL, "Capsule to controller map: { <slot>, <controller>  }\n");

    getCapsuleToControllerMap()->lock();
    UMLRTHashMap::Iterator ctclIter = getCapsuleToControllerMap()->getIterator();

    if (ctclIter == ctclIter.end())
    {
        BDEBUG(BD_MODEL, "    No capsule to controller assignments.\n");
    }
    else
    {
        while (ctclIter != ctclIter.end())
        {
            const char * capsuleName = (const char *)ctclIter.getKey();

            BDEBUG(BD_MODEL, "    { %s, %s }\n",
                                capsuleName,
                                (ctclIter.getObject() == NULL) ? "?no controller?" : ctclIter.getObject());
            ctclIter = ctclIter.next();
        }
    }
    getCapsuleToControllerMap()->unlock();
}

/*static*/ void UMLRTDeploymentMap::debugOutputControllerToHostMap ( )
{
    BDEBUG(BD_MODEL, "Controller to host map: { <controller>, <host> }\n");

    getControllerToHostMap()->lock();
    UMLRTHashMap::Iterator clIter = getControllerToHostMap()->getIterator();

    if (clIter == clIter.end())
    {
        BDEBUG(BD_MODEL, "    No controller to host assignments.\n");
    }
    else
    {
        while (clIter != clIter.end())
        {
            const char * controllerName = (const char *)clIter.getKey();
            
            BDEBUG(BD_MODEL, "    { %s, %s }\n",
                                controllerName,
                                (clIter.getObject() == NULL) ? "?no host?" : clIter.getObject());
            clIter = clIter.next();
        }
    }
    getControllerToHostMap()->unlock();
}
