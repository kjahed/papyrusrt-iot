// umlrtcoapthread.hh

/*******************************************************************************
* Copyright (c) 2014-2015 Zeligsoft (2009) Limited  and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*******************************************************************************/

#ifndef UMLRTCOAPSERVER_HH
#define UMLRTCOAPSERVER_HH

#include "umlrtbasicthread.hh"

class UMLRTCoapServer : public UMLRTBasicThread
{

public:

    UMLRTCoapServer( const char * name_, const char * addr, int port );

    void * run( void * args ) = 0;
};

#endif // UMLRTCOAPSERVER_HH
