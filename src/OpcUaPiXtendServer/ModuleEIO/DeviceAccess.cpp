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

#include <OpcUaPiXtendServer/ModuleEIO/DeviceAccess.h>
#include "OpcUaStackCore/Base/Log.h"

using namespace OpcUaStackCore;

namespace OpcUaPiXtendServer
{

    DeviceAccess::DeviceAccess(DeviceAccessType deviceAccessType)
    : deviceAccessType_(deviceAccessType)
    {
    }

    DeviceAccess::~DeviceAccess(void)
    {
    }

    std::string
	DeviceAccess::device(void)
    {
    	return device_;
    }

    DeviceAccessType
	DeviceAccess::deviceAccessType(void)
    {
      	return deviceAccessType_;
    }

    uint32_t&
	DeviceAccess::reference(void)
    {
    	return reference_;
    }

}
