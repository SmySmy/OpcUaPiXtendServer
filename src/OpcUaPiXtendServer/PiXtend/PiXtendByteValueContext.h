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

#ifndef __OpcUaPiXtendServer_PiXtendByteValueContext_h__
#define __OpcUaPiXtendServer_PiXtendByteValueContext_h__

#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include "OpcUaPiXtendServer/PiXtend/PiXtendValueContext.h"

namespace OpcUaPiXtendServer
{

    class PiXtendByteValueContext
    : public PiXtendValueContext
    {
      public:
        using SPtr = boost::shared_ptr<PiXtendByteValueContext>;
        using ReadFunc = std::function<uint8_t (void)>;
        using WriteFunc = std::function<void (uint8_t)>;

        PiXtendByteValueContext(void);
        virtual ~PiXtendByteValueContext(void);

        static PiXtendByteValueContext::SPtr createContext(
             ReadFunc readFunc,
             WriteFunc writeFunc
        );

        virtual void dataValueToOutputStruct(void) override;
        virtual void inputStructToDataValue(void) override;

        void readFunc(ReadFunc readFunc);
        ReadFunc readFunc(void);
        void writeFunc(WriteFunc writeFunc);
        WriteFunc writeFunc(void);

      private:
        ReadFunc readFunc_;
        WriteFunc writeFunc_;
    };

}

#endif
