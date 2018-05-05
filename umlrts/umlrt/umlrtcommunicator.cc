// umlrtcommunicator.cc

/*******************************************************************************

*******************************************************************************/

// UMLRTCommunicator base class.

#include "basedebug.hh"
#include "basefatal.hh"
#include "umlrtcommunicator.hh"
#include "umlrtdeploymentmap.hh"
#include "umlrtsignalregistry.hh"
#include "umlrthost.hh"
#include "umlrtobjectclass.hh"
#include "umlrtexecutiondirector.hh"

#include <stdlib.h>

#include "flatbuffers/flatbuffers.h"
#include "fbschemas/signal_generated.h"
#include "fbschemas/message_generated.h"

//#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"
//using namespace rapidjson;

#define FORMAT std::getenv("DATA_FORMAT") 

flatbuffers::FlatBufferBuilder builder1(2048);
flatbuffers::FlatBufferBuilder builder2(2048);

UMLRTCommunicator::Message::~Message ( )
{
    if(payload != NULL)
        free(payload);
}

int UMLRTCommunicator::Message::encode ( uint8_t ** buffer )
{
//    int size = strcmp(FORMAT, "flatbuffer") == 0 ? toFlatBuffer(buffer) : toJSON(buffer);
//    UMLRTExecutionDirector::total_messages_encoded++;
//    UMLRTExecutionDirector::total_size += size;
//    return size;
    if(strcmp(FORMAT, "flatbuffer") == 0)
        return toFlatBuffer(buffer);
    return toJSON(buffer);
}

UMLRTCommunicator::Message * UMLRTCommunicator::Message::decode ( uint8_t * buffer )
{
    if(strcmp(FORMAT, "flatbuffer") == 0)
        return UMLRTCommunicator::Message::fromFlatBuffer(buffer);
    return UMLRTCommunicator::Message::fromJSON(buffer);
}

int UMLRTCommunicator::DeploymentMessage::make ( UMLRTHost * src, UMLRTHost * dest, uint8_t ** buffer  )
{
    DeploymentMessage msg;
    msg.sender = src->name;
    msg.receiver = dest->name;
    msg.type = FBSchema::Type_DEPLOYMENT_MAP;
    msg.payloadSize = UMLRTDeploymentMap::encode(&msg.payload);
    return msg.encode(buffer);
}

UMLRTCommunicator::DeploymentMessage * UMLRTCommunicator::DeploymentMessage::make ( uint8_t * buffer )
{
    DeploymentMessage* msg = (DeploymentMessage*) UMLRTCommunicator::Message::decode(buffer);
    if( msg->type != FBSchema::Type_DEPLOYMENT_MAP )
        FATAL("Unexpected message received");
    return msg;
}

UMLRTCommunicator::ReadyMessage * UMLRTCommunicator::ReadyMessage::make ( uint8_t * buffer )
{
    ReadyMessage* msg = (ReadyMessage*) UMLRTCommunicator::Message::decode(buffer);
    if( msg->type != FBSchema::Type_READY_SIGNAL )
        FATAL("Unexpected message received");
    return msg;
}

int UMLRTCommunicator::ReadyMessage::make ( UMLRTHost * src, UMLRTHost * dest, uint8_t ** buffer  )
{
    ReadyMessage msg;
    msg.sender = src->name;
    msg.receiver = dest->name;
    msg.type = FBSchema::Type_READY_SIGNAL;
    msg.payloadSize = 0;
    msg.payload = NULL;
    return msg.encode(buffer);
}

UMLRTCommunicator::ProtocolMessage *  UMLRTCommunicator::ProtocolMessage::make ( 
        UMLRTHost * src, UMLRTHost * dest )
{
    ProtocolMessage * msg = new ProtocolMessage;
    msg->sender = src->name;
    msg->receiver = dest->name;
    msg->type = FBSchema::Type_PROTOCOL_SIGNAL;
    return msg;
}

UMLRTCommunicator::ProtocolMessage * UMLRTCommunicator::ProtocolMessage::make ( uint8_t * buffer )
{
    ProtocolMessage* msg = (ProtocolMessage*) UMLRTCommunicator::Message::decode(buffer);
    if( msg->type != FBSchema::Type_PROTOCOL_SIGNAL )
        FATAL("Unexpected message received");
    return msg;
}

