# core files

# platform independent include directories
set(INCS
  ${INCS}
  ${UMLRTS_ROOT}/util/include
  ${UMLRTS_ROOT}/include
  ${UMLRTS_ROOT}/umlrt/src/include
  ${UMLRTS_ROOT}/dds_idl/
  )

# sources that make up the RTS services library
set(SRCS
  ${SRCS}
  ${UMLRTS_ROOT}/umlrt/src/umlrtgetopt.cc
  ${UMLRTS_ROOT}/umlrt/umlrtapi.cc
  ${UMLRTS_ROOT}/umlrt/umlrtbasicthread.cc
  ${UMLRTS_ROOT}/umlrt/umlrtcapsule.cc
  ${UMLRTS_ROOT}/umlrt/umlrtcapsuleid.cc
  ${UMLRTS_ROOT}/umlrt/umlrtcommsport.cc
  ${UMLRTS_ROOT}/umlrt/umlrtcommunicator.cc
  ${UMLRTS_ROOT}/umlrt/umlrtcontroller.cc
  ${UMLRTS_ROOT}/umlrt/umlrtdeploymentmap.cc
  ${UMLRTS_ROOT}/umlrt/umlrtexecutiondirector.cc
  ${UMLRTS_ROOT}/umlrt/umlrtframeprotocol.cc
  ${UMLRTS_ROOT}/umlrt/umlrtframeservice.cc
  ${UMLRTS_ROOT}/umlrt/umlrthashmap.cc
  ${UMLRTS_ROOT}/umlrt/umlrtinsignal.cc
  ${UMLRTS_ROOT}/umlrt/umlrtinoutsignal.cc
  ${UMLRTS_ROOT}/umlrt/umlrtlogprotocol.cc
  ${UMLRTS_ROOT}/umlrt/umlrtmain.cc
  ${UMLRTS_ROOT}/umlrt/umlrtmainloop.cc
  ${UMLRTS_ROOT}/umlrt/umlrtmaintargetshutdown.cc
  ${UMLRTS_ROOT}/umlrt/umlrtmaintargetstartup.cc
  ${UMLRTS_ROOT}/umlrt/umlrtmessage.cc
  ${UMLRTS_ROOT}/umlrt/umlrtmessagepool.cc
  ${UMLRTS_ROOT}/umlrt/umlrtmessagequeue.cc
  ${UMLRTS_ROOT}/umlrt/umlrtmqttcommunicator.cc
  ${UMLRTS_ROOT}/umlrt/umlrtnanocommunicator.cc
  ${UMLRTS_ROOT}/umlrt/umlrtcoapcommunicator.cc
  ${UMLRTS_ROOT}/umlrt/umlrtddscommunicator.cc
  ${UMLRTS_ROOT}/umlrt/umlrtobjectclass.cc
  ${UMLRTS_ROOT}/umlrt/umlrtoutsignal.cc
  ${UMLRTS_ROOT}/umlrt/umlrtpool.cc
  ${UMLRTS_ROOT}/umlrt/umlrtprioritymessagequeue.cc
  ${UMLRTS_ROOT}/umlrt/umlrtprotocol.cc
  ${UMLRTS_ROOT}/umlrt/umlrtqueue.cc
  ${UMLRTS_ROOT}/umlrt/umlrtrtsinterfaceumlrt.cc
  ${UMLRTS_ROOT}/umlrt/umlrtsignal.cc
  ${UMLRTS_ROOT}/umlrt/umlrtsignalelement.cc
  ${UMLRTS_ROOT}/umlrt/umlrtsignalelementpool.cc
  ${UMLRTS_ROOT}/umlrt/umlrtsignalregistry.cc
  ${UMLRTS_ROOT}/umlrt/umlrttimerid.cc
  ${UMLRTS_ROOT}/umlrt/umlrttimerpool.cc
  ${UMLRTS_ROOT}/umlrt/umlrttimerprotocol.cc
  ${UMLRTS_ROOT}/umlrt/umlrttimerqueue.cc
  ${UMLRTS_ROOT}/umlrt/umlrttimespec.cc
  ${UMLRTS_ROOT}/dds_idl/RawTypeSupportImpl.cpp
  ${UMLRTS_ROOT}/dds_idl/RawTypeSupportC.cpp
  ${UMLRTS_ROOT}/dds_idl/RawTypeSupportS.cpp
  ${UMLRTS_ROOT}/dds_idl/RawC.cpp
  ${UMLRTS_ROOT}/dds_idl/RawS.cpp
  )
    
