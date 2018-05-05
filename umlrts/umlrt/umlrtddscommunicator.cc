// umlrtmqttcommunicator.cc

/*******************************************************************************

*******************************************************************************/

// UMLRTDDSCommunicator manages all MQTT communication.

#include "basedebug.hh"
#include "basefatal.hh"
#include "umlrtqueue.hh"
#include "umlrtexecutiondirector.hh"
#include "umlrtddscommunicator.hh"
#include "umlrthost.hh"

#include <unistd.h>
#include <stdio.h>
#include <vector>

#include "RawTypeSupportC.h"

#include <ace/streams.h>
#include <dds/DCPS/Marked_Default_Qos.h>

static DDS::DomainId_t DomainID{ 1066 };
static const char* MessageType{ "Message type" };
static const char* MessageTopic{ "Message topic" };

static bool wait_deployment = true;
static bool wait_ready = true;

static UMLRTQueue incomingDdsMessages;
static int ddsMessageReceivedCB(const PapyrusrtDDS::RawBuffer& rawBuffer)
{
    
    UMLRTCommunicator::Message* msg = 
        UMLRTCommunicator::Message::decode((uint8_t*)rawBuffer.buffer.get_buffer());

    if( msg->isDeploymentSignal() )
    {
        if( !wait_deployment )
        {
            delete msg;
            msg = NULL;
        }
        else
        {
            wait_deployment = false;
        }
    }

    else if( msg->isReadySignal() )
    {
        if( !wait_ready )
        {
            delete msg;
            msg = NULL;
        }
        else
        {
            wait_ready = false;
        }
    }

    else if( msg->isProtocolSignal() )
    {
        if( strcmp(msg->receiver, UMLRTExecutionDirector::localhost->name) != 0 )
        {
            delete msg;
            msg = NULL;
        }
    }
    else 
        FATAL("Unknown message type received\n");

    if( msg != NULL )
    {
        incomingDdsMessages.enqueue(msg);
    }

    return 1;
}


UMLRTDDSCommunicator::UMLRTDDSCommunicator ( const char * _localaddr ) 
    : UMLRTCommunicator(_localaddr)
{
    createParticipant();
    createPublisher();
    registerTopic();
    createDataWriter();

    createSubscriber();
    createDataReader();
}   

UMLRTDDSCommunicator::~UMLRTDDSCommunicator ( )
{
    try {
        if (!CORBA::is_nil (m_participant.in ())) {
            m_participant->delete_contained_entities();
        }
        if (!CORBA::is_nil (m_participantFactory.in ())) {
            m_participantFactory->delete_participant(m_participant.in ());
        }
    } catch (CORBA::Exception& e) {
    }

    TheServiceParticipant->shutdown();
}

void UMLRTDDSCommunicator::publishBuffer ( uint8_t* buffer, int bufferSize )
{
    PapyrusrtDDS::BufferType ddsBuffer(bufferSize, bufferSize, buffer, 0);
    PapyrusrtDDS::RawBuffer rawBuffer;
    rawBuffer.buffer = ddsBuffer;

    int res = m_dataWriter->write(rawBuffer, m_topicHandler);
    if (res != DDS::RETCODE_OK) 
        ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: Publisher::sendMessage write returned %d.\n"), res));
}

void UMLRTDDSCommunicator::sendDeployment ( UMLRTHost * host )
{
    wait_deployment = false;
    
    uint8_t* buffer;
    int bufferSize = UMLRTCommunicator::DeploymentMessage::make(localhost, host, &buffer);
    publishBuffer(buffer, bufferSize);
    free(buffer);
}


void UMLRTDDSCommunicator::waitForDeployment ( )
{
    while( incomingDdsMessages.count() == 0 )
        usleep(100);
    
    UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*)incomingDdsMessages.dequeue();    
    UMLRTDeploymentMap::decode( msg->payload, msg->payloadSize );
    delete msg;
}

