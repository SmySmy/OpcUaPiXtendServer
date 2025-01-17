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

#include "OpcUaPiXtendServer/ModuleSpi/PiXtendV2SInst.h"
#include "OpcUaStackCore/Base/Log.h"

using namespace OpcUaStackCore;

namespace OpcUaPiXtendServer
{

    PiXtendV2SInst::PiXtendV2SInst(const std::string& instanceName)
    : PiXtendV2S(instanceName)
    {
    }

    PiXtendV2SInst::~PiXtendV2SInst(void)
    {
    }

    // Configuration

    bool
    PiXtendV2SInst::startup(void)
    {
        resetSpiInputData(spiInputData_);
        resetSpiOutputData(spiOutputData_);
        resetSpiOutputDataDac(spiOutputDataDac_);

        //Configure SPI
        Spi_SetupV2(0); // communication with microcontroller
        Spi_SetupV2(1); // communication with DAC (analog inputs/outputs)

        // read first time - configuration
        int32_t returnValue = Spi_AutoModeV2S(&spiOutputData_, &spiInputData_);
        Spi_AutoModeDAC(&spiOutputDataDac_);

        if (!spiHelper_.checkPixtendReturnValue(returnValue))
        {
            Log(Error, "communication error in module v2s");
            return false;
        }

        if (spiInputData_.byModelIn != model_)
        {
            Log(Error, "receive undefined model in module v2s")
                .parameter("expected model", model_)
                .parameter("received model", spiInputData_.byModelIn);
            return false;
        }

        if (spiInputData_.byHardware != hardware_)
        {
            Log(Error, "different hardware versions in module v2s")
                .parameter("expected hardware", hardware_)
                .parameter("received hardware", spiInputData_.byHardware);
            return false;
        }

        return true;
    }

    bool
    PiXtendV2SInst::shutdown(void)
    {
        return true;
    }

    void
	PiXtendV2SInst::handleHardwareAccess(void)
    {
        // cycle time in <ms>
        delay(delayTime_);

        // transfer data
        int32_t returnValue = Spi_AutoModeV2S(&spiOutputData_, &spiInputData_);
        Spi_AutoModeDAC(&spiOutputDataDac_);

        if (!spiHelper_.checkPixtendReturnValue(returnValue))
        {
            Log(Error, "communication error in module v2s");
        }
    }

    // Status information

    uint8_t
    PiXtendV2SInst::firmware(void)
    {
        return spiInputData_.byFirmware;
    }

    uint8_t
    PiXtendV2SInst::hardware(void)
    {
        return spiInputData_.byHardware;
    }

    uint8_t
    PiXtendV2SInst::model(void)
    {
        return spiInputData_.byModelIn;
    }

    uint8_t
    PiXtendV2SInst::ucStatus(void)
    {
        return spiInputData_.byUCState;
    }

    uint8_t
    PiXtendV2SInst::ucWarning(void)
    {
        return spiInputData_.byUCWarnings;
    }

    // Inputs Analog

    double
    PiXtendV2SInst::ai0(void)
    {
        return spiHelper_.analog2Percent(spiInputData_.wAnalogIn0);
    }

    double
    PiXtendV2SInst::ai1(void)
    {
        return spiHelper_.analog2Percent(spiInputData_.wAnalogIn1);
    }

    // Output Analog

    void
    PiXtendV2SInst::ao0(double data)
    {
        spiOutputDataDac_.wAOut0 = spiHelper_.percent2Analog(data);
    }

    double
    PiXtendV2SInst::ao0(void)
    {
        return spiHelper_.analog2Percent(spiOutputDataDac_.wAOut0);
    }

    void
    PiXtendV2SInst::ao1(double data)
    {
        spiOutputDataDac_.wAOut1 = spiHelper_.percent2Analog(data);
    }

    double
    PiXtendV2SInst::ao1(void)
    {
        return spiHelper_.analog2Percent(spiOutputDataDac_.wAOut1);
    }

    // Inputs Digital

    bool
    PiXtendV2SInst::di0(void)
    {
        return spiHelper_.bit2Bool(spiInputData_.byDigitalIn, 0);
    }

    bool
    PiXtendV2SInst::di1(void)
    {
        return spiHelper_.bit2Bool(spiInputData_.byDigitalIn, 1);
    }

    bool
    PiXtendV2SInst::di2(void)
    {
        return spiHelper_.bit2Bool(spiInputData_.byDigitalIn, 2);
    }

