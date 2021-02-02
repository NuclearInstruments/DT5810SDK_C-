// TestApp.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include "..\NIErrorCode.h"
#include "DDE3_API_C.h"
#include <math.h>

int TestDT5810DRC()
{
	int dHandle;
	int cChannel;
	INT32 dummy[100000];

	UINT32 SpectrumTest[16384];

	INT32 testSHAPE[4096];
	UINT32 testSHAPElen;
	UINT32 temp;
	NI_Startup();
	tUSBDevice USBdevices[10];
	UINT nDevs;



	NI_USBEnumerate(USBdevices, &nDevs);


	NI_AttachNewDevice(NI_CONNECT_USB, USBdevices[0].SN, 0, 0, &dHandle);

	double offset, gain, chtov;
	offset = 1;
	gain = 2;
	chtov = 3;


	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 0);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 1);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 2);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 3);


	UpdateDisplayStatus(1, 1000, 3100, "time", 0, 4587, "energy", "exp", 100, 0, 0, 0, "", dHandle, 0);
	UpdateDisplayStatus(1, 2000, 2000, "time2", 1, 54321, "energy2", "exp2", 50, 0, 0, 0, "", dHandle, 1);



	//TEST WAVEFORM GENERATOR

	//EmulatorAWGModeControl(1, dHandle, 0);
	//EmulatorAWGModeControl(1, dHandle, 1);
	//EmulatorAWGProgramScaler(0, dHandle, 0);
	//EmulatorAWGProgramScaler(0, dHandle, 1);

	//UINT32  * data = (UINT32 *)malloc(160000 * sizeof(UINT32));
	//UINT32 * data2 = (UINT32 *)malloc(160000 * sizeof(UINT32));
	//for (int i = 0; i < 160000; i++)
	//{
	//	data[i] = 16000.0 * sin(2 * 3.14*(float)i / 160000.0 * 2);
	//	data2[i] = 16000.0 * sin(2 * 3.14*(float)i / 160000.0 * 8);
	//}
	//ProgramDDR(data2, 160000, data, 160000, 0, 0, dHandle);

	//return 0;


	//Emulator Mode
	EmulatorAWGModeControl(0, dHandle, 0);
	EmulatorAWGModeControl(0, dHandle, 1);
	EmulatorAWGProgramScaler(0, dHandle, 0);
	EmulatorAWGProgramScaler(0, dHandle, 1);


	cChannel = 0;


	//START RANDOM NUMBER GENERATORS

	ConfigureLFSR(0, LFSR_ENERGY, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_TIMEBASE, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_GAUSS, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_RW, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_FLIKR, LFSR_REPROGRAM, dHandle, cChannel);

	//SET D-RC 1us risetime, 50 us tau
	ConfigureDRC(1, 50000, 1, dHandle, cChannel);

	//SET ENERGY
	ConfigureEnergy(0, 32000, dHandle, cChannel);

	//SET RATE TO 1KHz PERIODIC
	ConfigureTimebase(0, 1000, 0, false, dHandle, cChannel);

	//ENABLE CHANNEL
	EnableChanel(1, dHandle, cChannel);

	//CONFIGURE MCA
	SetInputAnalogDatapath(1,
		0,
		0,
		0,
		0,
		19700,
		dHandle,
		0);

	//SET MCA I/O
	SetDIO(TRIGGER_IN_0, 0, 0, 0, 0, 0, 0, dHandle);

	//SET MCA MIX TO OFF
	SetInputAnalogMix(0, -10000, 0, 0, 0, 0, 0, 0, dHandle,
		0);

	//ENABLE CORRELATION (2=1)
	DelayAndCorrelationControl(1, 1, 0.1, dHandle);


	TimebaseMux(0, dHandle, 0);
	TimebaseMux(0, dHandle, 1);

	EnergyMux(0, dHandle, 0);
	EnergyMux(0, dHandle, 1);

	ConfigureGeneral(
		1,
		0,
		0,
		0,
		0,
		dHandle,
		cChannel);

	//PROGRAM SHAPER GENERATOR (NOT NEEDED WITH D-RC)
	for (int i = 0; i<17; i++)
		ConfigureShapeGenerator(NULL, i, 0, 0, 0, 0, 0, false, false, dHandle, 0);

	//RESET LOOPBACK SPECTRUM
	ResetSpectrum(dHandle, 0);

	//READ LOOPBACK SPECTRUM
	GetSpectrumLoopback((unsigned int*)dummy, &temp, dHandle, 0);

	//use a breakpoint to view data

	//READ LOOPBACK SIGNAL

	GetSignalLoopback(dummy, &temp, dHandle, cChannel);
	for (int i = 0; i<4092; i++)
		testSHAPE[i + 4] = 0;//i*8;

	testSHAPE[0] = 0;
	testSHAPE[1] = 0;
	testSHAPE[2] = 0;
	testSHAPE[3] = 0;

	testSHAPElen = 4000;
	//ConfigureShapeGenerator(testSHAPE,0,testSHAPElen,0,0,0,0,true,false,dHandle,0);


	//USAGE OF CUSTOM SHAPE

	//for (int i=0;i<4092;i++)
	//testSHAPE[i+4] = 32000 * exp(-(double)i/1000) * (1- exp(-(double)i/3));

	//
	//	for (int i=0;i<3000;i++)
	//		testSHAPE[i+4] = 32000;
	//
	//	for (int i=0;i<15;i++)
	//		ConfigureShapeGenerator(testSHAPE,i,testSHAPElen,0,0,0,0,true,false,dHandle,0);
	//
	//	ConfigureMultishape(100,0,0,0,dHandle,0);
	//
	//
	//NOISE EXAMLE

	//	ConfigureNOISE(000,0000,0000,00,8,dHandle,0);

	//BASELINE DRIFT EXAMPLE

	//for (int i=0;i<4096;i++)
	//	testSHAPE[i] =0;//i*8 + (1<<16);

	//ConfigureBaselineDrift(testSHAPE,4095,30,100,false,false,false,dHandle,0);


	//SPECTRUM EXAMPLE

	//GENERATE A CUSTOM SPECTRUM

	for (int i = 0; i<16384; i++)
		SpectrumTest[i] = 000;

	SpectrumTest[4000] = 10000;
	SpectrumTest[15000] = 10000;
	SpectrumTest[8000] = 20000;

	//
	//	WRITE SPECTRUM
	ProgramSpectrum(SpectrumTest, 16384, dHandle, 0);
	ConfigureEnergy(1, 0, dHandle, cChannel);
	//









	cChannel = 1;

	//NI_DHA_WriteReg(0,0,dHandle,0);
	//NI_DHA_ReadArray(dummy, 0, 100000, dHandle,0);

	ConfigureLFSR(0, LFSR_ENERGY, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_TIMEBASE, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_GAUSS, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_RW, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_FLIKR, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureDRC(0, 50000, 1, dHandle, cChannel);
	ConfigureEnergy(0, 32000, dHandle, cChannel);
	ConfigureTimebase(0, 1000, 0, false, dHandle, cChannel);
	EnableChanel(1, dHandle, cChannel);

	ConfigureGeneral(
		1,
		0,
		0,
		0,
		0,
		dHandle,
		cChannel);
	for (int i = 0; i<17; i++)
		ConfigureShapeGenerator(testSHAPE, i, testSHAPElen, 0, 0, 0, 0, true, false, dHandle, 1);
	SetAnalogDatapath(0, 0, dHandle, cChannel);
	//ConfigureShapeGenerator(NULL,i,0,0,0,1,1,false,false,dHandle,cChannel);

	//	ResetSpectrum(dHandle,0);
	//	ConfigureEnergy(0,30000,dHandle, 0);*/
	//ConfigureShapeGenerator(testSHAPE,0,testSHAPElen,NULL,0,true,true,false,0,dHandle,0);


	//	 ConfigureNOISE(0,0,0,5000,0,dHandle,0);


	//cChannel = 2;

	//
	//ConfigureLFSR(0, LFSR_ENERGY, LFSR_REPROGRAM,dHandle,cChannel);
	//ConfigureLFSR(0, LFSR_TIMEBASE, LFSR_REPROGRAM,dHandle,cChannel);
	//ConfigureLFSR(0, LFSR_NOISE_GAUSS, LFSR_REPROGRAM,dHandle,cChannel);
	//ConfigureLFSR(0, LFSR_NOISE_RW, LFSR_REPROGRAM,dHandle,cChannel);
	//ConfigureLFSR(0, LFSR_NOISE_FLIKR, LFSR_REPROGRAM,dHandle,cChannel);
	//ConfigureEnergy(0,32000,dHandle, cChannel);
	//ConfigureTimebase(0 ,5000,0, false, dHandle, cChannel);
	//EnableChanel(1, dHandle, cChannel);

	//RunControlResetStat(dHandle, 0);

	SetRunControlMode(0, 10, dHandle, 1);
	//RunControlResetStart(dHandle, 1);
	RunControlResetStat(dHandle, 1);
	//SetRunControlMode(0,10, dHandle, 0);

	double run_time;
	double sat_time;
	double busy_time;
	double real_time;
	UINT32 cnt_event;
	UINT32 sat_event;
	UINT32 lost_event;
	double measured_rate;
	UINT32 real_event;
	UINT32 busyflag;
	UINT32 satflag;
	UINT32 eflag;

	Sleep(2000);


	//ControlLFSR( 0, 3, 0, 0, 0, dHandle);

	return 0;


	while (1)
	{
		GetLiveData(&run_time,
			&sat_time,
			&busy_time,
			&real_time,
			&cnt_event,
			&sat_event,
			&lost_event,
			&measured_rate,
			&real_event,
			&busyflag,
			&satflag,
			&eflag,
			dHandle,
			1
		);

		printf("DATA: %f %f %f %d\n", run_time, real_time, measured_rate, cnt_event);
		Sleep(300);
	}
	return 0;
}