int UMLRTCommunicator::Message::toFlatBuffer ( uint8_t ** buffer )
{
    builder1.Clear();
    auto sender = builder1.CreateString(this->sender);
    auto receiver = builder1.CreateString(this->receiver);
    auto payloadVec = builder1.CreateVector(this->payload, this->payloadSize);

    FBSchema::MessageBuilder msgBuilder(builder1);
    msgBuilder.add_sender(sender);
    msgBuilder.add_receiver(receiver);
    msgBuilder.add_type(FBSchema::Type(this->type));

    if( this->payloadSize > 0 )
    {
        msgBuilder.add_payload(payloadVec);
        msgBuilder.add_payloadSize(payloadSize);
    }

    auto msgObj = msgBuilder.Finish();
    builder1.Finish(msgObj);

    *buffer = (uint8_t*) malloc(builder1.GetSize());
    memcpy(*buffer, builder1.GetBufferPointer(), builder1.GetSize());
    return builder1.GetSize();
}

int UMLRTCommunicator::Message::toJSON ( uint8_t ** buffer )
{
    Document document;
    document.SetObject();
	Document::AllocatorType& allocator = document.GetAllocator();

    Value sender;
    Value receiver;
    Value type;
    Value payloadSize;
    //Value payload;
    Value payload(kArrayType);

    sender.SetString(this->sender, allocator);
    receiver.SetString(this->receiver, allocator);
    type.SetInt(this->type);
    payloadSize.SetInt(this->payloadSize);

    for( int i=0; i<this->payloadSize; i++ )
        payload.PushBack(Value().SetUint(this->payload[i]), allocator);

//    payload.SetString((char*)this->payload, allocator);

    document.AddMember("sender", sender, allocator);
    document.AddMember("receiver", receiver, allocator);
    document.AddMember("type", type, allocator);
    document.AddMember("payloadSize", payloadSize, allocator);
    document.AddMember("payload", payload, allocator);

    StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
	document.Accept(writer);

    std::string json = strbuf.GetString();
    *buffer = (uint8_t*) malloc(json.length()+1);
    memcpy(*buffer, json.c_str(), json.length()+1);
    return json.length()+1;
}



UMLRTCommunicator::Message* UMLRTCommunicator::Message::fromJSON ( uint8_t * buffer )
{
    Document document;
    document.Parse((char*) buffer);

    const Value& sender = document["sender"];
    const Value& receiver = document["receiver"];
    const Value& type = document["type"];
    const Value& payloadSize = document["payloadSize"];
    const Value& payload = document["payload"];

    UMLRTCommunicator::Message* msg = new UMLRTCommunicator::Message;
    msg->sender = sender.GetString();
    msg->receiver = receiver.GetString();
    msg->type = type.GetInt();
    msg->payloadSize = payloadSize.GetInt();
    msg->payload = (uint8_t*) malloc(msg->payloadSize);
        
//    memcpy(msg->payload, payload.GetString(), msg->payloadSize);
    
    for( int i=0; i<payload.Size(); i++ )
        msg->payload[i] = payload[i].GetUint();

    return msg;
}

UMLRTCommunicator::Message* UMLRTCommunicator::Message::fromFlatBuffer ( uint8_t * buffer )
{
    UMLRTCommunicator::Message* msg = new UMLRTCommunicator::Message;

    auto msgObj = FBSchema::GetMessage(buffer);
    msg->sender = strdup(msgObj->sender()->c_str());
    msg->receiver = strdup(msgObj->receiver()->c_str());
    msg->type = static_cast<int>(msgObj->type());
    msg->payloadSize = msgObj->payloadSize();

    if( msg->payloadSize != 0 )
    {
        auto payload = msgObj->payload();
        msg->payload = (uint8_t*) malloc(msg->payloadSize);
        for(int i=0; i<msg->payloadSize; i++)
            msg->payload[i] = payload->Get(i);
    }

    return msg;
}