    bool
    PiXtendV2SInst::di3(void)
    {
        return spiHelper_.bit2Bool(spiInputData_.byDigitalIn, 3);
    }

    bool
    PiXtendV2SInst::di4(void)
    {
        return spiHelper_.bit2Bool(spiInputData_.byDigitalIn, 4);
    }

    bool
    PiXtendV2SInst::di5(void)
    {
        return spiHelper_.bit2Bool(spiInputData_.byDigitalIn, 5);
    }

    bool
    PiXtendV2SInst::di6(void)
    {
        return spiHelper_.bit2Bool(spiInputData_.byDigitalIn, 6);
    }

    bool
    PiXtendV2SInst::di7(void)
    {
        return spiHelper_.bit2Bool(spiInputData_.byDigitalIn, 7);
    }

    // Output Digital

    void
    PiXtendV2SInst::do0(bool data)
    {
        uint8_t bit = (data) ? 0x1 : 0x0;
        spiHelper_.changeBit(spiOutputData_.byDigitalOut, bit, 0);
    }

    bool
    PiXtendV2SInst::do0(void)
    {
        return spiHelper_.bit2Bool(spiOutputData_.byDigitalOut, 0);
    }

    void
    PiXtendV2SInst::do1(bool data)
    {
        uint8_t bit = (data) ? 0x1 : 0x0;
        spiHelper_.changeBit(spiOutputData_.byDigitalOut, bit, 1);
    }

    bool
    PiXtendV2SInst::do1(void)
    {
        return spiHelper_.bit2Bool(spiOutputData_.byDigitalOut, 1);
    }

    void
    PiXtendV2SInst::do2(bool data)
    {
        uint8_t bit = (data) ? 0x1 : 0x0;
        spiHelper_.changeBit(spiOutputData_.byDigitalOut, bit, 2);
    }

    bool
    PiXtendV2SInst::do2(void)
    {
        return spiHelper_.bit2Bool(spiOutputData_.byDigitalOut, 2);
    }

    void
    PiXtendV2SInst::do3(bool data)
    {
        uint8_t bit = (data) ? 0x1 : 0x0;
        spiHelper_.changeBit(spiOutputData_.byDigitalOut, bit, 3);
    }

    bool
    PiXtendV2SInst::do3(void)
    {
        return spiHelper_.bit2Bool(spiOutputData_.byDigitalOut, 3);
    }

    // Relay

    void
    PiXtendV2SInst::relay0(bool data)
    {
        uint8_t bit = (data) ? 0x1 : 0x0;
        spiHelper_.changeBit(spiOutputData_.byRelayOut, bit, 0);
    }

    bool
    PiXtendV2SInst::relay0(void)
    {
        return spiHelper_.bit2Bool(spiOutputData_.byRelayOut, 0);
    }

    void
    PiXtendV2SInst::relay1(bool data)
    {
        uint8_t bit = (data) ? 0x1 : 0x0;
        spiHelper_.changeBit(spiOutputData_.byRelayOut, bit, 1);
    }

    bool
    PiXtendV2SInst::relay1(void)
    {
        return spiHelper_.bit2Bool(spiOutputData_.byRelayOut, 1);
    }

    void
    PiXtendV2SInst::relay2(bool data)
    {
        uint8_t bit = (data) ? 0x1 : 0x0;
        spiHelper_.changeBit(spiOutputData_.byRelayOut, bit, 2);
    }

    bool
    PiXtendV2SInst::relay2(void)
    {
        return spiHelper_.bit2Bool(spiOutputData_.byRelayOut, 2);
    }

    void
    PiXtendV2SInst::relay3(bool data)
    {
        uint8_t bit = (data) ? 0x1 : 0x0;
        spiHelper_.changeBit(spiOutputData_.byRelayOut, bit, 3);
    }

    bool
    PiXtendV2SInst::relay3(void)
    {
        return spiHelper_.bit2Bool(spiOutputData_.byRelayOut, 3);
    }

    // Input/Output General Purpose

    void
    PiXtendV2SInst::gpio0(bool data)
    {
        Log(Warning, "not implemented in module v2s");
    }

    bool
    PiXtendV2SInst::gpio0(void)
    {
        Log(Warning, "not implemented in module v2s");
        return false;
    }

    void
    PiXtendV2SInst::gpio1(bool data)
    {
        Log(Warning, "not implemented in module v2s");
    }

