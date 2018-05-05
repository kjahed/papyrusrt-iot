// umlrtddscommunicator.hh

/*******************************************************************************
* Copyright (c) 2014-2015 Zeligsoft (2009) Limited  and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*******************************************************************************/

#ifndef UMLRTDDSCOMMUNICATOR_HH
#define UMLRTDDSCOMMUNICATOR_HH


#include "umlrthashmap.hh"
#include "umlrtcommunicator.hh"

#include <functional>

#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/LocalObject.h>
#include <dds/DCPS/Service_Participant.h>

#include "RawTypeSupportImpl.h"
#include "RawC.h"

class UMLRTDDSCommunicator : public UMLRTCommunicator
{
public:
    UMLRTDDSCommunicator (const char * _localaddr ); 
    ~UMLRTDDSCommunicator ( );

    void connect ( UMLRTHost * host );
    void disconnect ( UMLRTHost * host );
    UMLRTCommunicator::Message* sendrecv();
    
    void sendDeployment ( UMLRTHost * host );
    void sendReadySignal ( UMLRTHost * host );
    void waitForReadySignal ( UMLRTHost * host );
    void waitForDeployment ( );

    void subscribe( UMLRTHost * host, const char* topic );
    void publish( UMLRTHost* host, const char* topic , uint8_t* buffer, int bufferSize );

    class RawBufferReaderListenerImpl
        : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
    {
    public:
        RawBufferReaderListenerImpl() = default;
        virtual ~RawBufferReaderListenerImpl() = default;
        void setReceivedBufferFunction(std::function<void (const PapyrusrtDDS::RawBuffer&)> tf);
        virtual void on_requested_deadline_missed(DDS::DataReader_ptr reader, 
                const DDS::RequestedDeadlineMissedStatus& status);
        virtual void on_requested_incompatible_qos(DDS::DataReader_ptr reader, 
                const DDS::RequestedIncompatibleQosStatus& status);
        virtual void on_liveliness_changed(DDS::DataReader_ptr reader,
                const DDS::LivelinessChangedStatus& status);
        virtual void on_subscription_matched (DDS::DataReader_ptr reader,
                const DDS::SubscriptionMatchedStatus& status);
        virtual void on_sample_rejected(DDS::DataReader_ptr reader, const DDS::SampleRejectedStatus& status);
        virtual void on_data_available(DDS::DataReader_ptr reader);
        virtual void on_sample_lost(DDS::DataReader_ptr reader, const DDS::SampleLostStatus& status);

    private:
        std::function<void (const PapyrusrtDDS::RawBuffer&)> m_function;
    };

private:    
    void createParticipant ( );
    void createPublisher ( );
    void registerTopic ( );
    void createDataWriter ( );
    void createSubscriber ( );
    void createDataReader ( );

    void publishBuffer ( uint8_t* buffer, int bufferSize );

    UMLRTHashMap* clientMap;

    PapyrusrtDDS::RawBufferTypeSupport_var m_topicSupport;
    PapyrusrtDDS::RawBufferDataWriter_var m_dataWriter;

    DDS::DomainParticipantFactory_var m_participantFactory;
    DDS::DomainParticipant_var m_participant;
    DDS::Publisher_var m_publisher;
    DDS::Topic_var m_messageTopic;
    DDS::InstanceHandle_t m_topicHandler;
    DDS::DataReaderListener_var m_listener;
    DDS::TopicQos m_defaultQoS;
    DDS::Subscriber_var m_subscriber;
};

#endif // UMLRTDDSCOMMUNICATOR_HH
