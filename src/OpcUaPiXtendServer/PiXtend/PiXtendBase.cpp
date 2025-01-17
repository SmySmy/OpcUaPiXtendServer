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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaPiXtendServer/PiXtend/PiXtendBase.h"

using namespace OpcUaStackCore;

namespace OpcUaPiXtendServer
{

    PiXtendBase::PiXtendBase(const std::string& instanceName)
    : instanceName_(instanceName)
    {
    }

    PiXtendBase::~PiXtendBase(void)
    {
    }

    void
	PiXtendBase::loop(void)
    {
    	// set all input variables
    	for ( auto hardwareContext : piXtendValueContextVec_) {
    		if (!hardwareContext->writeAccess()) continue;
    		hardwareContext->dataValueToOutputStruct();
    	}

    	// call hardware access process
    	handleHardwareAccess();

    	// get all input variables
    	for ( auto hardwareContext : piXtendValueContextVec_) {
    		hardwareContext->inputStructToDataValue();
    	}
    }

    void
	PiXtendBase::contextIndex(ContextIndex::SPtr& contextIndex)
    {
    	contextIndex_ = contextIndex;
    	handleRegisterContext();
    }

    std::string
	PiXtendBase::instanceName(void)
    {
    	return instanceName_;
    }

    ContextIndex::SPtr
	PiXtendBase::contextIndex(void)
    {
    	return contextIndex_;
    }

    bool
	PiXtendBase::registerContext(
    	const std::string& contextName,
		PiXtendValueContext::SPtr& piXtendValueContext
    )
    {
    	// check if context already exist
    	if (contextIndex_->existContext(contextName)) {
    		Log(Error, "context already exist in context index")
    			.parameter("ContextName", contextName);
    		return false;
    	}

    	// add new context to context index
	piXtendValueContext->contextName(contextName);
    	BaseClass::SPtr context = piXtendValueContext;
    	contextIndex_->registerContext(contextName, context);

    	// add new context to local vector
    	piXtendValueContextVec_.push_back(piXtendValueContext);

    	return true;
    }

    bool
	PiXtendBase::registerContext(const CfgElementVec& cfgElementVec)
    {
    	for (auto cfgElement : cfgElementVec) {
    		auto name = instanceName_ + "." + cfgElement.first;
    		auto rc = registerContext(name, cfgElement.second);
    		if (!rc) return false;
    	}
    	return true;
    }

    double
	PiXtendBase::analog2Percent(uint16_t value, uint8_t base)
    {
    	uint32_t baseValue = pow(2,base);

    	if (value > baseValue) value = baseValue;

    	return (double)value / (double)baseValue;
    }

    uint16_t
	PiXtendBase::percent2Analog(double value, uint8_t base)
    {
    	if (value < 0.0) value = 0.0;
    	if (value > 1.0) value = 1.0;

    	return static_cast<uint16_t>(value * (double)pow(2,base));
    }

}
