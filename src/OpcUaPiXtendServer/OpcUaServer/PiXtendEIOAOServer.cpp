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
#include "OpcUaPiXtendServer/OpcUaServer/PiXtendEIOAOServer.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaPiXtendServer
{

    PiXtendEIOAOServer::PiXtendEIOAOServer(void)
    {
        ServerVariable::SPtr serverVariable;

        // register analog input variables
        serverVariable = boost::make_shared<ServerVariable>("AI_AI0_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AI_AI1_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AI_AI2_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AI_AI3_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AI_AI4_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AI_AI5_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AI_AI6_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AI_AI7_Variable");
        serverVariables().registerServerVariable(serverVariable);

        // register analog output variables
        serverVariable = boost::make_shared<ServerVariable>("AO_AO0_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AO_AO1_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AO_AO2_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AO_AO3_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AO_AO4_Variable");
        serverVariables().registerServerVariable(serverVariable);
        serverVariable = boost::make_shared<ServerVariable>("AO_AO5_Variable");
        serverVariables().registerServerVariable(serverVariable);
    }

    PiXtendEIOAOServer::~PiXtendEIOAOServer(void)
    {
    }

    bool
	PiXtendEIOAOServer::startup(
	    OpcUaStackServer::ApplicationServiceIf* applicationServiceIf,
	    const std::string& instanceName,
	    const std::string& namespaceName,
	    uint16_t namespaceIndex,
	    const OpcUaStackCore::OpcUaNodeId& parentNodeId,
	    uint32_t moduleAddress
	)
    {
        applicationServiceIf_ = applicationServiceIf;
        instanceName_ = instanceName;
        namespaceName_ = namespaceName;
        namespaceIndex_ = namespaceIndex;
        parentNodeId_ = parentNodeId;

    	// get pixtend eIO AO access interface
    	pixtend_ = PiXtendModulesFactory::createPiXtendEIOAO();

    	// startup pixtend interface
    	pixtend_->startup(moduleAddress);

        // create object instance in information model
        if (!createObjectInstance()) {
            return false;
        }

        return true;
    }

    bool
	PiXtendEIOAOServer::shutdown(void)
    {
    	// shutdown pixtend interface
    	pixtend_->shutdown();

    	// FIXME: TBD

    	return true;
    }

    bool
    PiXtendEIOAOServer::createObjectInstance(void)
    {
        //
        // create v2s object instance in opc ua information model
        //
        objectTypeNamespaceName(namespaceName_);
        objectTypeNodeId(OpcUaNodeId(1002, namespaceIndex_));
        Object::SPtr obj = shared_from_this();
        CreateObjectInstance createObjectInstance(
            namespaceName_,                                 // namespace name of the object instance
            OpcUaLocalizedText("", instanceName_),          // display name of the object instance
            parentNodeId_,                                  // parent node of the object instance
            OpcUaNodeId(OpcUaId_Organizes),                 // reference type between object and variable instance
            obj
        );
        if (!createObjectInstance.query(applicationServiceIf_)) {
            Log(Error, "create PiXtendV2S object response error (query)");
            return false;
        }
        if (createObjectInstance.resultCode() != Success) {
            Log(Error, "create PiXtendV2S object response error (result code)")
                .parameter("ResultCode", createObjectInstance.resultCode());
            return false;
        }

        return true;
    }

}
