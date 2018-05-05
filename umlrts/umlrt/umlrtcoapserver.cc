// umlrtcoapserver.cc

/*******************************************************************************
* Copyright (c) 2014-2015 Zeligsoft (2009) Limited  and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*******************************************************************************/

#include "umlrtcoapserver.hh"

static void
deploymentHandler(coap_context_t *ctx, struct coap_resource_t *resource,
              const coap_endpoint_t *local_interface, coap_address_t *peer,
              coap_pdu_t *request, str *token, coap_pdu_t *response)
{
	unsigned char buf[3];
	const char* response_data     = "Hello World!";
	response->hdr->code           = COAP_RESPONSE_CODE(205);
	coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
	coap_add_data  (response, strlen(response_data), (unsigned char *)response_data);
}

UMLRTCoapServer( const char * ip_, int port_ ) 
    : UMLRTBasicThread("UMLRTCoapServer"), ip(ip_), port(port_) 
{
}

void UMLRTCoapServer::init ( )
{
    coap_context_t* context;
    coap_address_t serverAddress;
    coap_address_init(&serverAddress);
    serverAddress.addr.sin.sin_family = AF_INT;
    serverAddress.addr.sin.sin_addr.s_addr = ip;
    serverAddress.addr.sin.sin_port = htons(port);

    context = coap_new_context(&serverAddress);
    if( !context )
        FATAL("Error creating CoAP context\n");

    coap_resource_t* deploymentResource;
    deploymentResource = coap_resource_init((unsigned char *)"deployment", 10, 0);
    coap_register_handler(deploymentResource, COAP_REQUEST_GET, deploymentHandler);
    coap_add_resource(context, deploymentResource);
}

void * UMLRTCoapServer::run ( void * args )
{
    
}