bool UMLRTCommunicator::Message::isProtocolSignal ( )
{
    if( FBSchema::Type(this->type) == FBSchema::Type_PROTOCOL_SIGNAL )
        return true;
    return false;
}

bool UMLRTCommunicator::Message::isDeploymentSignal ( )
{
    if( FBSchema::Type(this->type) == FBSchema::Type_DEPLOYMENT_MAP )
        return true;
    return false;
}

bool UMLRTCommunicator::Message::isReadySignal ( )
{
    if( FBSchema::Type(this->type) == FBSchema::Type_READY_SIGNAL )
        return true;
    return false;
}

int UMLRTCommunicator::Signal::encode ( uint8_t ** buffer )
{
    int size = strcmp(FORMAT, "flatbuffer") == 0 ? toFlatBuffer(buffer) : toJSON(buffer);
    UMLRTExecutionDirector::total_messages_encoded++;
    UMLRTExecutionDirector::total_size += size;
    return size;
//    if(strcmp(FORMAT, "flatbuffer") == 0)
//        return toFlatBuffer(buffer);
//    return toJSON(buffer);
}

UMLRTCommunicator::Signal * UMLRTCommunicator::Signal::decode ( uint8_t * buffer )
{
    if(strcmp(FORMAT, "flatbuffer") == 0)
        return UMLRTCommunicator::Signal::fromFlatBuffer(buffer);
    return UMLRTCommunicator::Signal::fromJSON(buffer);
}

static void toFlatBuffer_param( std::vector<flatbuffers::Offset<FBSchema::Obj>> & accumulator, 
        const UMLRTObject_class * desc, void * data ) {

	if (desc->object.numFields != 0) {	// complex object
        std::vector<flatbuffers::Offset<FBSchema::Obj>> fields;

    	for (size_t i=0; i<desc->object.numFields; i++) {
            const UMLRTObject_field* field = &desc->object.fields[i];
            toFlatBuffer_param(fields, field->desc, (uint8_t*) data + field->offset);
        }
        
        auto fieldsVec = builder2.CreateVector(fields);
        FBSchema::ObjBuilder objBuilder(builder2);
        objBuilder.add_fields(fieldsVec);
        accumulator.push_back(objBuilder.Finish()); 
    }
    else
    {
        //TODO Handler other types!
        if(strcmp("charptr", desc->name) == 0)
        {
            //void* ptr;
            //desc->copy(desc, data, &ptr);
            //int length = strlen((char*) ptr);
            auto payload = builder2.CreateVector(*((uint8_t * *) data), strlen(*((const char * *) data)));
            FBSchema::ObjBuilder objBuilder(builder2);
            objBuilder.add_data(payload);
            accumulator.push_back(objBuilder.Finish());
        }
    }
}

static bool fromFlatBuffer_param( const FBSchema::Obj * param, const UMLRTObject_class * desc, uint8_t * buffer) {
    
    auto fields = param->fields();

    if ( fields != NULL ) {
		for (size_t i=0; i<desc->object.numFields; i++) {
	    	auto field = fields->Get(i);
	    	if(!fromFlatBuffer_param(field, desc->object.fields[i].desc, 
                        buffer + desc->object.fields[i].offset))
	    		return false;
	    }
    }
    else
    {
        auto paramData = param->data();
        uint8_t* data = (uint8_t*) malloc(paramData->Length());
        for(int i=0; i<paramData->Length(); i++)
            data[i] = paramData->Get(i);

        desc->copy(desc, &data, buffer);
        free(data);
    }

	return true;
}