    bool
    PiXtendV2SInst::gpio1(void)
    {
        Log(Warning, "not implemented in module v2s");
        return false;
    }

    void
    PiXtendV2SInst::gpio2(bool data)
    {
        Log(Warning, "not implemented in module v2s");
    }

    bool
    PiXtendV2SInst::gpio2(void)
    {
        Log(Warning, "not implemented in module v2s");
        return false;
    }

    void
    PiXtendV2SInst::gpio3(bool data)
    {
        Log(Warning, "not implemented in module v2s");
    }

    bool
    PiXtendV2SInst::gpio3(void)
    {
        Log(Warning, "not implemented in module v2s");
        return false;
    }

    void
    PiXtendV2SInst::resetSpiInputData(PiXtendSpiInputData& spiInputData)
    {
        spiInputData.byFirmware = 0;
        spiInputData.byHardware = 0;
        spiInputData.byModelIn = 0;
        spiInputData.byUCState = 0;
        spiInputData.byUCWarnings = 0;
        spiInputData.byDigitalIn = 0;
        spiInputData.wAnalogIn0 = 0;
        spiInputData.wAnalogIn1 = 0;
        spiInputData.byGPIOIn = 0;
        spiInputData.wTemp0 = 0;
        spiInputData.byTemp0Error = 0;
        spiInputData.wTemp1 = 0;
        spiInputData.byTemp1Error = 0;
        spiInputData.wTemp2 = 0;
        spiInputData.byTemp2Error = 0;
        spiInputData.wTemp3 = 0;
        spiInputData.byTemp3Error = 0;
        spiInputData.wHumid0 = 0;
        spiInputData.wHumid1 = 0;
        spiInputData.wHumid2 = 0;
        spiInputData.wHumid3 = 0;
        spiInputData.rAnalogIn0 = 0.0;
        spiInputData.rAnalogIn1 = 0.0;
        spiInputData.rTemp0 = 0.0;
        spiInputData.rTemp1 = 0.0;
        spiInputData.rTemp2 = 0.0;
        spiInputData.rTemp3 = 0.0;
        spiInputData.rHumid0 = 0.0;
        spiInputData.rHumid1 = 0.0;
        spiInputData.rHumid2 = 0.0;
        spiInputData.rHumid3 = 0.0;
        memset(spiInputData.abyRetainDataIn, 0, sizeof(spiInputData.abyRetainDataIn));
    }

    void
    PiXtendV2SInst::resetSpiOutputData(PiXtendSpiOutputData& spiOutputData)
    {
        spiOutputData.byModelOut = model_;
        spiOutputData.byUCMode = 0;
        spiOutputData.byUCCtrl0 = 0;
        spiOutputData.byUCCtrl1 = 0;
        spiOutputData.byDigitalInDebounce01 = 0;
        spiOutputData.byDigitalInDebounce23 = 0;
        spiOutputData.byDigitalInDebounce45 = 0;
        spiOutputData.byDigitalInDebounce67 = 0;
        spiOutputData.byDigitalOut = 0;
        spiOutputData.byRelayOut = 0;
        spiOutputData.byGPIOCtrl = 0;
        spiOutputData.byGPIOOut = 0;
        spiOutputData.byGPIODebounce01 = 0;
        spiOutputData.byGPIODebounce23 = 0;
        spiOutputData.byPWM0Ctrl0 = 0;
        spiOutputData.wPWM0Ctrl1 = 0;
        spiOutputData.wPWM0A = 0;
        spiOutputData.wPWM0B = 0;
        spiOutputData.byPWM1Ctrl0 = 0;
        spiOutputData.byPWM1Ctrl1 = 0;
        spiOutputData.byPWM1A = 0;
        spiOutputData.byPWM1B = 0;
        spiOutputData.byJumper10V = 0;
        spiOutputData.byGPIO0Dht11 = 0;
        spiOutputData.byGPIO1Dht11 = 0;
        spiOutputData.byGPIO2Dht11 = 0;
        spiOutputData.byGPIO3Dht11 = 0;
        memset(spiOutputData.abyRetainDataOut, 0, sizeof(spiOutputData.abyRetainDataOut));
    }

    void
    PiXtendV2SInst::resetSpiOutputDataDac(PiXtendSpiOutputDataDac& spiOutputDataDac)
    {
        spiOutputDataDac.wAOut0 = 0;
        spiOutputDataDac.wAOut1 = 0;
    }

}