void UMLRTDDSCommunicator::sendReadySignal ( UMLRTHost * host )
{
    wait_ready = false;

    uint8_t* buffer;
    int bufferSize = UMLRTCommunicator::ReadyMessage::make(localhost, host, &buffer);
    publishBuffer(buffer, bufferSize); 
    free(buffer);
}

void UMLRTDDSCommunicator::waitForReadySignal ( UMLRTHost * host )
{
    while( incomingDdsMessages.count() == 0 )
        usleep(100);
    
    UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*)incomingDdsMessages.dequeue();    
    delete msg;
}

void UMLRTDDSCommunicator::connect ( UMLRTHost * host )
{
}

void UMLRTDDSCommunicator::disconnect ( UMLRTHost * host )
{

	host->connected = false;
}

void UMLRTDDSCommunicator::subscribe( UMLRTHost* host, const char* topic )
{
}

void UMLRTDDSCommunicator::publish( UMLRTHost* host, const char* topic , uint8_t* buffer, int bufferSize )
{
}

UMLRTCommunicator::Message* UMLRTDDSCommunicator::sendrecv()
{
	UMLRTCommunicator::Message* msg = (UMLRTCommunicator::Message*) messageQueue.dequeue();
	if(msg != NULL)
	{
        uint8_t* buffer;
        int bufferSize = msg->encode(&buffer);
        publishBuffer(buffer, bufferSize);
        free(buffer);
		delete msg;
	}

	if(incomingDdsMessages.count() > 0)
    {
        msg = (UMLRTCommunicator::Message*) incomingDdsMessages.dequeue();
        if( !msg->isProtocolSignal() 
                || strcmp(msg->receiver, localhost->name) != 0)
            FATAL("Unexpected message received from %s\n", msg->receiver);
        return msg;
    }

    return NULL;
}