int UMLRTCommunicator::Signal::toFlatBuffer ( uint8_t ** buffer )
{
    builder2.Clear();
    auto protocolName = builder2.CreateString(this->protocolName);
    auto signalName = builder2.CreateString(this->signalName);
    auto sourceSlot = builder2.CreateString(this->sourceSlot);
    auto destinationSlot = builder2.CreateString(this->destinationSlot);
    //auto payload = builder2.CreateVector(this->payload, this->payloadSize);

    const UMLRTObject * payloadObj = UMLRTSignalRegistry::getRegistry().
        getOutSignalPayloadObject(this->protocolName, this->signalName);

	if(payloadObj == NULL)
		FATAL("No payload object for signal %s of protocol %s\n", this->protocolName, this->signalName);

    std::vector<flatbuffers::Offset<FBSchema::Obj>> params;
    for(size_t i=0; i<payloadObj->numFields; i++) {
        toFlatBuffer_param(params, payloadObj->fields[i].desc, 
                &this->payload[payloadObj->fields[i].offset]);
    }

    auto paramsVec = builder2.CreateVector(params);

    FBSchema::SignalBuilder sigBuilder(builder2);
    sigBuilder.add_protocol(protocolName);
    sigBuilder.add_name(signalName);
    sigBuilder.add_sourceSlot(sourceSlot);
    sigBuilder.add_destinationSlot(destinationSlot);
    sigBuilder.add_sourcePort(this->sourcePort);
    sigBuilder.add_destinationPort(this->destinationPort);
    sigBuilder.add_isDestinationInternal(this->destinationInternal);
    sigBuilder.add_params(paramsVec);

//    sigBuilder.add_payloadSize(this->payloadSize);
//    sigBuilder.add_payload(payload);

    auto signal = sigBuilder.Finish();
    builder2.Finish(signal);

    *buffer = (uint8_t*) malloc(builder2.GetSize());
    memcpy(*buffer, builder2.GetBufferPointer(), builder2.GetSize());
    return builder2.GetSize();
}

int UMLRTCommunicator::Signal::toJSON ( uint8_t ** buffer )
{
    const UMLRTObject * payloadObj = UMLRTSignalRegistry::getRegistry().
        getOutSignalPayloadObject(this->protocolName, this->signalName);

	if(payloadObj == NULL)
		FATAL("No payload object for signal %s of protocol %s\n", this->protocolName, this->signalName);

    Document document;
    document.SetObject();
	Document::AllocatorType& allocator = document.GetAllocator();

    Value sourceSlot;
    Value destinationSlot;
    Value sourcePort;
    Value destinationPort;
    Value protocolName;
    Value signalName;
    Value destinationInternal;
    Value params(kArrayType);
//    Value payloadSize;
//    Value payload(kArrayType);

    sourceSlot.SetString(this->sourceSlot, allocator);
    destinationSlot.SetString(this->destinationSlot, allocator);
    protocolName.SetString(this->protocolName, allocator);
    signalName.SetString(this->signalName, allocator);
    sourcePort.SetInt(this->sourcePort);
    destinationPort.SetInt(this->destinationPort);
    destinationInternal.SetBool(this->destinationInternal);
//    payloadSize.SetInt(this->payloadSize);

//    for( int i=0; i<this->payloadSize; i++ )
//        payload.PushBack(Value().SetUint(this->payload[i]), allocator);

    for(size_t i=0; i<payloadObj->numFields; i++) {
		Value paramObj(kObjectType);
		
		Value paramName;
		paramName.SetString(payloadObj->fields[i].name, allocator);
		paramObj.AddMember("name", paramName, allocator);

		toJSON_param(document, paramObj, payloadObj->fields[i].desc, 
                &this->payload[payloadObj->fields[i].offset]);
		params.PushBack(paramObj, allocator);
	}

    document.AddMember("sourceSlot", sourceSlot, allocator);
    document.AddMember("destinationSlot", destinationSlot, allocator);
    document.AddMember("protocolName", protocolName, allocator);
    document.AddMember("signalName", signalName, allocator);
    document.AddMember("sourcePort", sourcePort, allocator);
    document.AddMember("destinationPort", destinationPort, allocator);
    document.AddMember("destinationInternal", destinationInternal, allocator);
    document.AddMember("params", params, allocator);

//    document.AddMember("payloadSize", payloadSize, allocator);
//    document.AddMember("payload", payload, allocator);

    StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
	document.Accept(writer);

    std::string json = strbuf.GetString();
    *buffer = (uint8_t*) malloc(json.length()+1);
    memcpy(*buffer, json.c_str(), json.length()+1);
    return json.length()+1;
}