int TestDT5810MemoryBasedPulseGeneratorWithVeryLongTail()
{
	int dHandle;
	int cChannel;
	INT32 dummy[100000];

	UINT32 SpectrumTest[16384];

	INT32 testSHAPE[4096];
	UINT32 testSHAPElen;
	UINT32 temp;
	NI_Startup();
	tUSBDevice USBdevices[10];
	UINT nDevs;



	NI_USBEnumerate(USBdevices, &nDevs);


	NI_AttachNewDevice(NI_CONNECT_USB, USBdevices[0].SN, 0, 0, &dHandle);

	double offset, gain, chtov;
	offset = 1;
	gain = 2;
	chtov = 3;


	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 0);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 1);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 2);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 3);



	//Emulator Mode
	EmulatorAWGModeControl(0, dHandle, 0);
	EmulatorAWGModeControl(0, dHandle, 1);
	EmulatorAWGProgramScaler(0, dHandle, 0);
	EmulatorAWGProgramScaler(0, dHandle, 1);

	cChannel = 0;


	//START RANDOM NUMBER GENERATORS

	ConfigureLFSR(0, LFSR_ENERGY, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_TIMEBASE, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_GAUSS, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_RW, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_FLIKR, LFSR_REPROGRAM, dHandle, cChannel);

	//Disable DRC
	ConfigureDRC(1, 50000, 0, dHandle, cChannel);


	//SET ENERGY
	ConfigureEnergy(0, 32000, dHandle, cChannel);

	//SET RATE TO 1KHz PERIODIC
	ConfigureTimebase(0, 100, 0, false, dHandle, cChannel);

	//ENABLE CHANNEL
	EnableChanel(1, dHandle, cChannel);


	//Disable list mode Timebase and Energy generator. Use statical generation
	TimebaseMux(0, dHandle, 0);
	TimebaseMux(0, dHandle, 1);
	EnergyMux(0, dHandle, 0);
	EnergyMux(0, dHandle, 1);

	//Configure general with default patameters
	ConfigureGeneral(
		1,
		0,
		0,
		0,
		0,
		dHandle,
		cChannel);


	for (int i = 0; i<4092; i++)
		testSHAPE[i + 4] = 0;//i*8;

	testSHAPE[0] = 0;
	testSHAPE[1] = 0;
	testSHAPE[2] = 0;
	testSHAPE[3] = 0;

	testSHAPElen = 4000;
	ConfigureShapeGenerator(testSHAPE,0,testSHAPElen,0,0,0,0,true,false,dHandle,0);


	//USAGE OF CUSTOM SHAPE

	for (int i = 0; i < 4092; i++)
		testSHAPE[i + 4] = 32000 * exp(-(double)i / 700);

	for (int i=0;i<15;i++)
		ConfigureShapeGenerator(testSHAPE,i,testSHAPElen,0,8,1,20000,true,true,dHandle,0);
	
		ConfigureMultishape(100,0,0,0,dHandle,0);
	
		return 0;
}



