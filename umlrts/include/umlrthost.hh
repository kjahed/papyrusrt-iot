// umlrthost.hh

/*******************************************************************************

*******************************************************************************/

#ifndef UMLRTHOST_HH
#define UMLRTHOST_HH

#include "basedebug.hh"
#include "umlrtdeploymentmap.hh"
#include <string.h>

// This is the data container for information the RTS needs about each host
class UMLRTHost
{
public:
	UMLRTHost ( const char* name_, const char* address_ ) 
        : name(strdup(name_)), address(strdup(address_))  {
		BDEBUG(BD_INSTANTIATE, "%s constructor\n", name);
    	UMLRTDeploymentMap::addHost(name, this);
        connected = false;
        deployed = false;
        signaled = false;
        gotack = false;
	}

	~UMLRTHost ( ) {
		BDEBUG(BD_INSTANTIATE, "%s destructor\n", name);
    	UMLRTDeploymentMap::removeHost(name);
	}

    bool isMQTT() {
        return strncmp("mqtt://", address, 7) == 0;
    }

    const char * const name;
    const char * const address;
    int socket;
    bool local;
    bool connected;
    bool deployed;
    bool signaled;
    bool gotack;
};

#endif // UMLRTHOST_HH