bool UMLRTCommunicator::Signal::fromJSON_param( const Value & param, const UMLRTObject_class * desc, uint8_t * buffer) {
	const Value& paramVal = param["value"];

	if(param.HasMember("type") 
		&& strcmp(param["type"].GetString(), desc->name) != 0)
		return false;

	if(paramVal.IsArray()) {
		if(paramVal.Size() != desc->object.numFields)
			return false;

		for (size_t i=0; i<desc->object.numFields; i++) {
	    	const Value& paramObj = paramVal[i];

	    	if(!paramObj.HasMember("value"))
	    		return false;

	    	if(paramObj.HasMember("type") 
	    		&& !paramObj["type"].IsString())
	    		return false;

	    	if(!fromJSON_param(paramObj, desc->object.fields[i].desc, buffer + desc->object.fields[i].offset))
	    		return false;
	    }

	}

	else if(strcmp(desc->name, "bool") == 0) {
		if(!paramVal.IsBool())
			return false;

		bool b = paramVal.GetBool();
		desc->copy(desc, &b, buffer);
	}

	else if(strcmp(desc->name, "char") == 0) {
		if(!paramVal.IsString())
			return false;

		const char* str = paramVal.GetString();
		if(strlen(str) != 1)
			return false;

		char c = str[0];
		desc->copy(desc, &c, buffer);
	}

	else if(strcmp(desc->name, "double") == 0) {
		if(!paramVal.IsDouble())
			return false;

		double d = paramVal.GetDouble();
		desc->copy(desc, &d, buffer);
	}

	else if(strcmp(desc->name, "float") == 0) {
		if(!paramVal.IsFloat())
			return false;

		float f = paramVal.GetFloat();
		desc->copy(desc, &f, buffer);
	}

	else if(strcmp(desc->name, "int") == 0) {
		if(!paramVal.IsInt())
			return false;

		int i = paramVal.GetInt();
		desc->copy(desc, &i, buffer);
	}

	else if(strcmp(desc->name, "charptr") == 0) {
		if(!paramVal.IsString())
			return false;

		const char* str = paramVal.GetString();
		desc->copy(desc, &str, buffer);
	}

	else
		return false;

	return true;
}

void UMLRTCommunicator::Signal::toJSON_param( Document & document, Value & paramObj, const UMLRTObject_class * desc, void * data ) {
	Document::AllocatorType& allocator = document.GetAllocator();

	Value paramType;
	paramType.SetString(desc->name, allocator);
	paramObj.AddMember("type", paramType, allocator);

	if (desc->object.numFields != 0) {	// complex object
		Value valueArray(kArrayType);

    	for (size_t i=0; i<desc->object.numFields; i++) {
            const UMLRTObject_field* field = &desc->object.fields[i];
            
            Value subParamObj(kObjectType);

            Value paramName;
			paramName.SetString(field->name, allocator);
			subParamObj.AddMember("name", paramName, allocator);

            toJSON_param(document, subParamObj, field->desc, (uint8_t*) data + field->offset);
			valueArray.PushBack(subParamObj, allocator);
        }

       	paramObj.AddMember("value", valueArray, allocator);
    }

	else if(strcmp(desc->name, "bool") == 0) {
	    bool b;
	    desc->copy(desc, data, &b);

	    Value paramValue;
		paramValue.SetBool(b);
		paramObj.AddMember("value", paramValue, allocator);
	}

	else if(strcmp(desc->name, "char") == 0) {
	    Value paramValue;
		paramValue.SetString((char*) data, 1, allocator);
		paramObj.AddMember("value", paramValue, allocator);
	}

	else if(strcmp(desc->name, "double") == 0) {
	    double d;
        desc->copy(desc, data, &d);

		
	    Value paramValue;
		paramValue.SetDouble(d);
		paramObj.AddMember("value", paramValue, allocator);	    
	}

	else if(strcmp(desc->name, "float") == 0) {
		float f;
        desc->copy(desc, data, &f);


	    Value paramValue;
		paramValue.SetFloat(f);
		paramObj.AddMember("value", paramValue, allocator);	
	}

	else if(strcmp(desc->name, "int") == 0) {
		int iv;
        desc->copy(desc, data, &iv);


	    Value paramValue;
		paramValue.SetInt(iv);
		paramObj.AddMember("value", paramValue, allocator);
	}

	else if(strcmp(desc->name, "charptr") == 0) {
		//void* ptr;
		//desc->copy(desc, data, &ptr);

		Value paramValue;
		paramValue.SetString(*((const char * *) data), allocator);
		paramObj.AddMember("value", paramValue, allocator);

		//desc->destroy(desc, &ptr);
	}

	else {
		FATAL("WARNING: JSON encoder skipping unsupported type '%s'\n", desc->name);
	}
}

