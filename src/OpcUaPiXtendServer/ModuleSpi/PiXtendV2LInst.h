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

#ifndef __OpcUaPiXtendServer_PiXtendV2LInst_h__
#define __OpcUaPiXtendServer_PiXtendV2LInst_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaPiXtendServer/PiXtend/PiXtendV2L.h"
#include "OpcUaPiXtendServer/ModuleSpi/PiXtendHardwareAccessSpi.h"
#include "OpcUaPiXtendServer/ModuleSpi/PiXtendSpiHelper.h"

namespace OpcUaPiXtendServer
{

    class PiXtendV2LInst
    : public PiXtendV2L
    {
      public:
        using SPtr = boost::shared_ptr<PiXtendV2LInst>;

        PiXtendV2LInst(const std::string& instanceName);
        virtual ~PiXtendV2LInst(void);

        // Configuration

        virtual bool startup(void) override;
        virtual bool shutdown(void) override;
        virtual void handleHardwareAccess(void) override;

        // Status information

        virtual uint8_t firmware(void) override;
        virtual uint8_t hardware(void) override;
        virtual uint8_t model(void) override;
        virtual uint8_t ucStatus(void) override;
        virtual uint8_t ucWarning(void) override;

        // Inputs Analog

        virtual double ai0(void) override;
        virtual double ai1(void) override;
        virtual double ai2(void) override;
        virtual double ai3(void) override;
        virtual double ai4(void) override;
        virtual double ai5(void) override;

        // Output Analog

        virtual void ao0(double data) override;
        virtual double ao0(void) override;
        virtual void ao1(double data) override;
        virtual double ao1(void) override;

        // Inputs Digital

        virtual bool di0(void) override;
        virtual bool di1(void) override;
        virtual bool di2(void) override;
        virtual bool di3(void) override;
        virtual bool di4(void) override;
        virtual bool di5(void) override;
        virtual bool di6(void) override;
        virtual bool di7(void) override;
        virtual bool di8(void) override;
        virtual bool di9(void) override;
        virtual bool di10(void) override;
        virtual bool di11(void) override;
        virtual bool di12(void) override;
        virtual bool di13(void) override;
        virtual bool di14(void) override;
        virtual bool di15(void) override;

        // Output Digital

        virtual void do0(bool data) override;
        virtual bool do0(void) override;
        virtual void do1(bool data) override;
        virtual bool do1(void) override;
        virtual void do2(bool data) override;
        virtual bool do2(void) override;
        virtual void do3(bool data) override;
        virtual bool do3(void) override;
        virtual void do4(bool data) override;
        virtual bool do4(void) override;
        virtual void do5(bool data) override;
        virtual bool do5(void) override;
        virtual void do6(bool data) override;
        virtual bool do6(void) override;
        virtual void do7(bool data) override;
        virtual bool do7(void) override;
        virtual void do8(bool data) override;
        virtual bool do8(void) override;
        virtual void do9(bool data) override;
        virtual bool do9(void) override;
        virtual void do10(bool data) override;
        virtual bool do10(void) override;
        virtual void do11(bool data) override;
        virtual bool do11(void) override;

        // Relay

        virtual void relay0(bool data) override;
        virtual bool relay0(void) override;
        virtual void relay1(bool data) override;
        virtual bool relay1(void) override;
        virtual void relay2(bool data) override;
        virtual bool relay2(void) override;
        virtual void relay3(bool data) override;
        virtual bool relay3(void) override;

        // Input/Output General Purpose

        virtual void gpio0(bool data) override;
        virtual bool gpio0(void) override;
        virtual void gpio1(bool data) override;
        virtual bool gpio1(void) override;
        virtual void gpio2(bool data) override;
        virtual bool gpio2(void) override;
        virtual void gpio3(bool data) override;
        virtual bool gpio3(void) override;

      private:
        // defines the model id: PiXtend V2 -L- = 76
        const uint8_t model_ {76};
        // defines the current hardware version 2.1 = 21
        const uint8_t hardware_ {21};
        // defines the delay time in <ms> - should be part of the configuration later
        const uint32_t delayTime_ {100};

        PiXtendSpiHelper spiHelper_;

        bool checkTrxValue(uint32_t trxValue);

        using PiXtendSpiInputData = pixtInV2L;
        using PiXtendSpiOutputData = pixtOutV2L;
        using PiXtendSpiOutputDataDac = pixtOutDAC;

        PiXtendSpiInputData spiInputData_;
        PiXtendSpiOutputData spiOutputData_;
        PiXtendSpiOutputDataDac spiOutputDataDac_;

        void resetSpiInputData(PiXtendSpiInputData& spiInputData);
        void resetSpiOutputData(PiXtendSpiOutputData& spiOutputData);
        void resetSpiOutputDataDac(PiXtendSpiOutputDataDac& spiOutputDataDac);
    };

}

#endif
