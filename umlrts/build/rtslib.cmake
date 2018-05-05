# rts library

# RTS value must match LIBRARY value in library's CMakelists.txt file
set(RTS rts)
set(NANO nanomsg)
set(FLAT flatbuffers)
set(FBSCHEMAS fbschemas)
set(MQTT paho-mqtt3c)
set(COAP coap)
set(CMAKE_CXX_STANDARD 11)

# building library
if (RTS STREQUAL LIBRARY)

  # library sources
  set(SRCS)
  if (NOT OS_FILES_SDIR)
    set(OS_FILES_SDIR ${TARGETOS})
  endif ()
  set(FILES_CONFIG_DIR ${UMLRTS_ROOT}/build/os)
  include(${FILES_CONFIG_DIR}/files.cmake)
  set(FILES_CONFIG_DIR ${UMLRTS_ROOT}/build/os/${OS_FILES_SDIR})
  include(${FILES_CONFIG_DIR}/files.cmake)
else ()

  # model project depends

  # add external project support
  include(ExternalProject)

  ExternalProject_Add(${NANO}
      SOURCE_DIR ${UMLRTS_ROOT}/${NANO}
    DOWNLOAD_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_INSTALL_LIBDIR=${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}/;-DCMAKE_INSTALL_INCLUDEDIR=${UMLRTS_ROOT}/include"
    INSTALL_COMMAND
            ${CMAKE_COMMAND}
            --build .
            --target install
            --config ${configuration}
    )

  ExternalProject_Add(${FLAT}
    SOURCE_DIR ${UMLRTS_ROOT}/flatbuffers
    DOWNLOAD_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_INSTALL_LIBDIR=${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}/;-DCMAKE_INSTALL_INCLUDEDIR=${UMLRTS_ROOT}/include;-DCMAKE_INSTALL_BINDIR=${UMLRTS_ROOT}/bin"
    INSTALL_COMMAND
            ${CMAKE_COMMAND}
            --build .
            --target install
            --config ${configuration}
    )


  ExternalProject_Add(${MQTT}
    SOURCE_DIR ${UMLRTS_ROOT}/${MQTT}
    DOWNLOAD_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
    CMAKE_ARGS "${CMAKE_ARGS};-DCMAKE_INSTALL_LIBDIR=${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}/;-DCMAKE_INSTALL_INCLUDEDIR=${UMLRTS_ROOT}/include;-DCMAKE_INSTALL_BINDIR=${UMLRTS_ROOT}/bin"
    INSTALL_COMMAND
            ${CMAKE_COMMAND}
            --build .
            --target install
            --config ${configuration}
    )

  ExternalProject_Add(${COAP}
    SOURCE_DIR ${UMLRTS_ROOT}/libcoap
    DOWNLOAD_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND 
        "${UMLRTS_ROOT}/libcoap/configure" 
        --disable-tests
        --disable-documentation
        --disable-examples
        --includedir ${UMLRTS_ROOT}/include
        --libdir ${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}
        --bindir ${UMLRTS_ROOT}/bin
    INSTALL_COMMAND 
        "make"
        install
    )

  # RTS build/install
  ExternalProject_Add(${RTS}
    SOURCE_DIR ${UMLRTS_ROOT}
    DOWNLOAD_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
    INSTALL_COMMAND
            ${CMAKE_COMMAND}
            --build .
            --target install
            --config ${configuration}
    )

  add_custom_command(
    OUTPUT  fbschemasc.stamp
    DEPENDS ${FLAT}
    COMMAND ${UMLRTS_ROOT}/bin/flatc -o ${UMLRTS_ROOT}/include/fbschemas  -c --include-prefix fbschemas ${UMLRTS_ROOT}/fbschemas/*.fbs
    COMMAND ${CMAKE_COMMAND} -E touch fbschemasc.stamp
  )

  add_custom_target(${FBSCHEMAS} DEPENDS fbschemasc.stamp)
  add_dependencies(${RTS} ${NANO} ${FLAT} ${FBSCHEMAS} ${MQTT} ${COAP})
  add_dependencies(${TARGET} ${RTS})

  # Destination directory for the RTS services library.
  set(RTS_NAME ${CMAKE_STATIC_LIBRARY_PREFIX}${RTS}${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(RTS_LIB ${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}/${RTS_NAME})

  # Destination directory for the nanomsg library.
  set(NANO_NAME ${CMAKE_STATIC_LIBRARY_PREFIX}${NANO}${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(NANO_LIB ${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}/${NANO_NAME})

  # Destination directory for the faltbuffers library.
  set(FLAT_NAME ${CMAKE_STATIC_LIBRARY_PREFIX}${FLAT}${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(FLAT_LIB ${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}/${FLAT_NAME})

  # Destination directory for the mqtt library.
  set(MQTT_NAME ${CMAKE_STATIC_LIBRARY_PREFIX}${MQTT}-static${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(MQTT_LIB ${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}/${MQTT_NAME})

  # Destination directory for the coap library.
  set(COAP_NAME ${CMAKE_STATIC_LIBRARY_PREFIX}${COAP}-2${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(COAP_LIB ${UMLRTS_ROOT}/lib/${TARGETOS}/${BUILD_TOOLS}/${BUILD_CONFIG}/${COAP_NAME})

  set(INCS
    ${INCS}
    ${UMLRTS_ROOT}/include
    )
  set(LIBS
    ${LIBS}
    ${RTS_LIB}
    ${NANO_LIB}
    ${FLAT_LIB}
    ${MQTT_LIB}
    ${COAP_LIB}
    ACE
    ACEXML
    ACEXML_Parser
    OpenDDS_Dcps
    OpenDDS_FACE
    OpenDDS_Federator
    OpenDDS_InfoRepoDiscovery
    OpenDDS_InfoRepoLib
    OpenDDS_InfoRepoServ
    OpenDDS_Model
    OpenDDS_Multicast
    OpenDDS_Rtps
    OpenDDS_Rtps_Udp
    OpenDDS_Shmem
    OpenDDS_Tcp
    OpenDDS_Udp
    OpenDDS_monitor
    TAO
    TAO_AnyTypeCode
    TAO_Async_IORTable
    TAO_Async_ImR_Client_IDL
    TAO_BiDirGIOP
    TAO_CSD_Framework
    TAO_CSD_ThreadPool
    TAO_CodecFactory
    TAO_Codeset
    TAO_CosNaming
    TAO_CosNaming_Serv
    TAO_CosNaming_Skel
    TAO_DynamicInterface
    TAO_FTORB_Utils
    TAO_FT_ClientORB
    TAO_FT_ServerORB
    TAO_IDL_BE
    TAO_IDL_FE
    TAO_IORManip
    TAO_IORTable
    TAO_ImR_Activator
    TAO_ImR_Activator_IDL
    TAO_ImR_Client
    TAO_ImR_Locator
    TAO_ImR_Locator_IDL
    TAO_Messaging
    TAO_PI
    TAO_PI_Server
    TAO_PortableGroup
    TAO_PortableServer
    TAO_Strategies
    TAO_Svc_Utils
    TAO_Valuetype
    )

  # install flatbuffer schemas
  file(COPY ${UMLRTS_ROOT}/fbschemas DESTINATION ${CMAKE_BINARY_DIR})

endif ()

# reorder lib list placing object files upfront
set(OLIST)
set(SLIST)
foreach(ITEM  ${LIBS})
  if (${ITEM} MATCHES "\\.[ao]$")
    list(APPEND OLIST ${ITEM})
  else ()
    list(APPEND SLIST ${ITEM})
  endif ()
endforeach (ITEM)
set(LIBS ${OLIST} ${SLIST})
set(OLIST)
set(SLIST)