int TestDT5810MemoryBasedPulseGeneratorWithVeryLongTailAndSpectrum()
{
	int dHandle;
	int cChannel;
	INT32 dummy[100000];

	UINT32 SpectrumTest[16384];

	INT32 testSHAPE[4096];
	UINT32 testSHAPElen;
	UINT32 temp;
	NI_Startup();
	tUSBDevice USBdevices[10];
	UINT nDevs;



	NI_USBEnumerate(USBdevices, &nDevs);


	NI_AttachNewDevice(NI_CONNECT_USB, USBdevices[0].SN, 0, 0, &dHandle);

	double offset, gain, chtov;
	offset = 1;
	gain = 2;
	chtov = 3;


	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 0);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 1);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 2);
	ReadCalibrarionFlash(&offset, &gain, &chtov, dHandle, 3);



	//Emulator Mode
	EmulatorAWGModeControl(0, dHandle, 0);
	EmulatorAWGModeControl(0, dHandle, 1);
	EmulatorAWGProgramScaler(0, dHandle, 0);
	EmulatorAWGProgramScaler(0, dHandle, 1);

	cChannel = 0;


	//START RANDOM NUMBER GENERATORS

	ConfigureLFSR(0, LFSR_ENERGY, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_TIMEBASE, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_GAUSS, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_RW, LFSR_REPROGRAM, dHandle, cChannel);
	ConfigureLFSR(0, LFSR_NOISE_FLIKR, LFSR_REPROGRAM, dHandle, cChannel);

	//Disable DRC
	ConfigureDRC(1, 50000, 0, dHandle, cChannel);


	//SET ENERGY
	ConfigureEnergy(0, 32000, dHandle, cChannel);

	//SET RATE TO 1KHz PERIODIC
	ConfigureTimebase(0, 100, 0, false, dHandle, cChannel);

	//ENABLE CHANNEL
	EnableChanel(1, dHandle, cChannel);


	//Disable list mode Timebase and Energy generator. Use statical generation
	TimebaseMux(0, dHandle, 0);
	TimebaseMux(0, dHandle, 1);
	EnergyMux(0, dHandle, 0);
	EnergyMux(0, dHandle, 1);

	//Configure general with default patameters
	ConfigureGeneral(
		1,
		0,
		0,
		0,
		0,
		dHandle,
		cChannel);


	for (int i = 0; i<4092; i++)
		testSHAPE[i + 4] = 0;//i*8;

	testSHAPE[0] = 0;
	testSHAPE[1] = 0;
	testSHAPE[2] = 0;
	testSHAPE[3] = 0;

	testSHAPElen = 4000;
	ConfigureShapeGenerator(testSHAPE, 0, testSHAPElen, 0, 0, 0, 0, true, false, dHandle, 0);


	//USAGE OF CUSTOM SHAPE

	for (int i = 0; i < 4092; i++)
		testSHAPE[i + 4] = 32000 * exp(-(double)i / 700);

	for (int i = 0; i<15; i++)
		ConfigureShapeGenerator(testSHAPE, i, testSHAPElen, 0, 8, 1, 20000, true, true, dHandle, 0);

	ConfigureMultishape(100, 0, 0, 0, dHandle, 0);


	for (int i = 0; i<16384; i++)
		SpectrumTest[i] = 000;

	SpectrumTest[4000] = 10000;
	SpectrumTest[8000] = 20000;
	SpectrumTest[15000] = 10000;

	//
	//	WRITE SPECTRUM
	ProgramSpectrum(SpectrumTest, 16384, dHandle, 0);
	ConfigureEnergy(1, 0, dHandle, cChannel);
	


	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestDT5810MemoryBasedPulseGeneratorWithVeryLongTailAndSpectrum();

	return 0;
}