void UMLRTDDSCommunicator::createParticipant() {
    int argc = 2;
    ACE_TCHAR *argv[2];
    argv[0] = "dummy";
    argv[1] = "-DCPSConfigFile dds.conf";
    auto m_participantFactory = TheParticipantFactoryWithArgs(argc, argv);

    m_participant = m_participantFactory->create_participant(
            DomainID,
            PARTICIPANT_QOS_DEFAULT,
            DDS::DomainParticipantListener::_nil(),
            ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (CORBA::is_nil (m_participant.in ()))
        FATAL("Failed to create participant\n");
}

void UMLRTDDSCommunicator::createPublisher() {
    m_publisher = m_participant->create_publisher(
        PUBLISHER_QOS_DEFAULT,
        DDS::PublisherListener::_nil(),
        ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
    );

    if (CORBA::is_nil(m_publisher.in()))
        FATAL("Failed to create publisher\n");
}

void UMLRTDDSCommunicator::registerTopic() {
    m_topicSupport = new PapyrusrtDDS::RawBufferTypeSupportImpl();
    if (DDS::RETCODE_OK != m_topicSupport->register_type(
        m_participant.in(),
        MessageType)
    ) {
        FATAL("Register message type failed\n");
    }

    DDS::TopicQos defaultQoS;
    m_participant->get_default_topic_qos(defaultQoS);
    m_messageTopic = m_participant->create_topic (
        MessageTopic,
        MessageType,
        defaultQoS,
        DDS::TopicListener::_nil(),
        ::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (CORBA::is_nil (m_messageTopic.in ())) {
        FATAL("Create topic failed\n"); 
    }
}

void UMLRTDDSCommunicator::createDataWriter() {
    DDS::DataWriterQos defaultQoS;
    m_publisher->get_default_datawriter_qos (defaultQoS);
    DDS::DataWriter_var dataWriter =
    m_publisher->create_datawriter(
        m_messageTopic.in (),
        defaultQoS,
        DDS::DataWriterListener::_nil(),
        ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
    );
    if (CORBA::is_nil (dataWriter.in ())) 
        FATAL("Error creating data writer\n");

    m_dataWriter = PapyrusrtDDS::RawBufferDataWriter::_narrow(dataWriter.in());
    if (CORBA::is_nil (m_dataWriter.in ()))
        FATAL("Error narrowing data writer\n");

    PapyrusrtDDS::RawBuffer rawBuffer;
    m_topicHandler = m_dataWriter->register_instance(rawBuffer);
}

void UMLRTDDSCommunicator::createSubscriber() {
    m_subscriber = m_participant->create_subscriber(
        SUBSCRIBER_QOS_DEFAULT,
        DDS::SubscriberListener::_nil(),
        ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
    );
    if (CORBA::is_nil (m_subscriber.in ()))
        FATAL("Error creating subscriber\n");
}

void UMLRTDDSCommunicator::createDataReader() {
    m_listener = new RawBufferReaderListenerImpl;
    auto listenerServant = dynamic_cast<RawBufferReaderListenerImpl*>(m_listener.in());

    if (CORBA::is_nil (m_listener.in ()))
        FATAL("Error creating data listener\n");

    DDS::DataReaderQos defaultQoS;
    m_subscriber->get_default_datareader_qos (defaultQoS);

    DDS::DataReader_var quote_dr = m_subscriber->create_datareader(
        m_messageTopic.in (),
        defaultQoS,
        m_listener.in (),
        ::OpenDDS::DCPS::DEFAULT_STATUS_MASK
    );

    std::function<void (const PapyrusrtDDS::RawBuffer&)> fun = ddsMessageReceivedCB;
    listenerServant->setReceivedBufferFunction(fun);
}

void UMLRTDDSCommunicator::RawBufferReaderListenerImpl::setReceivedBufferFunction(
        std::function<void (const PapyrusrtDDS::RawBuffer&)> tf) {
	m_function = tf;
}

void UMLRTDDSCommunicator::RawBufferReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader) {
	try {
		PapyrusrtDDS::RawBufferDataReader_var messageDataReader 
            = PapyrusrtDDS::RawBufferDataReader::_narrow(reader);

		if (CORBA::is_nil(messageDataReader.in ())) 
            FATAL("Error initializing data reader\n");

		int count = 0;

		while(true) {
			PapyrusrtDDS::RawBuffer rawBuffer;
			DDS::SampleInfo si ;
			DDS::ReturnCode_t status = messageDataReader->take_next_sample(rawBuffer, si) ;
			if (status == DDS::RETCODE_OK && si.valid_data) {
				m_function(rawBuffer);
			} else if (status == DDS::RETCODE_NO_DATA) {
				break;
			} else {
                FATAL("Error reading dds message\n");
			}
		}
	} catch (CORBA::Exception& e) {
        FATAL("Caught CORBA::Exception\n");
	}
}

void UMLRTDDSCommunicator::RawBufferReaderListenerImpl::on_requested_deadline_missed(DDS::DataReader_ptr,
        const DDS::RequestedDeadlineMissedStatus&) {
}

void UMLRTDDSCommunicator::RawBufferReaderListenerImpl::on_requested_incompatible_qos(DDS::DataReader_ptr, 
        const DDS::RequestedIncompatibleQosStatus&) {
}

void UMLRTDDSCommunicator::RawBufferReaderListenerImpl::on_liveliness_changed(DDS::DataReader_ptr, 
        const DDS::LivelinessChangedStatus&) {
}

void UMLRTDDSCommunicator::RawBufferReaderListenerImpl::on_subscription_matched(DDS::DataReader_ptr, 
        const DDS::SubscriptionMatchedStatus&) {
}

void UMLRTDDSCommunicator::RawBufferReaderListenerImpl::on_sample_rejected(DDS::DataReader_ptr, 
        const DDS::SampleRejectedStatus&) {
}

void UMLRTDDSCommunicator::RawBufferReaderListenerImpl::on_sample_lost(DDS::DataReader_ptr, 
        const DDS::SampleLostStatus&) {
}
