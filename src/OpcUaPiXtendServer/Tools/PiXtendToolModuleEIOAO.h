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

#ifndef __OpcUaPiXtendServer_PiXtendToolModuleEIOAO_h__
#define __OpcUaPiXtendServer_PiXtendToolModuleEIOAO_h__

#include "OpcUaPiXtendServer/PiXtend/PiXtendEIOAO.h"
#include "OpcUaPiXtendServer/Tools/PiXtendToolModule.h"

namespace OpcUaPiXtendServer
{

    class PiXtendToolModuleEIOAO
    : public PiXtendToolModule
    {
      public:
        using SPtr = boost::shared_ptr<PiXtendToolModuleEIOAO>;

        PiXtendToolModuleEIOAO(const std::string& name, ModuleAddress moduleAddress);
        virtual ~PiXtendToolModuleEIOAO(void);

        virtual bool readPin(Pins pin, PiXtendToolReadStruct& result) override;

        virtual bool writeAnalogPin(Pins pin, AValue data) override;
        virtual bool writeDigitalPin(Pins pin, DValue data) override;

      private:
        PiXtendEIOAO::SPtr pixtendSPtr_;
    };

}

#endif