UMLRTCommunicator::Signal* UMLRTCommunicator::Signal::fromJSON ( uint8_t * buffer )
{
    Document document;
    document.Parse((char*) buffer);

    const Value& sourceSlot = document["sourceSlot"];
    const Value& destinationSlot = document["destinationSlot"];
    const Value& sourcePort = document["sourcePort"];
    const Value& destinationPort = document["destinationPort"];
    const Value& protocolName = document["protocolName"];
    const Value& signalName = document["signalName"];
    const Value& destinationInternal = document["destinationInternal"];
    const Value& params = document["params"];    

//    const Value& payloadSize = document["payloadSize"];
//    const Value& payload = document["payload"];

    UMLRTCommunicator::Signal* sig = new UMLRTCommunicator::Signal;
    sig->sourceSlot = sourceSlot.GetString();
    sig->destinationSlot = destinationSlot.GetString();
    sig->sourcePort = sourcePort.GetInt();
    sig->destinationPort = destinationPort.GetInt();
    sig->protocolName = protocolName.GetString();
    sig->signalName = signalName.GetString();
    sig->destinationInternal = destinationInternal.GetBool();
    
    UMLRTObject * payloadObj = UMLRTSignalRegistry::getRegistry().getInSignalPayloadObject(
            sig->protocolName, sig->signalName);
    sig->payload = (uint8_t*) malloc(sizeof(uint8_t) * payloadObj->sizeOf);

    for (size_t i=0; i<payloadObj->numFields; i++)
    {
        const Value& param = params[i];
        fromJSON_param(param, payloadObj->fields[i].desc, sig->payload + payloadObj->fields[i].offset);
    }


//    sig->payloadSize = payloadSize.GetInt();

//    sig->payload = (uint8_t*) malloc(sig->payloadSize);
//    for( int i=0; i<payload.Size(); i++ )
//        sig->payload[i] = payload[i].GetUint();

    return sig;
}

UMLRTCommunicator::Signal * UMLRTCommunicator::Signal::fromFlatBuffer ( uint8_t * buffer )
{
   
    UMLRTCommunicator::Signal * signal = new UMLRTCommunicator::Signal;

    auto signalObj = FBSchema::GetSignal(buffer);
    //auto payload = signalObj->payload();
    auto params = signalObj->params();

    signal->protocolName = signalObj->protocol()->c_str(); 
    signal->signalName = signalObj->name()->c_str();
    signal->sourceSlot = signalObj->sourceSlot()->c_str();
    signal->destinationSlot = signalObj->destinationSlot()->c_str();
    signal->sourcePort = signalObj->sourcePort();
    signal->destinationPort = signalObj->destinationPort();
    signal->destinationInternal = signalObj->isDestinationInternal();

    UMLRTObject * payloadObj = UMLRTSignalRegistry::getRegistry().getInSignalPayloadObject(
            signal->protocolName, signal->signalName);
    signal->payload = (uint8_t*) malloc(sizeof(uint8_t) * payloadObj->sizeOf);

    for (size_t i=0; i<payloadObj->numFields; i++)
    {
        auto param = params->Get(i);
        fromFlatBuffer_param(param, payloadObj->fields[i].desc, signal->payload + payloadObj->fields[i].offset);
    }
//    signal->payloadSize = signalObj->payloadSize();
//
//    signal->payload = (uint8_t*) malloc(payload->Length());
//    for(int i=0; i<payload->Length(); i++)
//        signal->payload[i] = payload->Get(i);
    
    return signal;
}
