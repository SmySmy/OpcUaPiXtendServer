/*
   Copyright 2021 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
          Samuel Huebl (Samuel@huebl-sgh.de)
 */

#include "OpcUaStackCore/BuildInTypes/OpcUaIdentifier.h"
#include "OpcUaStackServer/ServiceSetApplication/CreateObjectInstance.h"
#include "OpcUaStackServer/ServiceSetApplication/ForwardNodeSync.h"
#include "OpcUaStackServer/ServiceSetApplication/RegisterForwardNode.h"
#include "OpcUaPiXtendServer/OpcUaServer/PiXtendBaseServer.h"
#include "OpcUaPiXtendServer/OpcUaServer/NodeContext.h"
#include "OpcUaPiXtendServer/PiXtend/PiXtendValueContext.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaPiXtendServer
{

    PiXtendBaseServer::PiXtendBaseServer(
    	const std::string& typeName,
		uint32_t typeNodeId
	)
    : typeName_(typeName)
    , typeNodeId_(typeNodeId)
    {
    }

    PiXtendBaseServer::~PiXtendBaseServer(void)
    {
    }

    bool
	PiXtendBaseServer::startup(
        OpcUaStackCore::IOThread::SPtr& ioThread,
        boost::shared_ptr<boost::asio::io_service::strand>& strand,
		OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
		const std::string& instanceName,
		const std::string& namespaceName,
		uint16_t namespaceIndex,
		const OpcUaStackCore::OpcUaNodeId& parentNodeId,
		ContextIndex::SPtr& contextIndex
	)
    {
    	ioThread_ = ioThread;
    	strand_ = strand;
    	applicationServiceIf_ = applicationServiceIf;
    	instanceName_ = instanceName;
    	namespaceName_ = namespaceName;
    	namespaceIndex_ = namespaceIndex;
    	parentNodeId_ = parentNodeId;
    	contextIndex_ = contextIndex;

    	// call specific startup handler
    	if (!handleStartup()) {
    		return false;
    	}

    	// create node context
    	createNodeContext(nodePinConfigVec_);

    	// create object instance in information model
    	if (!createObjectInstance()) {
    		return false;
    	}

    	// register opc ua service functions
    	if (!registerServiceFunctions()) {
    		return false;
    	}

    	return true;
    }

    bool
	PiXtendBaseServer::shutdown(void)
    {
    	// shutdown pixtend interface
    	if (!handleShutdown()) {
    		return false;
    	}

    	return true;
    }

    bool
	PiXtendBaseServer::createObjectInstance(void)
    {
   		//
		// create v2s object instance in opc ua information model
		//
		objectTypeNamespaceName(namespaceName_);
		objectTypeNodeId(OpcUaNodeId(typeNodeId_, namespaceIndex_));
		Object::SPtr obj = shared_from_this();
		CreateObjectInstance createObjectInstance(
			namespaceName_,									// namespace name of the object instance
			OpcUaLocalizedText("", instanceName_),			// display name of the object instance
			parentNodeId_,									// parent node of the object instance
			OpcUaNodeId(OpcUaId_Organizes),					// reference type between object and variable instance
			obj
		);
		if (!createObjectInstance.query(applicationServiceIf_)) {
			Log(Error, "create object response error (query)")
				.parameter("TypeName", typeName_);
			return false;
		}
		if (createObjectInstance.resultCode() != Success) {
			Log(Error, "create PiXtendV2S object response error (result code)")
			    .parameter("ResultCode", createObjectInstance.resultCode())
				.parameter("TypeName", typeName_);
			return false;
		}

    	return true;
    }

    bool PiXtendBaseServer::createNodeContext(
     	const NodePinConfig::Vec& nodePinConfigVec
	)
    {
    	for (auto nodePinConfig : nodePinConfigVec) {
        	// create server variable and register server variable
           	auto serverVariable = boost::make_shared<ServerVariable>(nodePinConfig.nodeName_);
        	serverVariables().registerServerVariable(serverVariable);

        	// find context
        	auto contextName = instanceName_ + "." + nodePinConfig.pinName_;
        	auto hardwareContext = contextIndex_->getContext(contextName);
        	if (hardwareContext.get() == nullptr) {
        		Log(Error, "context not exist in context index")
        			.parameter("ContextName", contextName);
        		return false;
        	}

        	// create node context
        	auto nodeContext = boost::make_shared<NodeContext>();
        	nodeContext->serverVariable(serverVariable);
        	nodeContext->hardwareContext(hardwareContext);
        	BaseClass::SPtr context = nodeContext;
        	serverVariable->applicationContext(context);
    	}

    	return true;
    }

    bool
	PiXtendBaseServer::registerServiceFunctions(void)
    {
    	// get nodeids from digital and analog variables
    	for (auto serverVariable : serverVariables().serverVariableMap()) {
    		auto variable = serverVariable.second;
    		auto applicationContext = variable->applicationContext();
    		if (applicationContext.get() == nullptr) continue;
    		auto nodeContext = boost::static_pointer_cast<NodeContext>(applicationContext);

    		// register service functions
    		RegisterForwardNode registerForwardNode(variable->nodeId());
    		registerForwardNode.addApplicationContext(applicationContext);
    		registerForwardNode.setReadCallback(boost::bind(&PiXtendBaseServer::readValue, this, _1));
    		registerForwardNode.setWriteCallback(boost::bind(&PiXtendBaseServer::writeValue, this, _1));
    		registerForwardNode.setMonitoredItemStartCallback(boost::bind(&PiXtendBaseServer::receiveMonotoredItemStart, this, _1));
    		registerForwardNode.setMonitoredItemStopCallback(boost::bind(&PiXtendBaseServer::receiveMonitoredItemStop, this, _1));
    		if (!registerForwardNode.query(applicationServiceIf_, true)) {
    		    Log(Error, "register forward node response error")
    			    .parameter("NodeId", variable->nodeId())
					.parameter("NodeName", serverVariable.first)
					.parameter("ModulName", instanceName_);
    			return false;

    		}
    	}

    	return true;
    }

    void
	PiXtendBaseServer::readValue(
    	ApplicationReadContext* applicationReadContext
	)
    {
    	// get node context
    	auto nodeContext = boost::static_pointer_cast<NodeContext>(applicationReadContext->applicationContext_);
    	if (!nodeContext) {
       		applicationReadContext->statusCode_ = BadUnexpectedError;
        	return;
    	}

    	// get hardware context
    	auto hardwareContext = boost::static_pointer_cast<PiXtendValueContext>(nodeContext->hardwareContext());
    	if (!hardwareContext) {
       		applicationReadContext->statusCode_ = BadUnexpectedError;
        	return;
    	}

    	// read pixtend variable
    	applicationReadContext->dataValue_ = hardwareContext->dataValue();
    	applicationReadContext->statusCode_ = Success;
    }

    void
	PiXtendBaseServer::writeValue(
		ApplicationWriteContext* applicationWriteContext
	)
    {
       	// get node context
        auto nodeContext = boost::static_pointer_cast<NodeContext>(applicationWriteContext->applicationContext_);
        if (!nodeContext) {
        	applicationWriteContext->statusCode_ = BadUnexpectedError;
            return;
        }

        // get hardware context
        auto hardwareContext = boost::static_pointer_cast<PiXtendValueContext>(nodeContext->hardwareContext());
        if (!hardwareContext) {
        	applicationWriteContext->statusCode_ = BadUnexpectedError;
            return;
        }

    	// check if write function exist
    	if (!hardwareContext->writeAccess()) {
    		applicationWriteContext->statusCode_ = BadUserAccessDenied;
    		return;
    	}

        // write pixtend variable
    	hardwareContext->dataValue(applicationWriteContext->dataValue_);
    	applicationWriteContext->statusCode_ = Success;
    }

	void
	PiXtendBaseServer::receiveMonotoredItemStart(
		OpcUaStackCore::ApplicationMonitoredItemStartContext* monitoredItemStartContext
	)
	{
		// FIXME: todo
	}

	void
	PiXtendBaseServer::receiveMonitoredItemStop(
		OpcUaStackCore::ApplicationMonitoredItemStopContext* monitoredItemStopContext
	)
	{
		// FIXME: todo
	}

}