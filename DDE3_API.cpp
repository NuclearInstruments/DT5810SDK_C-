#include "stdafx.h"
#include "DDE3_API.h"
#include <math.h>
#include <time.h>
#include <stdlib.h> 
#include <signal.h>
#include <fstream>
#include "mkfilter.h"
#include <math.h>

#undef	PI
#define PI	    3.14159265358979323846  /* Microsoft C++ does not define M_PI ! */

#define __CHOFFSET				0x10000000

//DAC

#define RFA_DACCNRL_RESET					0xF0000015
#define RFA_DACCNRL_DACSEL					0xF0000016
#define RFA_DACCNRL_SYNC					0xF0000014
#define RFA_DACCNRL_VAL						0xF0000011
#define RFA_DACCNRL_STROBE					0xF0000013


//CLOCK GENERATOR
#define RFA_CLKGEN_POWER					0xF0000030
#define RFA_CLKGEN_CFG0						0xF0000001
#define RFA_CLKGEN_CFG1						0xF0000002
#define RFA_CLKGEN_STROBE					0xF0000003
#define RFA_CLKGEN_CFG2      				0xF0000004
#define RFA_CLKGEN_CFG3      				0xF0000005
#define RFA_CLKGEN_CFG4      				0xF0000006
#define RFA_CLKGEN_CFG5      				0xF0000007
#define RFA_CLKGEN_CFG6      				0xF0000008
#define RFA_CLKGEN_CFG7      				0xF0000009
#define RFA_CLKGEN_CFG8      				0xF000000A

//CLOCK GENERATOR DT5810B 
#define RFA_DT5810B_CLKGEN_STROBE					0xF0000001
#define RFA_DT5810B_CLKGEN_CFG0						0xF0000002
#define RFA_DT5810B_CLKGEN_CFG1						0xF0000003
#define RFA_DT5810B_CLKGEN_CFG2      				0xF0000004
#define RFA_DT5810B_CLKGEN_CFG3      				0xF0000005
#define RFA_DT5810B_CLKGEN_CFG4      				0xF0000006
#define RFA_DT5810B_CLKGEN_CFG5      				0xF0000007
#define RFA_DT5810B_CLKGEN_CFG6      				0xF0000008
#define RFA_DT5810B_CLKGEN_CFG7      				0xF0000009
#define RFA_DT5810B_CLKGEN_CFG8      				0xF000000A
#define RFA_DT5810B_CLKGEN_CFG9      				0xF000000B
#define RFA_DT5810B_CLKGEN_CFGA      				0xF000000C
#define RFA_DT5810B_CLKGEN_CFGB      				0xF000000D


#define RFA_DT5810B_CLKGEN_INSEL      				0xF0000037


//ChannelGlobalControl
#define RFA_CHANNEL_ENABLE					0x0F000002
#define RFA_TIMEMUX							0x0F000005
#define RFA_ENERGYMUX						0x0F000006

#define RFA_HWINVERTA						0xF0000043
#define RFA_HWINVERTB						0xF0000044


//TIMEBASE
#define RFA_TIMEBASE_SEED1					0x00100000
#define RFA_TIMEBASE_SEED2					0x00100001
#define RFA_TIMEBASE_SEED3					0x00100002
#define RFA_TIMEBASE_SEED4					0x00100003
#define RFA_TIMEBASE_SEEDPROG				0x00100004
#define RFA_TIMEBASE_POISSONP				0x00100006
#define RFA_TIMEBASE_PARFLAG				0x00100007
#define RFA_TIMEBASE_DEADTIME				0x00100008
#define RFA_TIMEBASE_RATE					0x00100009
#define RFA_TIMEBASE_STATSOURCEMUX			0x0010000A
#define RFA_TIMEBASE_TRIGGEROUTLEN			0x0010000B
#define RFA_TIMEBASE_TRIGGEROUTEN			0x0010000C

//DIGITAL RC

#define RFA_DRC_PALFA1						0x00300000
#define RFA_DRC_PALFA1B						0x00300001
#define RFA_DRC_PALFA2						0x00300002
#define RFA_DRC_PALFA3						0x00300003
#define RFA_DRC_PALFA4						0x00300004
#define RFA_DRC_LALFA1						0x00300005
#define RFA_DRC_LALFA1B						0x00300006
#define RFA_DRC_LALFA2						0x00300007
#define RFA_DRC_LALFA3						0x00300008
#define RFA_DRC_LALFA4						0x00300009
#define RFA_DRC_RESET						0x0030000A
#define RFA_DRC_ENABLE						0x0030000B
#define RFA_DRC_OS							0x0030000C
#define RFA_DRC_BYPASSLOWPASS				0x0030000D

#define RFA_DRC_MODE						0x00300010
#define RFA_DRC_LIMITLOW					0x00300011
#define RFA_DRC_LIMITHIGH					0x00300012
#define RFA_DRC_SCALE						0x00300013

#define RFA_DRC_DECIMATOR					0x00300014

//SHAPES
#define RFA_SHAPE_OFFSET					0x00100000
#define RFA_SHAPE_BA						0x00500000
#define RFA_SHAPE_ENABLE					0x0050F000
#define RFA_SHAPE_MULTISHAPEID				0x0050F001
#define RFA_SHAPE_PROG						0x0050F002
#define RFA_SHAPE_LEN						0x0050F003
#define RFA_SHAPE_INTERP_ENABLE				0x0050F004
#define RFA_SHAPE_INTERP_CROSS				0x0050F005
#define RFA_SHAPE_INTERP_DOWN				0x0050F006
#define RFA_SHAPE_INTERP_UP					0x0050F007


//MULTISHAPE
#define RFA_MSHAPE_SEED1					0x00400000
#define RFA_MSHAPE_SEED2					0x00400001
#define RFA_MSHAPE_SEMEPROG					0x00400002
#define RFA_MSHAPE_PROB1					0x00400003
#define RFA_MSHAPE_PROB2					0x00400004
#define RFA_MSHAPE_PROB3					0x00400005
#define RFA_MSHAPE_BYPASS					0x00400006


//GAUSS
#define RFA_NOISE_GAUSSM					0x01400000
#define RFA_NOISE_SAMPL						0x01400003
#define RFA_NOISE_TEMPSEED1					0x01400004
#define RFA_NOISE_TEMPSEED2					0x01400005
#define RFA_NOISE_STARTSEEDGEN				0x01400009
#define RFA_NOISE_ENABLELFSRUSB				0x01400007
#define RFA_NOISE_RESETLFSR					0x01400006
#define RFA_NOISE_CTRLLFSR					0x01400008


//1/F NOISE
#define RFA_NF_MAGNITUDE					0x01500000
#define RFA_NF_SEME1						0x01500001
#define RFA_NF_SEME2						0x01500002
#define RFA_NF_RESETLFSRUSB					0x01500003
#define RFA_NF_ENABLELFSRUSB				0x01500004
#define RFA_NF_COEFFLOAD					0x01500006
#define RFA_NF_COEFF						0x01600000

//RANDOM WALK
#define RFA_RW_MAG							0x01900000
#define RFA_RW_SEED1						0x01900001
#define RFA_RW_SEED2						0x01900002
#define RFA_RW_STARTSEEDGEN					0x01900003

//RANDOM NOISE
#define RFA_RN_MAG							0x01700002
#define RFA_RN_SEED1						0x01700000
#define RFA_RN_SEED2						0x01700001
#define RFA_RN_RESETLFSR					0x01700003
#define RFA_RN_PCENABLELFSR					0x01700004
#define RFA_RN_CTRLLFSR						0x01700005


//RANDOM NOISE
#define RFA_BLD_BA							0x01800000
#define RFA_BLD_PROG						0x0180F000
#define RFA_BLD_MEMLEN						0x0180F001
#define RFA_BLD_RSLOW						0x0180F002
#define RFA_BLD_ASLOW						0x0180F003
#define RFA_BLD_RFAST						0x0180F004
#define RFA_BLD_AFAST						0x0180F005
#define RFA_BLD_ENABLE						0x0180F006
#define RFA_BLD_RESET						0x0180F007



//OFFSET GAIN
#define RFA_OFFGAIN_GAIN					0x0F000001
#define RFA_OFFGAIN_OFFSET					0x0F000000
#define RFA_OFFGAIN_INVERT					0x0F000004
//#define RFA_OFFGAIN_ENABLE					0x0F000002


//OFFSET ICAPDNA
#define RFA_ICAPDNA_REBOOTADDR			0x00000700
#define RFA_ICAPDNA_UNLOCK				0x00000701
#define RFA_ICAPDNA_nREBOOT				0x00000702
#define RFA_ICAPDNA_ENABLE				0x00000703
#define RFA_ICAPDNA_UID1				0x00000704
#define RFA_ICAPDNA_UID2				0x00000705
#define RFA_ICAPDNA_CHECK1				0x00000706
#define RFA_ICAPDNA_CHECK2				0x00000707
#define RFA_ICAPDNA_CHECK3				0x00000708
#define RFA_ICAPDNA_CHECK4				0x00000709
#define RFA_ICAPDNA_CHECK5				0x0000070A
#define RFA_ICAPDNA_CHECK6				0x0000070B
#define RFA_ICAPDNA_CHECK7				0x0000070C
#define RFA_ICAPDNA_CHECK8				0x0000070D
#define RFA_ICAPDNA_WF					0xFEFFFF00
#define RFA_ICAPDNA_EEPROM				0xFEFFFF01
#define RFA_ICAPDNA_READEEPROM			0xFEFFFF07


//ENERGY
#define RFA_ENERGY_MEMBA					0x00200000
#define RFA_ENERGY_SEED1					0x0020F000
#define RFA_ENERGY_SEED2					0x0020F001
#define RFA_ENERGY_STARTSEEDGEN				0x0020F002
#define RFA_ENERGY_PROGMODE					0x0020F003
#define RFA_ENERGY_FIXSPE					0x0020F004
#define RFA_ENERGY_FIXENERGY				0x0020F005
#define RFA_ENERGY_FLUSH					0x0020F006


//SPECTRUM LOOPBACK
#define RFA_SPELOOP_MEMBA				0x10000000
#define RFA_SPELOOP_RESETSM				0x01A00000
#define RFA_SPELOOP_MODE				0x01A00001
#define RFA_SPELOOP_SCALE				0x01A00002

//MONITOR
#define RFA_MON_MEMBA					0x20000000
#define RFA_MON_RESTOREACQ				0x01B00000
#define RFA_MON_CPSR_DATAREADY			0x0003F001
#define RFA_MON_CPSR					0x0003F002
#define RFA_MON_CPSLIVER				0x0003F003
#define RFA_MON_CPSR_CCOUNTER1			0x0003F004
#define RFA_MON_CPSR_CCOUNTER2			0x0003F005
#define RFA_MON_CPSR_OVERLOAD_OVERFLOW	0x0003F006
#define RFA_MON_CPSR_RESET				0x0003F007


//MCA
//ADC
#define MCA_BASE						0x30000000
#define ADC_BASE						0x0E000000
#define ADC_ENABLE_A					0x00000000
#define ADC_ENABLE_B					0x00000001
#define ADC_GAIN_A						0x00000002
#define ADC_GAIN_B						0x00000003
#define ADC_OFFSET_A					0x00000004
#define ADC_OFFSET_B					0x00000005
#define ADC_INV_A						0x00000006
#define ADC_INV_B						0x00000007



//CORRELATION AND DELAY
#define CORRELATION_BASE				0x20000000
#define CORRELATION_DELAY_OFFSET		0x0E000000
#define CORRELATION_DELAY_MODE			0x00000000
#define CORRELATION_DELAY_VAL			0x00000001
#define CORRELATION_DELAY_DIGA			0x00000002
#define CORRELATION_DELAY_DIGB			0x00000003


//DIGITALIO
#define RFA_DIO_FUNCINA					0xA0000000
#define RFA_DIO_FUNCINB					0xA0000001
#define RFA_DIO_PULSELEN				0xA0000002
#define RFA_DIO_FUNCOUTA				0xA0000003
#define RFA_DIO_FUNCOUTB				0xA0000004


#define RUNCONTROL_MODE					0x01C00005
#define RUNCONTROL_TIMETARGET1			0x01C00000
#define RUNCONTROL_TIMETARGET2			0x01C00001
#define RUNCONTROL_CONTARGET			0x01C00002
#define RUNCONTROL_RESETSTART			0x01C00003
#define RUNCONTROL_RESETSTAT			0x01C00004
#define RUNCONTROL_ENABLE				0x01C00006


#define LFSR_ALLCHANNEL					0xFFFF0100
#define LFSR_SINGLECHANNEL				0x0FFF0100



//INFOS
#define RFA_INFO_CLKFREQ				0x00000001
#define RFA_INFO_CHANNELCOUNT			0x00000002
#define RFA_INFO_CHANNELOFFSET			0x00000003

#define RFA_INFO_ISTRVER				0xFFF00000
#define RFA_INFO_FIRMWARE_RELEASE		0xFFF00001
#define RFA_INFO_SERIAL_NUMBER			0xFFF00002
#define RFA_INFO_UID0					0xFFF00002
#define RFA_INFO_UID1					0xFFF00003
#define RFA_INFO_OPTIONS				0xFFF00004
#define RFA_INFO_BOARDREV				0xFFF00005
#define RFA_INFO_ANALOGINFO				0xFFF00006
#define RFA_STATUS_CONNECTION_MODE		0xFFF00007
#define RFA_INFO_UCRELEASE				0xFFF00009

//Display
#define RFA_UC_BA					    0xF00000A0
#define RFA_UC_CHOFF				    0x00000040
#define RFA_DISPLAY_TIMEMODE			0x00000000
#define RFA_DISPLAY_RATE				0x00000001
#define RFA_DISPLAY_RATEP				0x00000003
#define RFA_DISPLAY_TIMESTR				0x00000005
#define RFA_DISPLAY_ENERGYMODE			0x0000000C
#define RFA_DISPLAY_ENERGY				0x0000000D
#define RFA_DISPLAY_ENERGYSTR			0x0000000E
#define RFA_DISPLAY_SHAPE				0x00000015
#define RFA_DISPLAY_LIVE				0x0000001C
#define RFA_DISPLAY_FUNCGEN				0x00000030
#define RFA_DISPLAY_FUNCGEN_VOLT		0x00000031
#define RFA_DISPLAY_FUNCGEN_FREQ		0x00000032
#define RFA_DISPLAY_FUNCGEN_STR			0x00000033

#define RFA_PORTEXP1					0x00000020
#define RFA_PORTEXP2					0x00000021
#define RFA_PORTEXP3					0x00000022

#define RFA_CONNECTED					0x00000023
#define RFA_IN_OFFSET					0x00000024



#define RFA_INFO_WRITETOFLASH			0x00000009
#define RFA_INFO_gOFFSET				0x0000000A
#define RFA_INFO_gGAIN					0x0000000B
#define RFA_INFO_gCTV					0x0000000C


#define LIVE_BASEADDRESS_A	 0x110000A0
#define LIVE_BASEADDRESS_B	 0x310000A0





#define REG_COUNT				26					
#define REG_ADDRESS				0

//top_module
#define RFA_GLOBAL_LFSR_SOURCE_CONTROL	0
#define RFA_GLOBAL_LFSR_RESET_ETH	1
#define RFA_GLOBAL_LFSR_ENABLE_ETH	2
#define RFA_GLOBAL_LFSR_ONESHOT	3
#define RFA_GLOBAL_PLAYPAUSE_ENABLE	4
#define RFA_GLOBAL_PLAYPAUSA_MORESHOT_COUNTER	5
#define RFA_GLOBAL_PLAYPAUSA_MORESHOT_ENABLE	6
//fire_pulse_source	
#define RFA_TIMEBASE_SOURCE_REG	7
//timebase_by_rate	
#define RFA_CONSTANTRATE_REG	8
//timebase_statistic	
#define RFA_STAT_POISSON_P	9
#define RFA_LFSR_SOURCE_CONTROL_TBS	10
#define RFA_LFSR_PC_ENABLE_ETH_TBS	11
#define RFA_RESET_LFSR_ETH_TBS	12
#define RFA_PAR_FLAG_REG	13
#define RFA_DEADTIME_REG	14
#define RFA_SEME1_TBS	15
#define RFA_SEME2_TBS	16
//spectrum_modulator	
#define RFA_ENERGY_SOURCE_REG	17
#define RFA_CONSTANT_ENERGY_REG	18
#define RFA_PROG_MODE	19
#define RFA_SCALE_FACTOR	20
#define RFA_RESET_LFSR_ETH_SP_MOD	21
#define RFA_LFSR_PC_ENABLE_ETH_SP_MOD	22
#define RFA_LFSR_SOURCE_CONTROL_SP_MOD	23
#define RFA_SEME1_SP_MOD	24
#define RFA_SEME2_SP_MOD	25

// Default values
//top
#define	RFD_TIMEBASE_SOURCE_REG					00
#define	RFD_CONSTANTRATE_REG					0x00F00000
#define	RFD_GLOBAL_LFSR_RESET_ETH				0
#define	RFD_GLOBAL_LFSR_ENABLE_ETH				1
#define	RFD_GLOBAL_LFSR_SOURCE_CONTROL			00
#define	RFD_GLOBAL_LFSR_ONESHOT					0
#define	RFD_GLOBAL_PLAYPAUSE_ENABLE				1
#define	RFD_GLOBAL_PLAYPAUSA_MORESHOT_COUNTER	0x00000000
#define	RFD_GLOBAL_PLAYPAUSA_MORESHOT_ENABLE	0
#define	RFD_ENERGY_SOURCE_REG					01
#define	RFD_CONSTANT_ENERGY_REG					0x7FFF
//timebase_statistic
#define	RFD_STAT_POISSON_P						0x00400000
#define	RFD_LFSR_SOURCE_CONTROL_TBS				00
#define	RFD_LFSR_PC_ENABLE_ETH_TBS				1
#define	RFD_RESET_LFSR_ETH_TBS					0
#define	RFD_PAR_FLAG_REG						0
#define	RFD_DEADTIME_REG						0x0000
#define	RFD_SEME1_TBS							0x166C3967
#define	RFD_SEME2_TBS							0x1AF0F0E9
//spectrum modulator		
#define	RFD_PROG_MODE							0
#define	RFD_SCALE_FACTOR						00
#define	RFD_RESET_LFSR_ETH_SP_MOD				0
#define	RFD_LFSR_PC_ENABLE_ETH_SP_MOD			1
#define	RFD_LFSR_SOURCE_CONTROL_SP_MOD			00
#define	RFD_SEME1_SP_MOD						0x2CD872CE
#define	RFD_SEME2_SP_MOD						0x35E1E1D2


//FUNCTION GENERATOR
#define DDR_MEMCACHE_BA							0xFA000000
#define DDR_BA1									0xFA001000
#define DDR_BA2									0xFA001003
#define DDR_CTRL_REG							0xFA001001
#define DDR_MEMLEN1								0xFA001002
#define DDR_MEMLEN2								0xFA001004
#define DDR_SCALER1								0xFA001005
#define DDR_SCALER2								0xFA001006
#define DDR_WFENABLE							0xFA00100A
#define DDR_BAOFFSET							67108864/2

#define DDR_CTRL_MODE							0xFA00100C

unsigned int REGISTER_MEMMAP[REG_COUNT];
unsigned int _REGISTER_MEMMAP[REG_COUNT];

DDE3_API::DDE3_API()
{
	MCAspectrum = new unsigned int[25000];
	for (int i = 0; i < 16384; i++)
		MCAspectrum[i] = 0;
	peout1=0;
	return;
}

DDE3_API::~DDE3_API()
{
		return;
}


	void DDE3_API::Allocate()
	{
		niHAL = new NIHAL::NI_HAL();
		return;
	}

	void DDE3_API::Destroy()
	{
		niHAL->CloseConnection();
		delete niHAL;
	}


NI_RESULT DDE3_API::USBEnumerate(tUSBDevice *pvArg1, unsigned int *numDevs)
{
	return niHAL->USBEnumerate(pvArg1, numDevs);
}

NI_RESULT DDE3_API::ETHEnumerate(tETHDevices *pvArg1)
{
	return niHAL->ETHEnumerate(pvArg1);
}

NI_RESULT DDE3_API::ETHGetSerialNumber(short int port, char *IPAddress, char *SN)
{
	return niHAL->ETHGetSerialNumber(port,IPAddress,SN);
}

typedef struct {
	unsigned long INSTRUMENTVERSION;
	unsigned long FIRMWARERELEASE;
	unsigned long SERIALNUMBER;
	unsigned long UID0;
	unsigned long UID1;
	unsigned long OPTIONS;
	unsigned long HWBOARDREV;
	unsigned long OUTPUTCHANNELS;
	unsigned long INPUTCHANNELS;
}t_HWINFO;

t_HWINFO HWINFO;
NI_RESULT DDE3_API::ReadHardwareProperties()
{
	unsigned int TTT[32];
	unsigned int TEMP;
	float t;
	unsigned int OFS;
	int i;

	UINT64 UID;
	unsigned int SN;

	niHAL->WriteReg(0xFF, 0xFFFF0000);

	if (niHAL->ReadArray(TTT, 0xFFFF0000, 3) != NI_OK)
		return NI_ERROR;

	//_clockFREQUENCY = 250000000;//875000000/4 * 1.052;//250000000/1000*875;

	if (niHAL->ReadArray(TTT, 0xFFF00000,9) != NI_OK)
		return NI_ERROR;

	i = 0;

	SECReadUIDSN(&UID, &SN);

	HWINFO.INSTRUMENTVERSION = TTT[i++] & 0xFFFFFFF;
	HWINFO.FIRMWARERELEASE = TTT[i++];
	HWINFO.SERIALNUMBER = SN;//TTT[i++];
	i++;
	HWINFO.UID0 = (UID&0xFFFFFFFF); //TTT[i++];
	i++;
	HWINFO.UID1 = ( (UID>>32) &0xFFFFFFFF); //TTT[i++];
	i++;
	HWINFO.OPTIONS = TTT[i++];
	HWINFO.HWBOARDREV = TTT[i++];
	HWINFO.OUTPUTCHANNELS = TTT[i] & 0xF;
	HWINFO.INPUTCHANNELS = (TTT[i++] >> 28) & 0xF;

	_clockFREQUENCY = ((double) TTT[i++]) / 4.0;
	
	if (HWINFO.INSTRUMENTVERSION == 0x5810) 
		INSTRUMENTVERSION = DT5810;
	if (HWINFO.INSTRUMENTVERSION == 0x5810B)
		INSTRUMENTVERSION = DT5810B;
	

	if ((HWINFO.INSTRUMENTVERSION != 0x10005810) && (HWINFO.INSTRUMENTVERSION != 0x1005810B))
		return NI_INVALID_HARDWARE;
	

	return NI_OK;
}
NI_RESULT DDE3_API::EthernetConnect(short int port, char *IPAddress)
{
	NI_RESULT Status = niHAL->EthernetConnect(port, IPAddress);
	if (Status == NI_CONNECTED)
		{

			
			niHAL->WriteReg(1, 0xFFFFFFFE);
			niHAL->WriteReg(1, 0xFFFFFFFE);
			niHAL->WriteReg(1, 0xFFFFFFFE);

			ReadHardwareProperties();


			if (INSTRUMENTVERSION == DT5810B)
			{
				niHAL->WriteReg(0, 0xF0000035); //Attivazione alimentazioni uscita
				
				Sleep(100);
				ConfigureCLOCKGENDT5810B(_clockFREQUENCY*4/1000000);	
			}
			else
			{
				ConfigureCLOCKGEN();
			}

			if (INSTRUMENTVERSION == DT5810B)
			{
				niHAL->WriteReg(0, 0xF0000036);
				niHAL->WriteReg(1, 0xF0000036);
				niHAL->WriteReg(0, 0xF0000036);

				niHAL->WriteReg(0, 0xF0000034);
			}

			if (INSTRUMENTVERSION == DT5810)
				ConfigureDAC(0,1);

			Sleep(100);
				

			return NI_OK;
		}
		else
			return NI_ERROR;
}

NI_RESULT DDE3_API::USBConnect(char *SN)
{	int i;
	
	NI_RESULT Status = niHAL->USBConnect(SN);


	if (Status == NI_CONNECTED)
	{
		niHAL->WriteReg(0, 0xFFFFFFFE);
		niHAL->WriteReg(0, 0xFFFFFFFE);
		niHAL->WriteReg(0, 0xFFFFFFFE);
		

		ReadHardwareProperties	();

		if (INSTRUMENTVERSION == DT5810B)
		{
			niHAL->WriteReg(0, 0xF0000035); //Attivazione alimentazioni uscita
			Sleep(100);
			ConfigureCLOCKGENDT5810B(_clockFREQUENCY*4/1000000);	
		}
		else
		{
			ConfigureCLOCKGEN();
		}

		if (INSTRUMENTVERSION == DT5810B)
		{
			niHAL->WriteReg(0, 0xF0000036);
			niHAL->WriteReg(1, 0xF0000036);
			niHAL->WriteReg(0, 0xF0000036);

			niHAL->WriteReg(0, 0xF0000034);
		}

		if (INSTRUMENTVERSION == DT5810)
			ConfigureDAC(0,1);

		Sleep(1000);


		UpdateDisplayStatus(0,0,0,0,"",0,0,"","",100,0,0,0,"");
		UpdateDisplayStatus(1,0,0,0,"",0,0,"","",100, 0, 0, 0, "");
		UnlockDisplays();
		return NI_OK;
	}
		else
			return NI_ERROR;
}

NI_RESULT DDE3_API::CloseConnection()
{
	return niHAL->CloseConnection();
}

NI_RESULT DDE3_API::ConnectionStatus()
{
	//return niHAL->ConnectionStatus();
	unsigned int TTT[32];
	int i=0;
	if (niHAL->ReadArray(TTT, 0xFFF00000,9) != NI_OK)
		return 0;
	i=0;
	HWINFO.INSTRUMENTVERSION = TTT[i++];
	if ((HWINFO.INSTRUMENTVERSION == 0x10005850) || (HWINFO.INSTRUMENTVERSION == 0x10005810) || (HWINFO.INSTRUMENTVERSION == 0x1005810B))
		return 1;
	return 0;
}

NI_RESULT DDE3_API::DHA_WriteReg(UINT32 value, UINT32 address)
{
	NI_STATUS Status;
		Status = niHAL->WriteReg(value,address);
	return Status;
}

NI_RESULT DDE3_API::DHA_ReadReg(UINT32 *value, UINT32 address)
{
	NI_STATUS Status;
	Status = niHAL->ReadReg(value,address);
	return Status;
}

NI_RESULT DDE3_API::DHA_WriteArray(UINT32 *value, UINT32 address, UINT32 length)
{
	NI_STATUS Status;
	Status = niHAL->WriteArray(value,address,length);
	return Status;

}

NI_RESULT DDE3_API::DHA_ReadArray(UINT32 *value, UINT32 address, UINT32 length)
{
	NI_STATUS Status;
	Status = niHAL->ReadArray(value,address,length);
	return Status;

}


NI_RESULT DDE3_API::SetuDDERegister(UINT32 value, UINT32 address)
{
	_REGISTER_MEMMAP[address] = value;
	return NI_OK;
}

NI_RESULT DDE3_API::GetuDDERegister(UINT32 *value, UINT32 address)
{
	*value=_REGISTER_MEMMAP[address];
	return NI_OK;
}





/*
	CONFIGURE CHANNEL
	enable			0					disabled
					1					enabled
*/


void DDE3_API::programReg(unsigned int channel, unsigned int addr, unsigned int val)
{



}


uint32_t reversebits(uint32_t val)
{
	uint32_t result=0;
	int i;
	for (i=0;i<16;i++)
	{
		result += ((val >> i) & 0x01) << (15-i);
	}
	return result;
}



NI_RESULT DDE3_API::ConfigureCLOCKGENDT5810B(int freq)
{

	Sleep(2000);

	//cfg500();
	
	unsigned int inclock, outclock;

	SMReadClockControl(&inclock, &outclock);

	if (inclock == 1)
	{
		if(niHAL->WriteReg(1, 	RFA_DT5810B_CLKGEN_INSEL ) != NI_OK)
			return NI_ERROR;
		Sleep (1000);
	}
	else
	{
		if(niHAL->WriteReg(0, 	RFA_DT5810B_CLKGEN_INSEL ) != NI_OK)
			return NI_ERROR;
	}

	if(niHAL->WriteReg(0, RFA_CLKGEN_STROBE ) != NI_OK)
		return NI_ERROR;
	
	//cfg1000_in_out(inclock, outclock);
	
	if (freq==1250)
	{
		if (niHAL->WriteReg(reversebits((0x00<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG0) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x01<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG1) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x02<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG2) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x03<<9) + 0x00), RFA_DT5810B_CLKGEN_CFG3) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x04<<9) + 0x0A), RFA_DT5810B_CLKGEN_CFG4) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x05<<9) + 0x03), RFA_DT5810B_CLKGEN_CFG5) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x06<<9) + 0xE8), RFA_DT5810B_CLKGEN_CFG6) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x07<<9) + 0x33), RFA_DT5810B_CLKGEN_CFG7) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x08<<9) + 0x9A), RFA_CLKGEN_CFG8) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x09<<9) + 0x8B), RFA_DT5810B_CLKGEN_CFG9) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x0A<<9) + 0x00), RFA_DT5810B_CLKGEN_CFGA) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x0B<<9) + 0x41), RFA_DT5810B_CLKGEN_CFGB) != NI_OK)
			return NI_ERROR;

				//STROBE
		if(niHAL->WriteReg(1, RFA_DT5810B_CLKGEN_STROBE ) != NI_OK)
			return NI_ERROR;
		if(niHAL->WriteReg(0, RFA_DT5810B_CLKGEN_STROBE ) != NI_OK)
			return NI_ERROR;


		Sleep (200);

	}
	else
	{
		if (niHAL->WriteReg(reversebits((0x00<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG0) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x01<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG1) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x02<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG2) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x03<<9) + 0xA0), RFA_DT5810B_CLKGEN_CFG3) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x04<<9) + 0x0A), RFA_DT5810B_CLKGEN_CFG4) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x05<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG5) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x06<<9) + 0xB0), RFA_DT5810B_CLKGEN_CFG6) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x07<<9) + 0x33), RFA_DT5810B_CLKGEN_CFG7) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x08<<9) + 0x3B), RFA_CLKGEN_CFG8) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x09<<9) + 0x48), RFA_DT5810B_CLKGEN_CFG9) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x0A<<9) + 0x04), RFA_DT5810B_CLKGEN_CFGA) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x0B<<9) + 0x41), RFA_DT5810B_CLKGEN_CFGB) != NI_OK)
			return NI_ERROR;

		//STROBE
		if(niHAL->WriteReg(1, RFA_DT5810B_CLKGEN_STROBE ) != NI_OK)
			return NI_ERROR;
		if(niHAL->WriteReg(0, RFA_DT5810B_CLKGEN_STROBE ) != NI_OK)
			return NI_ERROR;



		Sleep (200);


		if (niHAL->WriteReg(reversebits((0x00<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG0) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x01<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG1) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x02<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG2) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x03<<9) + 0xA0), RFA_DT5810B_CLKGEN_CFG3) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x04<<9) + 0x0A), RFA_DT5810B_CLKGEN_CFG4) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x05<<9) + 0x04), RFA_DT5810B_CLKGEN_CFG5) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x06<<9) + 0xB0), RFA_DT5810B_CLKGEN_CFG6) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x07<<9) + 0x33), RFA_DT5810B_CLKGEN_CFG7) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x08<<9) + 0x3B), RFA_CLKGEN_CFG8) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x09<<9) + 0x48), RFA_DT5810B_CLKGEN_CFG9) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x0A<<9) + 0x00), RFA_DT5810B_CLKGEN_CFGA) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(reversebits((0x0B<<9) + 0x41), RFA_DT5810B_CLKGEN_CFGB) != NI_OK)
			return NI_ERROR;

		//STROBE
		if(niHAL->WriteReg(1, RFA_DT5810B_CLKGEN_STROBE ) != NI_OK)
			return NI_ERROR;
		if(niHAL->WriteReg(0, RFA_DT5810B_CLKGEN_STROBE ) != NI_OK)
			return NI_ERROR;



		Sleep (200);
		
	}
		

	




	//if (niHAL->WriteReg(reversebits((0x00<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG0) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x01<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG1) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x02<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG2) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x03<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG3) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x04<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG4) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x05<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG5) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x06<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG6) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x07<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG7) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x08<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG8) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x09<<9) + 0x100), RFA_DT5810B_CLKGEN_CFG9) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x0A<<9) + 0x100), RFA_DT5810B_CLKGEN_CFGA) != NI_OK)
	//	return NI_ERROR;
	//if (niHAL->WriteReg(reversebits((0x0B<<9) + 0x100), RFA_DT5810B_CLKGEN_CFGB) != NI_OK)
	//	return NI_ERROR;
	//
	////STROBE
	//if(niHAL->WriteReg(1, RFA_DT5810B_CLKGEN_STROBE ) != NI_OK)
	//	return NI_ERROR;
	//if(niHAL->WriteReg(0, RFA_DT5810B_CLKGEN_STROBE ) != NI_OK)
	//	return NI_ERROR;

}





NI_RESULT DDE3_API::cfg1000_in_out(int clk_in, int clk_out)
{
	//CFG0
	if (niHAL->WriteReg(0x83400320, RFA_CLKGEN_CFG0) != NI_OK) //83800320
		return NI_ERROR;

	Sleep(5);
	//CFG1
	if (clk_in)
	{
		if (niHAL->WriteReg(0x83400321, RFA_CLKGEN_CFG1) != NI_OK) //83800301
			return NI_ERROR;

	}
	else
	{
		if (niHAL->WriteReg(0x83400301, RFA_CLKGEN_CFG1) != NI_OK) //83800301
			return NI_ERROR;

	}

	Sleep(5);
	//CFG2
	if (niHAL->WriteReg(0xEB040302, RFA_CLKGEN_CFG2) != NI_OK) //EB0C0302
		return NI_ERROR;

	Sleep(5);
	//CFG3
	if (clk_out)
	{
		if (niHAL->WriteReg(0x311E0303, RFA_CLKGEN_CFG3) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if (niHAL->WriteReg(0x68860303, RFA_CLKGEN_CFG3) != NI_OK)
			return NI_ERROR;
	}
	Sleep(5);
	//CFG4
	if (niHAL->WriteReg(0xEB040304, RFA_CLKGEN_CFG4) != NI_OK)
		return NI_ERROR;

	Sleep(5);
	
	//CFG5
	if (clk_in)
	{
		if (niHAL->WriteReg(0x10000AA5, RFA_CLKGEN_CFG5) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if (niHAL->WriteReg(0x10000B25, RFA_CLKGEN_CFG5) != NI_OK)
			return NI_ERROR;
	}
	

	Sleep(5);
	//CFG6
	if (niHAL->WriteReg(0x806E03F6, RFA_CLKGEN_CFG6) != NI_OK)
		return NI_ERROR;

	Sleep(5);
	//CFG7
	if (niHAL->WriteReg(0xBD997DF7, RFA_CLKGEN_CFG7) != NI_OK)
		return NI_ERROR;

	Sleep(5);
	//CFG8
	if (niHAL->WriteReg(0x20009D98, RFA_CLKGEN_CFG8) != NI_OK)
		return NI_ERROR;
	Sleep(5);

}


NI_RESULT DDE3_API::ConfigureCLOCKGEN()
{

		//POWER ON CLK
	if(niHAL->WriteReg(0, RFA_CLKGEN_POWER ) != NI_OK)
		return NI_ERROR;


	//POWER ON CLK
	if(niHAL->WriteReg(1, RFA_CLKGEN_POWER ) != NI_OK)
		return NI_ERROR;


	//cfg500();
	
	unsigned int inclock, outclock;

	SMReadClockControl(&inclock, &outclock);
	
	cfg1000_in_out(inclock, outclock);
	

	//STROBE
	if(niHAL->WriteReg(1, RFA_CLKGEN_STROBE ) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_CLKGEN_STROBE ) != NI_OK)
		return NI_ERROR;


	//CFG8
	if (niHAL->WriteReg(0x846E03F6, RFA_CLKGEN_CFG8) != NI_OK)
		return NI_ERROR;
	Sleep(5);

	//STROBE
	if(niHAL->WriteReg(1, RFA_CLKGEN_STROBE ) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_CLKGEN_STROBE ) != NI_OK)
		return NI_ERROR;
	Sleep(500);

	//CFG8
	if (niHAL->WriteReg(0x806E03F6, RFA_CLKGEN_CFG8) != NI_OK)
		return NI_ERROR;
	Sleep(5);

	//STROBE
	if(niHAL->WriteReg(1, RFA_CLKGEN_STROBE ) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_CLKGEN_STROBE ) != NI_OK)
		return NI_ERROR;
}




NI_RESULT DDE3_API::WriteDAC(unsigned int channel, unsigned int addr, unsigned int val)
{
	int mask_write;
	if (channel==0)
		mask_write = 2;		//DAC 1
	else
		mask_write = 1;		//DAC 2
		
	if (channel==0xF)
		mask_write = 0;		//DAC 1
	
	if (niHAL->WriteReg(mask_write, RFA_DACCNRL_DACSEL ) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg((addr<<8) + val, RFA_DACCNRL_VAL ) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(1, RFA_DACCNRL_STROBE ) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, RFA_DACCNRL_STROBE ) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(3, RFA_DACCNRL_DACSEL ) != NI_OK)
		return NI_ERROR;

}


unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

NI_RESULT DDE3_API::ReadDAC(unsigned int channel, unsigned int addr, unsigned int val, unsigned int *readdata)
{
	int mask_write;
	unsigned int VV[5];
	if (channel==0)
		mask_write = 2;		//DAC 1
	else
		mask_write = 1;		//DAC 2
		
	if (channel==0xF)
		mask_write = 0;		//DAC 1
	
	if (niHAL->WriteReg(mask_write, RFA_DACCNRL_DACSEL ) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(((0x80 + addr)<<8) + val, RFA_DACCNRL_VAL ) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(1, RFA_DACCNRL_STROBE ) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, RFA_DACCNRL_STROBE ) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(3, RFA_DACCNRL_DACSEL ) != NI_OK)
		return NI_ERROR;

	if (niHAL->ReadArray(VV, 0xFE000001-1,1) != NI_OK)
		return NI_ERROR;

	VV[0] = VV[0] >> 9;

	*readdata = reverse(VV[0]);

	return NI_OK;
}

NI_RESULT DDE3_API::ConfigureDAC(unsigned int channel, unsigned int ENABLE)
{
	unsigned int VVV[5];
	unsigned int v1;
	unsigned int v2;

	niHAL->WriteReg(0, RFA_HWINVERTB);
		Sleep(50);

	ReadHardwareProperties();
	//while(1)
	//{
		//DISABLE SIGNAL - STOP DCLK
		niHAL->WriteReg(3, 0xF0000031);			
		//DISABLE SYNC
		niHAL->WriteReg(3, RFA_DACCNRL_SYNC);
		Sleep(50);

		//DAC 1+2
		niHAL->WriteReg(0, RFA_DACCNRL_DACSEL);

		//REMOVE RESET
		niHAL->WriteReg(3, RFA_DACCNRL_RESET ); 

		//RESET DAC
		niHAL->WriteReg(0, RFA_DACCNRL_RESET);

		Sleep(50);
		//REMOVE RESET
		niHAL->WriteReg(3, RFA_DACCNRL_RESET);
		
		Sleep(50);

		

		//CONFIG3
		WriteDAC (0x0,0x3,0x70);
		WriteDAC (0x1,0x3,0x70);
		//ONLY PLL_BYPASS (DLL ON)
		
		//CONFIG5
		WriteDAC (0x0,0x5,0x82);
		WriteDAC (0x1,0x5,0x82);

		//CONFIG14		//SDO 0
		//WriteDAC (0x0,0xE,(0<<5));
		//CONFIG14		//SDO 1
		//WriteDAC (0x1,0xE,(0<<5));

		//CONFIG1		//INTERPOLATION DISABLE
		WriteDAC (0x0,0x1,0x00);
		WriteDAC (0x1,0x1,0x00);

		//CONFIG10		//FASE DLL 0
		WriteDAC (0x0,0xA, (0x0c << 4) + 0);
		//CONFIG10		//FASE DLL 1
		//WriteDAC (0x1,0xA, (0x0c << 4) + 8);		//COSI ERA QUANDO ANDAVA
		WriteDAC(0x1, 0xA, (0x0c << 4) + 0);

		//CONFIG6
		WriteDAC (0x0,0x6, 0x2E);
		WriteDAC (0x1,0x6, 0x2E);

		//CONFIG2		//ONE COMPLEMENT AND OTHER INIT PARAMS
		WriteDAC (0x0,0x2, 0x80);
		WriteDAC (0x1,0x2, 0x80);

		//CONFIG8		//RESET DELLA DLL
		WriteDAC (0x0,0x8, 0x04);
		WriteDAC (0x1,0x8, 0x04);
		Sleep(50);
		//DISABLE SIGNAL - START DCLK
		niHAL->WriteReg(1, 0xF0000031);
		Sleep(50);
		
		//CONFIG8		//RESTART DELLA DLL 0
		WriteDAC (0x0,0x8, 0x00);
		Sleep(50);
		//CONFIG8		//RESTART DELLA DLL 1
		WriteDAC (0x1,0x8, 0x00);
		Sleep(50);

		//CONFIG3
		WriteDAC (0x0,0x3, 0x60);
		WriteDAC (0x1,0x3, 0x60);



		//ABILITA CLOCK E DATI
		niHAL->WriteReg(0, 0xF0000031);

		//MODALITA' DI TEST
		niHAL->WriteReg(2, 0xF0000034);
		
		Sleep(50);


				//DISABLE SYNC
		niHAL->WriteReg(0x0, RFA_DACCNRL_SYNC);
		Sleep(50);


			niHAL->WriteReg(0, 0xF0000034);


	return NI_OK;

}






NI_RESULT DDE3_API::ProgramDataPhase(unsigned int channel, unsigned int pdir)
{
	
	//Idirizzo PHASE CH2
	if(niHAL->WriteReg(pdir, 0xF0000032 + channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


NI_RESULT DDE3_API::EnableChanel(unsigned int channel, unsigned int ENABLE)
{

	if (ENABLE > 1 || ENABLE <0 ) 
		return NI_PARAM_OUT_OF_RANGE;
		
	if (INSTRUMENTVERSION == DT5810)
	{
		if ( ENABLE == 1)
			niHAL->WriteReg(0, 0xF0000035); //Attivazione alimentazioni uscita
	}
	//CHANNEL ENABLE
	if(niHAL->WriteReg(ENABLE, RFA_CHANNEL_ENABLE + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	
	return NI_OK;
}



/*
	SETUP ENERGY EMULATION MODE
	mode			0					fixed energy
					1					spectrum
	energy			0 .. 2^15 e			fixed energy amplitude
*/
NI_RESULT DDE3_API::ConfigureEnergy(unsigned int channel, unsigned int MODE, unsigned int ENERGY)
{
	NI_STATUS Status;

	if(niHAL->WriteReg(MODE, RFA_ENERGY_FIXSPE + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	
	if(niHAL->WriteReg(ENERGY, RFA_ENERGY_FIXENERGY + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


/*
	SETUP ENERGY EMULATION MODE
	spectrum		*					spectrum
	spectrumLen							length of the spectrum vector
*/

NI_RESULT DDE3_API::ProgramSpectrum(unsigned int channel, unsigned int *spectrum, unsigned int spectrumLen)
{
	int i;
	unsigned int q;
	double max;
	double * cumulativo  = new double [32000];
	unsigned int * Icumulativo  = new unsigned int [32000];
	q=spectrum[0];
	cumulativo[0] = 0;
	for (i=1;i<16384;i++)
	{
		if (spectrumLen > i)
		{
			q=spectrum[i];
		}
		else
		{
			q=0;
		}

		cumulativo[i] = cumulativo[i-1] + q;
	}

	max = cumulativo[16383];

	for (i=0;i<16384;i++)
	{
		cumulativo[i] =  (( cumulativo[i] /  max) * (pow(2.0,32)-1));
		Icumulativo[i] = (unsigned int) cumulativo[i];
	}

	
	if(niHAL->WriteReg(1, RFA_ENERGY_PROGMODE + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	
//	for (int j=0;j<16384;j++)
//		if(niHAL->WriteReg(Icumulativo[j], j+ RFA_ENERGY_MEMBA + __CHOFFSET * channel) != NI_OK)
//			return NI_ERROR;

	if(niHAL->WriteArray(Icumulativo, RFA_ENERGY_MEMBA + __CHOFFSET * channel, 16384) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(0, RFA_ENERGY_PROGMODE + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	
	if(niHAL->WriteReg(1, RFA_ENERGY_FLUSH + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_ENERGY_FLUSH + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


/*
	SETUP TIMEBASE
	mode			0					fixed rate
					1					poisson
					2					external trigger
	rate			0 .. 1 MHz 			rate of the pulses
*/

NI_RESULT DDE3_API::ConfigureTimebase(	unsigned int channel, 
										unsigned int MODE, 
										double RATE, 
										unsigned long DeatTime, 
										bool Parallelizable)
{
	NI_STATUS Status;

	double fRATE;
	double pRATE;
	double RATE_KHz = RATE/1000.0;
	
	if (RATE < 0.001) RATE =0.001;
	
//	RATE = RATE + ((RATE_KHz * RATE_KHz * 0.0000041166) + (0.0009539873* RATE_KHz))*1000.0;

	fRATE = (_clockFREQUENCY/RATE)-1; 
	pRATE = ((RATE/4.0 * (1.0 / _clockFREQUENCY))) * pow(2.0, 32);

	if(niHAL->WriteReg(MODE, RFA_TIMEBASE_STATSOURCEMUX + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

		if(niHAL->WriteReg(fRATE, RFA_TIMEBASE_RATE + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(pRATE, RFA_TIMEBASE_POISSONP + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg((unsigned int) DeatTime, RFA_TIMEBASE_DEADTIME + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if (Parallelizable==true)
	{
		if(niHAL->WriteReg(1, RFA_TIMEBASE_PARFLAG + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if(niHAL->WriteReg(0, RFA_TIMEBASE_PARFLAG + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}



	return NI_OK;
}

/*
	SETUP DIGITAL CONNECTOR
	outtriggered	0					disabled
					1					enabled
	outtriggerlen	0... 10us			ns pulse len
					

*/

NI_RESULT DDE3_API::ConfigureDIO(unsigned int channel, unsigned int OUTTRIGGEREN, unsigned int OUTTRIGGERLEN)
{
	if(niHAL->WriteReg(OUTTRIGGEREN, RFA_TIMEBASE_TRIGGEROUTEN + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	int length;

	length = _clockFREQUENCY * ( ((double)OUTTRIGGERLEN) * 10e-9);
	if (length < 1)
		length = 1;

	if(niHAL->WriteReg(length, RFA_TIMEBASE_TRIGGEROUTLEN + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


/*
	SETUP DIGITAL RC
	risetime		0 .. 10us			risetime in ns
	falltime		0 .. 10ms			falltime in ns
*/

NI_RESULT DDE3_API::ConfigureDRC(unsigned int channel, unsigned int RISETIME, unsigned int FALLTIME, unsigned int ENABLE)
{
	
	double tau1;
	double tau2;
	double APolesF1[5];
	double APolesF2[5];
	int scale_decimator;
	long unsigned int APolesI1[5];
	long unsigned int APolesI2[5];

	RISETIME = RISETIME / 2;
	if (FALLTIME < 10) FALLTIME=10;

	if (FALLTIME > 5000000)
	{
		FALLTIME = FALLTIME / 8;
		RISETIME = RISETIME / 8;
		scale_decimator = 8;
	}
	else
	{
		if (FALLTIME > 200000)
		{
			FALLTIME = FALLTIME / 4;
			RISETIME = RISETIME / 4;
			scale_decimator = 4;
		}
		else
		if (FALLTIME > 100000)
		{
			FALLTIME = FALLTIME /2;
			RISETIME = RISETIME / 2;
			scale_decimator = 2;
		}
		else
			scale_decimator = 1;
	}
	


	if (RISETIME < 4)
	{
		if(niHAL->WriteReg(1, RFA_DRC_BYPASSLOWPASS + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
		RISETIME=2;
	}
	else
	{
		if(niHAL->WriteReg(0, RFA_DRC_BYPASSLOWPASS + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
	tau1 = 1-(1.0/(PI *2.0 * ((double)FALLTIME*1.25) * 1e-9 * _clockFREQUENCY/4));								
	tau2 = 1-(1.0/(PI *2.0 * ((double)RISETIME*1.25) * 1e-9 * _clockFREQUENCY/4));								

	APolesF1[0] = pow(tau1,1)* pow(2,32);
	APolesF1[1] = -1;
	APolesF1[2] = pow(tau1,2)* pow(2,32);
	APolesF1[3] = pow(tau1,3)* pow(2,32);
	APolesF1[4] = pow(tau1,4)* pow(2,32);
	
	APolesF2[0] = pow(tau2,1)* pow(2,32);
	APolesF2[1] = pow(1-tau2,1)* pow(2,32);
	APolesF2[2] = pow(tau2,2)* pow(2,32);
	APolesF2[3] = pow(tau2,3)* pow(2,32);
	APolesF2[4] = pow(tau2,4)* pow(2,32);

	for (int i=0;i<5;i++)
	{
		APolesI1[i] = (unsigned int) APolesF1[i];
		APolesI2[i] = (unsigned int) APolesF2[i];
	}


	if(niHAL->WriteReg((unsigned int) APolesI1[0], RFA_DRC_PALFA1 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((unsigned int) APolesI1[1], RFA_DRC_PALFA1B + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((unsigned int) APolesI1[2], RFA_DRC_PALFA2 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((unsigned int) APolesI1[3], RFA_DRC_PALFA3 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((unsigned int) APolesI1[4], RFA_DRC_PALFA4 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	

	if(niHAL->WriteReg((unsigned int) APolesI2[0], RFA_DRC_LALFA1 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((unsigned int) APolesI2[1], RFA_DRC_LALFA1B + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((unsigned int) APolesI2[2], RFA_DRC_LALFA2 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((unsigned int) APolesI2[3], RFA_DRC_LALFA3 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((unsigned int) APolesI2[4], RFA_DRC_LALFA4 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(1, RFA_DRC_RESET + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_DRC_RESET + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(0, RFA_DRC_OS + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(ENABLE, RFA_DRC_ENABLE + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if (scale_decimator == 1)
	{
		if (niHAL->WriteReg(0, RFA_DRC_DECIMATOR + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if (niHAL->WriteReg((((1)-1) << 8) + ((scale_decimator) - 1), RFA_DRC_DECIMATOR + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
	return NI_OK;
}


NI_RESULT DDE3_API::ConfigureTR(unsigned int channel, unsigned int ENABLE,  unsigned int RISETIME,  unsigned int LIMITHIGH, unsigned int SCALE)
{

	if (niHAL->WriteReg(ENABLE, RFA_DRC_MODE + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (ENABLE)
	{
		ConfigureDRC(channel, RISETIME, 100, true);
		if (niHAL->WriteReg(LIMITHIGH*(1<<(SCALE+1)), RFA_DRC_LIMITHIGH + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
		if (niHAL->WriteReg(SCALE, RFA_DRC_SCALE + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
}



NI_RESULT DDE3_API::ConfigureShapeGenerator(	unsigned int channel,
												int *shape,
												unsigned int shape_id,
												unsigned int shape_length, 
												int multishape_id,
												unsigned int rising_falling_crosspoint,
												unsigned int interpolator_factor_rising,
												unsigned int interpolator_factor_falling,
												bool reconfigure_shape,
												bool enable_shape)
{
	UINT32 interShape[4096];
	UINT32 forstep = 0;

	if (enable_shape)
	{
		if(niHAL->WriteReg(1,  shape_id * RFA_SHAPE_OFFSET +  RFA_SHAPE_ENABLE + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if(niHAL->WriteReg(0,  shape_id * RFA_SHAPE_OFFSET +  RFA_SHAPE_ENABLE + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;

	}

	if(niHAL->WriteReg(multishape_id,  shape_id * RFA_SHAPE_OFFSET +  RFA_SHAPE_MULTISHAPEID + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if ((interpolator_factor_rising>0) || (interpolator_factor_falling>0))
	{
		UINT32 alfa_up, alfa_down;
		UINT32 ratio_up, ratio_down;
		UINT32 en_up, en_down;
		UINT32 crosspoint;

		ratio_up = interpolator_factor_rising-1;
		ratio_down = interpolator_factor_falling-1;
		alfa_up = (0x10000/(ratio_up +2));
		alfa_down = (0x10000/(ratio_down +2));
		crosspoint = rising_falling_crosspoint / 2;
		forstep = 1;

		if (interpolator_factor_rising > 0)
			en_up = 1;
		else
		{
			en_up = 0;
			ratio_up = ratio_down;
			alfa_up = 0x7FFF;
		}


		if (interpolator_factor_falling > 0)
			en_down = 1;
		else
		{
			en_down = 0;
			ratio_down = ratio_up;
			alfa_down = 0x7FFF;
		}

		
		if(niHAL->WriteReg((alfa_up << 16) + ratio_up,  shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_INTERP_UP + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;

		if(niHAL->WriteReg((alfa_down << 16) + ratio_down,  shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_INTERP_DOWN + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;

		if(niHAL->WriteReg((en_down << 1) + en_up,  shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_INTERP_ENABLE + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;

		if(niHAL->WriteReg(crosspoint,  shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_INTERP_CROSS + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;
				
	}
	else
	{
		if(niHAL->WriteReg(0,  shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_INTERP_ENABLE + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;
	}

	if (reconfigure_shape)
		{
			if(niHAL->WriteReg(1,  shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_PROG + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;			

			memset(interShape,0,4096);

			if (forstep==0)
			{
				for (int j=0;j<shape_length/2;j++)
				{
					interShape[j]  = (((UINT32)shape[2*j]) << 0) + (((UINT32)shape[2*j+1]<<16) ) ;
				}
			}
			else
			{
				interShape[0]  = 0;
				interShape[1]  = 0;
				for (int j=0;j<shape_length/(2*4);j++)
				{
					interShape[j+2]  = (((UINT32)shape[(2*4)*j]) << 0) + (((UINT32)shape[((2*4)*j)+4]<<16) ) ;
				}
			}
			
			if(niHAL->WriteArray((unsigned int *) interShape, shape_id * RFA_SHAPE_OFFSET +  RFA_SHAPE_BA + __CHOFFSET * channel,2048) != NI_OK)
				return NI_ERROR;
		
			if(niHAL->WriteReg(interShape[0],  shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_BA + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;

			if(niHAL->WriteReg(interShape[1],  1+ shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_BA + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;
			if (forstep==0)
			{
				if(niHAL->WriteReg(((int)(shape_length)/2) +1 ,  shape_id * RFA_SHAPE_OFFSET +  RFA_SHAPE_LEN + __CHOFFSET * channel) != NI_OK)
					return NI_ERROR;			
			}
			else
			{
				if(niHAL->WriteReg(((int)(shape_length)/(2*4)) +1+2 ,  shape_id * RFA_SHAPE_OFFSET +  RFA_SHAPE_LEN + __CHOFFSET * channel) != NI_OK)
					return NI_ERROR;			
			}

			if(niHAL->WriteReg(0, shape_id * RFA_SHAPE_OFFSET  +  RFA_SHAPE_PROG + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;			

		}

	return NI_OK;
}

NI_RESULT DDE3_API::ConfigureMultishape(		unsigned int channel,
												double prob2,
												double prob3,
												double prob4,
												bool enable)
{
	if (enable)
	{
		double v1, v2, v3;
		unsigned int vi1, vi2, vi3;
		v1 = 100 - prob2 - prob3 -prob4;
		v2 = v1 + prob2;
		v3 = v2 + prob3;
		
		vi1 = (unsigned int) ((v1/100.0)  * ((double)  0xFFFF));
		vi2 = (unsigned int) ((v2/100.0)  * ((double)  0xFFFF));
		vi3 = (unsigned int) ((v3/100.0)  * ((double)  0xFFFF));

		
		if(niHAL->WriteReg(vi1, RFA_MSHAPE_PROB1 + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;	
		if(niHAL->WriteReg(vi2, RFA_MSHAPE_PROB2 + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;	
		if(niHAL->WriteReg(vi3, RFA_MSHAPE_PROB3 + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;	
		if(niHAL->WriteReg(0, RFA_MSHAPE_BYPASS + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;	

	}
	else
	{
		if(niHAL->WriteReg(1, RFA_MSHAPE_BYPASS + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;	
	}

}


/*
	SETUP DIGITAL RC
	gauss		0 ... 100				magnitude
	drift		0 ... 100				magnitude
*/

NI_RESULT DDE3_API::ConfigureNOISE(unsigned int channel, unsigned int RANDM, unsigned int GAUSSM, unsigned int DRIFTM, unsigned int FLIKERM, unsigned int FLIKERCorner)
{
   const unsigned int valori100[] = {1, 16777215, 16777216, 2, 16777214, 11430184, 3, 16777213, 7787294, 7, 16777209, 5305422, 15, 16777201,
                            3614542, 33, 16777183, 2462559, 70, 16777146, 1677722, 151, 16777065, 1143018, 326, 16776890, 778729, 703,
                            16776513, 530542};

   const unsigned int valori500[] = {1, 16777215, 16777216, 2, 16777214, 10452533, 5, 16777211, 6512133, 12, 16777204, 4057186, 31, 16777185,
                            2527706, 80, 16777136, 1574810, 205, 16777011, 981138, 529, 16776687, 611268, 1364,
                           16775852, 380832, 3513, 16773703, 237266};

   const unsigned int valori1k[] = {1, 16777215, 16777216, 2, 16777214, 9197420, 8, 16777208, 5042108, 26, 16777190, 2764128, 86,
                            16777130, 1515320, 287, 16776929, 830712, 954, 16776262, 455404, 3173, 16774043, 249656, 10557, 16766659, 136864, 35101,
                            16742115, 75030};

   const unsigned int valori5k[] = {1, 16777215, 16777216, 2, 16777214, 10057677, 5, 16777211, 6029419, 15, 16777201, 3614542, 42, 16777174,
                            2166861, 117, 16777099, 1298999, 326, 16776890, 778729, 908, 16776308, 466836, 2525, 16774691, 279861,
                            7026, 16770190, 167772};

   const unsigned int valori10k[] = {1, 16777215, 16777216, 3, 16777213, 8849977, 9, 16777207, 4668360, 33, 16777183, 2462559, 117, 16777099, 1298999,
                            421, 16776795, 685222, 1514, 16775702, 361454, 5440, 16771776, 190667, 19543, 16757673, 100577,
                            70129, 16707087, 53054};

   const unsigned int valori50k[] = {1, 16777215, 16777216, 3, 16777213, 8093017, 13, 16777203, 3903921, 56, 16777160, 1883179, 240, 16776976, 908410,
                            1030, 16776186, 438200, 4427, 16772789, 211380, 19015, 16758201, 101966, 81565, 16695651, 49186, 347752,
                        16429464, 23727};


   const unsigned int valori100k[] = {1, 16777215, 16777216, 3, 16777213, 7787294, 15, 16777201, 3614542, 70, 16777146, 1677722, 326, 16776890,
                            778729, 1514, 16775702, 361454, 7026, 16770190, 167772, 32588, 16744628, 77873, 150726, 16626490, 36145,
                            688444, 16088772, 16777};

   const unsigned int valori500k[] = {1, 16777215, 16777216, 4, 16777212, 7121228, 22, 16777194, 3022664, 120, 16777096, 1282995, 667, 16776549, 544578, 3702,
							16773514, 231151, 20536, 16756680, 98114, 113670, 16663546, 41645, 621415, 16155801, 17677, 3191294,
							13585922, 7503};

   const unsigned int valori1M[] = {1, 16777215, 16777216, 4, 16777212, 6852215, 25, 16777191, 2798608, 151, 16777065, 1143018, 908, 16776308, 466836,
							5440, 16771776, 190667, 32588, 16744628, 77873, 194417, 16582799, 31805, 1133151, 15644065, 12990,
							5881965, 10895251, 5305
										};

   const unsigned int  *valori;

	NI_STATUS Status;

	if(niHAL->WriteReg(RANDM, RFA_RN_MAG + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;


	//HACK FORSE DA RIMUOVERE
	if (niHAL->WriteReg(1, RFA_RW_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, RFA_RW_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;


	
	if (niHAL->WriteReg(1, RFA_NOISE_ENABLELFSRUSB + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, RFA_NOISE_CTRLLFSR + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	

	if(niHAL->WriteReg(GAUSSM, RFA_NOISE_GAUSSM + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(0x12248741, RFA_NOISE_TEMPSEED1 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0xA03f23AB, RFA_NOISE_TEMPSEED2 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;


	if (niHAL->WriteReg(1, RFA_NOISE_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, RFA_NOISE_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;





	if(niHAL->WriteReg(DRIFTM, RFA_RW_MAG + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(FLIKERM, RFA_NF_MAGNITUDE + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;




	switch( FLIKERCorner )
	{
		case 0:
			valori = valori100;
			break;
		case 1:
			valori = valori500;
			break;
		case 2:
			valori = valori1k;
			break;
		case 3:
			valori = valori5k;
			break;
		case 4:
			valori = valori10k;
			break;
		case 5:
			valori = valori50k;
			break;
		case 6:
			valori = valori100k;
			break;
		case 7:
			valori = valori500k;
			break;
		case 8:
			valori = valori1M;
			break;
		default :
			valori = valori100;

	}


	for (int i = 0;i<30;i++)
	{
		unsigned int UVal ;
		if (valori[i] >= 0)
			UVal = valori[i];
		else
			UVal = valori[i]+0xFFFFFF;

		if(niHAL->WriteReg(UVal, i+RFA_NF_COEFF + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;

	}
    

	return NI_OK;
}


/*
	SETUP GENERAL
	gain			0..4				
	offset			-2^15 .. 2^15
	invert			0 / 1
					

*/

NI_RESULT DDE3_API::ConfigureGeneral(unsigned int channel, double gain, int offset, unsigned int invert, unsigned int OUTFILTER, unsigned int ANALOGSEL)
{
	NI_STATUS Status;
	unsigned int gainn;
	gainn = (unsigned int)(gain * 4096);

	if(niHAL->WriteReg(gainn, RFA_OFFGAIN_GAIN + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (channel==0)
	{
		if(niHAL->WriteReg(offset-510, RFA_OFFGAIN_OFFSET + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if(niHAL->WriteReg(offset-850, RFA_OFFGAIN_OFFSET + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}

	if(niHAL->WriteReg(invert, RFA_OFFGAIN_INVERT + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;


	SetAnalogDatapath(channel, OUTFILTER, ANALOGSEL);


	return NI_OK;
}


NI_RESULT DDE3_API::ConfigureLFSR(unsigned int channel, unsigned long SEED, t_LFSR_ID LFSR_ID, t_LFSR_OPERATION Operation)
{
	NI_STATUS Status;
	unsigned int gainn;

	switch (Operation)
	{
		case LFSR_REPROGRAM:
			switch (LFSR_ID)
			{
				case LFSR_ENERGY:
					if(niHAL->WriteReg(1, RFA_ENERGY_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
						return NI_ERROR;
					if(niHAL->WriteReg(0, RFA_ENERGY_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
						return NI_ERROR;
					break;

				case LFSR_TIMEBASE:
					if(niHAL->WriteReg(1, RFA_TIMEBASE_SEEDPROG + __CHOFFSET * channel) != NI_OK)
						return NI_ERROR;
					if(niHAL->WriteReg(0, RFA_TIMEBASE_SEEDPROG + __CHOFFSET * channel) != NI_OK)
						return NI_ERROR;
					break;

				case LFSR_NOISE_GAUSS:
					if(niHAL->WriteReg(1, RFA_NOISE_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
						return NI_ERROR;
					if(niHAL->WriteReg(0, RFA_NOISE_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
						return NI_ERROR;
					break;

				case LFSR_NOISE_RW:
					if(niHAL->WriteReg(1, RFA_RW_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
						return NI_ERROR;
					if(niHAL->WriteReg(0, RFA_RW_STARTSEEDGEN + __CHOFFSET * channel) != NI_OK)
						return NI_ERROR;
					break;

				case LFSR_NOISE_FLIKR:

					break;

				case LFSR_NOISE_RN:

					break;


			}


		default:
			return NI_ERROR;
	}




	return NI_OK;
}


NI_RESULT DDE3_API::ConfigureBaselineDrift(		unsigned int channel,
												int *baseline,
												unsigned int baseline_length, 
												unsigned int interpolator_factor_slow,
												unsigned int interpolator_factor_fast,
												bool reconfigure_drift,
												bool enable_shape,
												bool reset)
{



	UINT32 interShape[5000];

	if (enable_shape)
	{
		if(niHAL->WriteReg(1,  RFA_BLD_ENABLE + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if(niHAL->WriteReg(0,  RFA_BLD_ENABLE + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;

	}

	if(niHAL->WriteReg(50*interpolator_factor_slow-1,  RFA_BLD_RSLOW + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;

	if(niHAL->WriteReg(10*interpolator_factor_fast-1,  RFA_BLD_RFAST + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;


	if(niHAL->WriteReg( 0xFFFFFFFF/(interpolator_factor_slow*50),  RFA_BLD_ASLOW + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;

	if(niHAL->WriteReg(0xFFFFFFFF/ (interpolator_factor_fast*10),  RFA_BLD_AFAST + __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;


	if(niHAL->WriteReg(((int)(baseline_length)),  RFA_BLD_MEMLEN  + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;			


	if (reconfigure_drift)
		{
			if(niHAL->WriteReg(1,  RFA_BLD_PROG + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;			

			if (baseline_length > 4095)
				baseline_length=4095;
			for (int j=0;j<baseline_length;j++)
			{
				interShape[j]  = baseline[j];// (((UINT32)baseline[2*j]) << 0) + (((UINT32)baseline[2*j+1]<<16) ) ;

			}
			
			if(niHAL->WriteArray((unsigned int *) interShape, RFA_BLD_BA + __CHOFFSET * channel, baseline_length) != NI_OK)
				return NI_ERROR;
		
			if(niHAL->WriteReg(interShape[0],  RFA_BLD_BA + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;

			
			
			if(niHAL->WriteReg(0,  RFA_BLD_PROG + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;						

		}

	
	if(niHAL->WriteReg(((int)(baseline_length)),  RFA_BLD_MEMLEN  + __CHOFFSET * channel) != NI_OK)
				return NI_ERROR;		

}



/*
	CH TO VOLTAGE
	channel		0 ... 2^15					
*/

double DDE3_API::ChannelsToVoltage(int channel, int channels)
{
	return _VoltageChannelFactor[channel] * channels;
}



/*
	VOLTAGE TO CHANNEL
	channel		0 ... 2^15					
*/

unsigned int DDE3_API::VoltageToChannels(unsigned int channel, double voltage)
{
	return voltage / _VoltageChannelFactor[channel] ;
}


/*
	GET STATS
	cps				counts per second
	ccounter		general event counter
	sat				saturation counter
	overflow		non poisson event generated
*/

NI_RESULT DDE3_API::GetStat(unsigned int channel, unsigned int *cps,  unsigned int *live, UINT64 *ccounter, unsigned int *overflow)
{
	unsigned int temp, temp2;
	if(niHAL->ReadReg(&temp, RFA_MON_CPSR + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	*cps = temp;

	if(niHAL->ReadReg(&temp, RFA_MON_CPSLIVER + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	*live = temp;

	if(niHAL->ReadReg(&temp, RFA_MON_CPSR_CCOUNTER1 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->ReadReg(&temp2, RFA_MON_CPSR_CCOUNTER2 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	*ccounter = temp + temp2 << 32;

	if(niHAL->ReadReg(&temp, RFA_MON_CPSR_CCOUNTER1 + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	*overflow = temp;

	return NI_OK;
}


/*
	RESETS THE USER COUNTER

	*/
NI_RESULT DDE3_API::ResetCCounter(unsigned int channel)
{
	if(niHAL->WriteReg(1, RFA_MON_CPSR_RESET + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_MON_CPSR_RESET + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	
	return NI_OK;
}

NI_RESULT DDE3_API::UnlockDisplays()
{
	if(niHAL->WriteReg(0xF1CA, RFA_CONNECTED+ RFA_UC_BA + 0) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


NI_RESULT DDE3_API::UpdateDisplayStatus(unsigned int channel, int timemode, int rate, int ratep, char *time_str, int energy_mode, int energy, char *energy_str, char *shape_str, int live, int funcgen, int funcgen_mvolt, int funcgen_freq, char *funcgen_str)
{
	if (channel > 1)
		return NI_OK;
	unsigned int tmpstr[20];
	int i;

	if(niHAL->WriteReg(timemode, RFA_DISPLAY_TIMEMODE+ RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg((rate >> 16) ,RFA_DISPLAY_RATE + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((rate & 0xFFFF) ,1 + RFA_DISPLAY_RATE + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;
	
	if(niHAL->WriteReg((ratep >> 16) ,RFA_DISPLAY_RATEP + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg((ratep & 0xFFFF) ,1 + RFA_DISPLAY_RATEP + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;

	
	if ((time_str != NULL) )
	{
		i=0;
		memset(tmpstr,0,20*4);
		while((*time_str != '\0') && (i<7))
		{
			tmpstr[i++] = (*time_str << 8) + *(time_str+1);
			time_str+=2;
		}
		if(niHAL->WriteArray(tmpstr, RFA_DISPLAY_TIMESTR + RFA_UC_BA + RFA_UC_CHOFF * channel, 7) != NI_OK)
			return NI_ERROR;

	}
	if(niHAL->WriteReg(energy_mode ,RFA_DISPLAY_ENERGYMODE + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;

	if ((energy_str != NULL) )
	{
		i=0;
		memset(tmpstr,0,20*4);
		while((*energy_str != '\0') && (i<14))
		{
			tmpstr[i++] = (*energy_str << 8) + *(energy_str+1);
			energy_str+=2;
			 
		}
		if(niHAL->WriteArray(tmpstr, RFA_DISPLAY_ENERGYSTR + RFA_UC_BA + RFA_UC_CHOFF * channel, 7) != NI_OK)
			return NI_ERROR;

	}

	if(niHAL->WriteReg(energy ,RFA_DISPLAY_ENERGY + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;

	if ((shape_str != NULL) )
	{
		i=0;
		memset(tmpstr,0,20*4);
		while((*shape_str != '\0') && (i<14))
		{
			tmpstr[i++] = (*shape_str << 8) + *(shape_str+1);
			shape_str+=2;
		}
		if(niHAL->WriteArray(tmpstr, RFA_DISPLAY_SHAPE + RFA_UC_BA + RFA_UC_CHOFF * channel, 7) != NI_OK)
			return NI_ERROR;

	}

	if(niHAL->WriteReg(live ,RFA_DISPLAY_LIVE + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;
	

	if ((shape_str != NULL))
	{
		i = 0;
		memset(tmpstr, 0, 20 * 4);
		while ((*funcgen_str != '\0') && (i<14))
		{
			tmpstr[i++] = (*funcgen_str << 8) + *(funcgen_str + 1);
			funcgen_str += 2;
		}
		if (niHAL->WriteArray(tmpstr, RFA_DISPLAY_FUNCGEN_STR + RFA_UC_BA + RFA_UC_CHOFF * channel, 7) != NI_OK)
			return NI_ERROR;

	}
	if (niHAL->WriteReg(funcgen, RFA_DISPLAY_FUNCGEN + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(funcgen_mvolt, RFA_DISPLAY_FUNCGEN_VOLT + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(funcgen_freq, RFA_DISPLAY_FUNCGEN_FREQ + RFA_UC_BA + RFA_UC_CHOFF * channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}




NI_RESULT DDE3_API::SetAnalogDatapath(unsigned int channel, unsigned int filter, unsigned int hvlv)
{
	
	if (INSTRUMENTVERSION == DT5810B)
	{
		hvlv = 1;
		filter = !filter;
	}

	if (channel == 0)
		peout1 = (peout1 & 0x3) + (hvlv << 3) + (filter << 2);
	else
		if (channel == 1)
			peout1 = (peout1 & 0xC) + (filter << 1) + (hvlv << 0);
		else
			return NI_OK;

	//uint32_t p;
	//niHAL->ReadReg(&p, RFA_PORTEXP3 + 0xFF000000); // RFA_UC_BA 



	if (niHAL->WriteReg(peout1, RFA_PORTEXP3 + RFA_UC_BA) != NI_OK)
		return NI_ERROR;


	if (niHAL->WriteReg(hvlv, RFA_HWINVERTA+channel) != NI_OK)
			return NI_ERROR;



	return NI_OK;

}


NI_RESULT DDE3_API::SetInputAnalogDatapath(	unsigned int channel, 
												unsigned int input_imp,
												unsigned int input_div,
												unsigned int input_mode,
												unsigned int input_scale,
												unsigned int input_gain,
												double offset
												)
{
	int word;
	int ioffset;
	word=0;
	word += (1-input_imp) << 7;
	word += (1-input_div) << 6;
	word += input_mode << 5;
	switch(input_scale)
	{
		case 2:
			word += 0 << 4;
			word += 0 << 3;
			break;

		case 1:
			word += 1 << 4;
			word += 0 << 3;
			break;

		case 0:
			word += 1 << 4;
			word += 1 << 3;
			break;

		default:
			word += 0 << 4;
			word += 0 << 3;
	}

	
	ioffset = 32576 + offset;

	if (niHAL->WriteReg(word, RFA_PORTEXP2 + RFA_UC_BA) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(offset, RFA_IN_OFFSET + RFA_UC_BA) != NI_OK)
		return NI_ERROR;

}



NI_RESULT DDE3_API::SetInputAnalogMix(		    unsigned int channel, 
												double gain_a,
												int offset_a,
												double gain_b,
												int offset_b,
												unsigned int enable_a,
												unsigned int enable_b,
												unsigned int inv_a,
												unsigned int inv_b
									)
{
	int dgain;
	unsigned int doffset;

	
	dgain = gain_a * 256;
	if (offset_a < 0)
		doffset = 0xFFFF-offset_a;
	else
		doffset = offset_a;
	

	if (niHAL->WriteReg(enable_a, ADC_ENABLE_A + MCA_BASE+ ADC_BASE) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(enable_b, ADC_ENABLE_B + MCA_BASE+ ADC_BASE) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(dgain, ADC_GAIN_A + MCA_BASE+ ADC_BASE) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(dgain, ADC_GAIN_B + MCA_BASE+ ADC_BASE) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(offset_a, ADC_OFFSET_A + MCA_BASE+ ADC_BASE) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(offset_b, ADC_OFFSET_B + MCA_BASE+ ADC_BASE) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(inv_a, ADC_INV_A + MCA_BASE+ ADC_BASE) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(inv_b, ADC_INV_B + MCA_BASE+ ADC_BASE) != NI_OK)
		return NI_ERROR;

	return NI_OK;

}

NI_RESULT DDE3_API::SetDIO(		    DIO_IN_MODE Dio_In_A, 
									DIO_IN_MODE Dio_In_B, 
									DIO_OUT_MODE Dio_Out_A,
									DIO_OUT_MODE Dio_Out_B,
									unsigned int DIO_Out_PulseLen,
									double delayA,
									double delayB
							)
{
	
	unsigned int func__in_a;
	unsigned int func__in_b;
	unsigned int func__out_a;
	unsigned int func__out_b;
	
	func__in_a = 0;
	func__in_b = 0;
	switch(Dio_In_A)
	{
	case IN_OFF:
		func__in_a = 0x0;
		break;
	case TRIGGER_GATE_0:
		func__in_a = 0x1;
		break;
	case TRIGGER_GATE_1:
		func__in_a = 0x2;
		break;
	case GATE_0:
		func__in_a = 0x4;
		break;
	case GATE_1:
		func__in_a = 0x8;
		break;
	case TRIGGER_IN_0:
		func__in_a = 0x10;
		break;
	case TRIGGER_IN_1:
		func__in_a = 0x20;
		break;
	case RUNSTOP:
		func__in_a = 0x40;
		break;
	case STEPOVER:
		func__in_a = 0x80;
		break;
	case RESET_LFSR:
		func__in_a = 0x100;
		break;
	case BASELINERESET:
		func__in_a = 0x200;
		break;
	default:
		func__in_a = 0x0;
	}

	switch(Dio_In_B)
	{
	case OUT_OFF:
		func__in_b = 0x0;
		break;
	case TRIGGER_GATE_0:
		func__in_b = 0x1;
		break;
	case TRIGGER_GATE_1:
		func__in_b = 0x2;
		break;
	case GATE_0:
		func__in_b = 0x4;
		break;
	case GATE_1:
		func__in_b = 0x8;
		break;
	case TRIGGER_IN_0:
		func__in_b = 0x10;
		break;
	case TRIGGER_IN_1:
		func__in_b = 0x20;
		break;
	case RUNSTOP:
		func__in_b = 0x40;
		break;
	case STEPOVER:
		func__in_b = 0x80;
		break;
	case RESET_LFSR:
		func__in_b = 0x100;
		break;
	case BASELINERESET:
		func__in_b = 0x200;
		break;
	default:
		func__in_b = 0x0;

	}

	if ((Dio_Out_A >=OUT_OFF ) && (Dio_Out_A <=LOOP_1))
		func__out_a = (unsigned int) Dio_Out_A;
	else
		func__out_a = 0x00;

	if ((Dio_Out_B >=OUT_OFF ) && (Dio_Out_B <= LOOP_1))
		func__out_b = (unsigned int) Dio_Out_B;
	else
		func__out_b = 0x00;

	if (niHAL->WriteReg(func__in_a, RFA_DIO_FUNCINA) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(func__in_b, RFA_DIO_FUNCINB) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(Dio_Out_A, RFA_DIO_FUNCOUTA) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(Dio_Out_B, RFA_DIO_FUNCOUTB) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(DIO_Out_PulseLen, RFA_DIO_PULSELEN) != NI_OK)
		return NI_ERROR;

	uint32_t delay_idx_a=0;
	uint32_t delay_idx_b = 0;
	if (delayA > 0)
		delay_idx_a = _clockFREQUENCY * delayA*10e-9;
	if (delayB > 0)
		delay_idx_b = _clockFREQUENCY * delayB*10e-9;



	if (niHAL->WriteReg(delay_idx_a, CORRELATION_DELAY_DIGA + CORRELATION_DELAY_OFFSET + CORRELATION_BASE) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(delay_idx_b, CORRELATION_DELAY_DIGB + CORRELATION_DELAY_OFFSET + CORRELATION_BASE) != NI_OK)
		return NI_ERROR;


	return NI_OK;

}


NI_RESULT DDE3_API::SetRunControlMode(unsigned int channel, tRUNCONTROL_MODE rmode, double limitvalue)
{
	ULONG64 timetarget;

	timetarget = limitvalue * _clockFREQUENCY;

	if (niHAL->WriteReg((unsigned int)rmode, RUNCONTROL_MODE+ __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg((timetarget >> 0) & 0xFFFFFFFF, RUNCONTROL_TIMETARGET1+ __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg((timetarget >> 32) & 0xFFFFFFFF, RUNCONTROL_TIMETARGET2+ __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(((unsigned int)limitvalue) & 0xFFFFFFFF, RUNCONTROL_CONTARGET+ __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


NI_RESULT DDE3_API::RunControlResetStat(unsigned int channel)
{

	if (niHAL->WriteReg(1, RUNCONTROL_RESETSTAT+ __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, RUNCONTROL_RESETSTAT+ __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	return NI_OK;
}

NI_RESULT DDE3_API::RunControlResetStart(unsigned int channel)
{

	if (niHAL->WriteReg(1, RUNCONTROL_RESETSTART+ __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, RUNCONTROL_RESETSTART+ __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	return NI_OK;
}


NI_RESULT DDE3_API::RunControlEnable(unsigned int channel, bool enable)
{
	if (enable)
	{
		if (niHAL->WriteReg(1, RUNCONTROL_ENABLE+ __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if (niHAL->WriteReg(0, RUNCONTROL_ENABLE+ __CHOFFSET * channel) != NI_OK)
			return NI_ERROR;
	}

	return NI_OK;
}





NI_RESULT DDE3_API::DelayAndCorrelationControl( unsigned int correlation_mode,
												unsigned int enableCchannel,
												double delay)
{
	unsigned int idelay;
	idelay = delay * 1000 * _clockFREQUENCY/250000000.0 ;
	unsigned int mode;
	mode = (correlation_mode & 0x7) + (enableCchannel<<3);

	if (niHAL->WriteReg(idelay, CORRELATION_DELAY_VAL + CORRELATION_DELAY_OFFSET+ CORRELATION_BASE) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(mode,  CORRELATION_DELAY_MODE + CORRELATION_DELAY_OFFSET+ CORRELATION_BASE) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}



NI_RESULT DDE3_API::TimebaseMux( unsigned int channel,
								 unsigned int mode)
{
	
	if(niHAL->WriteReg(mode, RFA_TIMEMUX + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


NI_RESULT DDE3_API::EnergyMux( unsigned int channel,
								 unsigned int mode)
{
	
	if(niHAL->WriteReg(mode, RFA_ENERGYMUX + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


/*
	GET AN OUTPUT SHAPE
	data			vector with downloaded shape
	len				length of the data downloaded
	*/
NI_RESULT DDE3_API::GetSignalLoopback(unsigned int channel, INT32 *data, unsigned int *len)
{
	int i;
	unsigned int temp;
	int st;
	//unsigned int * buffer  = new unsigned int [20000];
	unsigned int buffer[20000];
	//if(niHAL->ReadReg(&temp, RFA_MON_CPSR_DATAREADY + __CHOFFSET * channel) != NI_OK)
	//	return NI_ERROR;
	
	//if (temp & 0x01 == 1)
	while (1)
	{
		if(niHAL->ReadArray(buffer, RFA_MON_MEMBA + __CHOFFSET * 2*channel, 8192) != NI_OK)
			return NI_ERROR;
		if(niHAL->WriteReg(1, RFA_MON_RESTOREACQ + __CHOFFSET *  channel) != NI_OK)
			return NI_ERROR;
		if(niHAL->WriteReg(0, RFA_MON_RESTOREACQ + __CHOFFSET *channel) != NI_OK)
			return NI_ERROR;

		for (i=0;i<8192;i++)
		{
			st = (INT32) (buffer[i] & 0xFFF);
			if (st>0x7FF)
				data[i] = st-0xFFF;
			else
				data[i] = st;
			
		}
		*len = 8192;
	//	delete [] buffer;
		return NI_OK;
	}
	//else
	//	return NI_NO_DATA_AVAILABLE;
	
}


NI_RESULT DDE3_API::ResetSpectrum(unsigned int channel)
{
	if(niHAL->WriteReg(1, RFA_SPELOOP_RESETSM + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_SPELOOP_RESETSM + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	
	return NI_OK;
}



NI_RESULT DDE3_API::ReprogramLFSREnergy(ULONG64 seed, unsigned int channel)
{

	return NI_OK;
}
NI_RESULT DDE3_API::ReprogramLFSRTimebase(ULONG64 seed, unsigned int channel)
{
	//STROBE SEED GENERATION
	if(niHAL->WriteReg(1, RFA_TIMEBASE_SEEDPROG + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_TIMEBASE_SEEDPROG + __CHOFFSET * channel) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


NI_RESULT DDE3_API::ReprogramLFSRMultishape(ULONG64 seed, unsigned int channel)
{
	return NI_OK;
}

NI_RESULT DDE3_API::ReprogramLFSRNoise(ULONG64 seed, unsigned int channel)
{
	return NI_OK;
}


/*
	GET SPECTRUM GENERATED
	data			vector with downloaded shape
	len				length of the data downloaded
	*/
NI_RESULT DDE3_API::GetSpectrumLoopback(unsigned int channel, UINT32*data, unsigned int *len)
{
	int i;
	//unsigned int * buffer  = new unsigned int [5000];
	unsigned int  buffer[5000];
	
		if (niHAL->ReadArray(buffer, RFA_SPELOOP_MEMBA + __CHOFFSET * 2* channel, 4096) != NI_OK)
			return NI_ERROR;

	
	for (i=0;i<4096;i++)
		data[i] = (UINT32)buffer[i];

	*len = 4096;
	//delete [] buffer;
	return NI_OK;

}


/*
	ACTIVATE GET UID
	*data			vector with UID
	*/
NI_RESULT DDE3_API::ActiveteGetUID(unsigned int *key1, unsigned int *key2)
{
	NI_STATUS Status;
	unsigned int v;
	if (niHAL->ReadReg (&v, RFA_ICAPDNA_UID1)!= NI_OK)
		return NI_ERROR ;
	*key1 = v;
	if (niHAL->ReadReg (&v, RFA_ICAPDNA_UID2)!= NI_OK)
		return NI_ERROR ;
	*key2 = v;
	return NI_OK;

}

/*
	ACTIVATE PROGRAM KEY
	*data			vector with the key
	*/
NI_RESULT DDE3_API::ActiveteRegister(unsigned char *data)
{
	int i;


	NI_STATUS Status;

	return NI_OK;

}

NI_RESULT DDE3_API::IsActivated(unsigned int *data)
{
	unsigned int v;
	if (niHAL->ReadReg (&v, RFA_ICAPDNA_ENABLE)!= NI_OK)
		return NI_ERROR ;

	if ( (v & 0x01) == 1)
		*data = 1;
	else
		*data = 0;
	NI_STATUS Status;

	return NI_OK;

}

/*
	GET HARDWARE INFORMATION

	*/

NI_RESULT DDE3_API::HardwareInfo(unsigned int *HWREV, unsigned int *UCREV, unsigned int *FWREV, unsigned int *HWOPTIONS, unsigned int *DEVICEMODEL)
{

	if (niHAL->ReadReg(DEVICEMODEL, RFA_INFO_ISTRVER) != NI_OK)
		return NI_ERROR ;

	if (niHAL->ReadReg(HWREV, RFA_INFO_BOARDREV) != NI_OK)
		return NI_ERROR ;

	if (niHAL->ReadReg(FWREV, RFA_INFO_FIRMWARE_RELEASE) != NI_OK)
		return NI_ERROR ;

	if (niHAL->ReadReg(HWOPTIONS, RFA_INFO_OPTIONS) != NI_OK)
		return NI_ERROR;

	if (niHAL->ReadReg(UCREV, RFA_INFO_UCRELEASE) != NI_OK)
		return NI_ERROR ;

	NI_STATUS Status;

	return NI_OK;
}



NI_RESULT DDE3_API::EmulatorAWGModeControl(UINT32 mode, UINT32 channel)
{
	if (niHAL->WriteReg(mode, DDR_WFENABLE + channel) != NI_OK)
		return NI_ERROR;
		
	return NI_OK;

}

NI_RESULT DDE3_API::EmulatorAWGProgramScaler(UINT32 scaler, UINT32 channel)
{
	if (channel == 0)
	{
		if (niHAL->WriteReg(scaler, DDR_SCALER1) != NI_OK)
			return NI_ERROR;
	}
	if (channel == 1)
	{
		if (niHAL->WriteReg(scaler, DDR_SCALER2) != NI_OK)
			return NI_ERROR;
	}

	return NI_OK;
}

NI_RESULT DDE3_API::ProgramDDR(UINT32 *data1, UINT32 *time1,  UINT32 length1, UINT32 *data2, UINT32 *time2, UINT32 length2, UINT32 memorymode1, UINT32 memorymode2)
{
	UINT32 TEMPVECT[1024];
	int divider1, divider1_base;
	int divider2, divider2_base;
	int lq1, lq2, lq;
	int residual1, residual2, residual;
	int memorymode = 0;
	UINT32 *data = NULL;
	UINT32 *time = NULL;
	int current_transfer_length;

	if (length1 > 0)
	{
		if (memorymode1 == 0)
		{
			if (length1 % 16)
				return NI_PARAM_INVALID;

			divider1 = 2048;
			divider1_base = 16;
		}
		else
		{
			if (length1 % 8)
				return NI_PARAM_INVALID;

			divider1 = 1024;
			divider1_base = 8;
		}
		lq1 = ceil(((float)length1) / ((float)divider1));
		residual1 = (length1 - (lq1 - 1) * divider1) / (divider1_base/8);
	}
	if (length2 > 0)
	{
		if (memorymode2 == 0)
		{
			if (length2 % 16)
				return NI_PARAM_INVALID;

			divider2 = 2048;
			divider2_base = 16;
		}
		else
		{
			if (length2 % 8)
				return NI_PARAM_INVALID;
			divider2 = 1024;
			divider2_base = 8;
		}
		lq2 = ceil(((float)length2) / ((float)divider2));
		residual2 = (length2 - (lq2 - 1) * divider2) / (divider2_base / 8);
	}





	


	//DDR3 idle mode
	niHAL->WriteReg(0, DDR_CTRL_REG);

	Sleep(10);
	int ch = 0;
	for (int ch = 0; ch < 2; ch++)
	{
		
		if (ch == 0)
		{
			if (length1 < 1) continue;
			lq = lq1; residual = residual1; data = data1; time = time1;  memorymode = memorymode1;
		
		}

		if (ch == 1)
		{
			if (length2 < 1) continue;
			lq = lq2; residual = residual2; data = data2; time = time2; memorymode = memorymode2;
			
		}

		for (int j = 0; j < lq; j++)
		{
			if (j == lq - 1)
				current_transfer_length = residual;
			else
				current_transfer_length = 1024;

			niHAL->WriteReg(j * (1024) + (DDR_BAOFFSET * ch), DDR_BA1);

			if (memorymode == 0)
			{
				for (int i = 0; i < current_transfer_length; i += 8)
				{
					for (int k = 0; k < 8; k++)
						TEMPVECT[i + k] = (data[(j * (2048) + i*2 + (15 - (k*2)))] << 16) + (data[(j * (2048) + i*2 + (15 - (k * 2)-1))] << 0); //(j * (1024) + i + (7 - k)) + (((j * (1024) + i + (7 - k)) * 1) << 16);
																			//niHAL->WriteReg((j * (1024) + i +(7-k))*32 + (((j * (1024) + i + (7 - k)) * 32)<<16) ,0xFA000000+i+k);
				}
			}
			else
			{
				for (int i = 0; i < current_transfer_length; i += 8)
				{
					for (int k = 0; k < 8; k++)
						TEMPVECT[i + k] = data[(j * (1024) + i + (7 - k))] + ((time[(j * (1024) + i + (7 - k))]) << 16); //(j * (1024) + i + (7 - k)) + (((j * (1024) + i + (7 - k)) * 1) << 16);
																			//niHAL->WriteReg((j * (1024) + i +(7-k))*32 + (((j * (1024) + i + (7 - k)) * 32)<<16) ,0xFA000000+i+k);
				}
			}
			
			niHAL->WriteArray(TEMPVECT, DDR_MEMCACHE_BA, 1024);
			
			//DDR3 write strobe
			niHAL->WriteReg(1, DDR_CTRL_REG);
			niHAL->WriteReg(0, DDR_CTRL_REG);

			niHAL->WriteReg(memorymode1 + (memorymode2<<1), DDR_CTRL_MODE);
			//Sleep(1);
		}
	}

	//DDR3 idle mode
	niHAL->WriteReg(0, DDR_BA1);
	niHAL->WriteReg(DDR_BAOFFSET, DDR_BA2);

	if (length1 > 0)
		niHAL->WriteReg((length1 / divider1_base)-1, DDR_MEMLEN1);
	else
		niHAL->WriteReg(99, DDR_MEMLEN1);

	if (length2 > 0)
		niHAL->WriteReg((length2 / divider2_base)-1, DDR_MEMLEN2);
	else
		niHAL->WriteReg(99, DDR_MEMLEN2);

	//niHAL->WriteReg(scaler1, DDR_SCALER1);
	//niHAL->WriteReg(scaler2, DDR_SCALER2);



	//DDR3 start loop
	niHAL->WriteReg(8, DDR_CTRL_REG);

	//emulator in waveform mode
	//niHAL->WriteReg(1, DDR_WFENABLE);
	//niHAL->WriteReg(1, DDR_WFENABLE+1);
}

/*
	Reboot
		mode	0		standard mode
				1		bootloader
*/
#define RFA_ICAPDNA_REBOOTADDR			0x00000700
#define RFA_ICAPDNA_UNLOCK				0x00000701
#define RFA_ICAPDNA_nREBOOT				0x00000702
NI_RESULT DDE3_API::Reboot(unsigned int mode)
{
	NI_STATUS Status;
	UINT32 address;
	if (mode == 1)
	{
		if(niHAL->WriteReg(0x10000, RFA_ICAPDNA_REBOOTADDR ) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if(niHAL->WriteReg(0x400000, RFA_ICAPDNA_REBOOTADDR ) != NI_OK)
			return NI_ERROR;

	}

	if(niHAL->ReadReg(&address, RFA_ICAPDNA_REBOOTADDR ) != NI_OK)
		return NI_ERROR;


	if(niHAL->WriteReg(0xABBA5511, RFA_ICAPDNA_UNLOCK ) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(0xABBA5511, RFA_ICAPDNA_nREBOOT ) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}

/*
	CheckRunningMode
		mode	0		standard mode
				1		bootloader
*/

NI_RESULT DDE3_API::CheckRunningMode(unsigned int *mode)
{
	NI_STATUS Status;
	unsigned int HWOPTIONS;

	if (niHAL->ReadReg (&HWOPTIONS, 0xFFFE0006)!= NI_OK)
		return NI_ERROR ;

	if (HWOPTIONS >> 24 == 0x5B)
		*mode = 0;
	else
		if (HWOPTIONS >> 24 == 0x5A)
		*mode = 1;
		else
			*mode = 2;
	

	return NI_OK;
}


#define MCA_BASE_PREV						0x00000000
#define RFA_MCA_OSCILLO_BA					0xE0000000
#define RFA_MCA_SPECTRUM_BA					0xE0008000

#define RFA_MCA_PRECFG_MUXSEL_DATA_A		0x3
#define RFA_MCA_PRECFG_MUXSEL_DATA_B		0x4
#define RFA_MCA_PRECFG_TRIGGERMUX			0x5
#define RFA_MCA_PRECFG_COMPARATORVAL1		0x6
#define RFA_MCA_PRECFG_COMPARATORVAL2		0x7
#define RFA_MCA_PRECFG_POSTLEN				0x2
#define RFA_MCA_PRECFG_RESCALE				0x8
#define RFA_MCA_PRECFG_TRIGGER_ARM			0x1
#define RFA_MCA_PRECFG_TRIGGER_RESET		0x0
#define RFA_MCA_PRECFG_TRIGGER_AVAL			0xE00007FF



NI_RESULT DDE3_API::MCA_ReadPreview(int *data1,
									int *data2,
									int *Digital1,
									int *Digital2,
										int u1,
										int u2)
{



	NI_STATUS Status;
	unsigned int reg;
	unsigned int available;
	unsigned int armed;
	unsigned int triggerpos;
	unsigned int downloaddata[16384];
	int pre;
	int q = 0;
	int i;

	
	if (niHAL->ReadReg(&reg, RFA_MCA_PRECFG_TRIGGER_AVAL) != NI_OK)
		return NI_ERROR;

	available = (reg >> 12) & 0x01;
	armed = (reg >> 11) & 0x01;
	triggerpos = (reg)& 0x7FF;

	if (available == 1)
	{



		for (i = 0; i < 2048; i++)
		{
			data1[i] = 0;
			data2[i] = 0;
			Digital1[i] = 0;
			Digital2[i] = 0;

		}



		Status = niHAL->ReadArray(downloaddata, RFA_MCA_OSCILLO_BA, 2048);
		if (Status != NI_OK)
		{
			return NI_ERROR;
		}
		

		pre = triggerpos - (2047 - MCAPostLen);


		if (pre < 0)
			pre = 2047 + pre;

		if ((pre > 2048) || (pre < 0))
			return NI_ERROR;
		for (i = pre; i<2047; i++)
		{
			if ((downloaddata[i] >> 29) & 0x01)
				Digital1[q] = 1;
			else
				Digital1[q] = 0;

			if ((downloaddata[i] >> 28) & 0x01)
				Digital2[q] = 1;
			else
				Digital2[q] = 0;

			data1[q] = ((int)((downloaddata[i] >> 14) & 0x3FFF));
			if ((data1[q] > 8192) && (u1 == 1))
				data1[q] -= 16384;

			data2[q] = ((int)((downloaddata[i]) & 0x3FFF));
			if ((data2[q] > 8192) && (u2 == 1))
				data2[q] -= 16384;

			q = q + 1;
		}

		for (i = 0; i<pre; i++)
		{

			if ((downloaddata[i] >> 29) & 0x01)
				Digital1[q] = 1;
			else
				Digital1[q] = 0;

			if ((downloaddata[i] >> 28) & 0x01)
				Digital2[q] = 1;
			else
				Digital2[q] = 0;



			data1[q] = ((int)((downloaddata[i] >> 14) & 0x3FFF));
			if ((data1[q] > 8192) && (u1 == 1))
				data1[q] -= 16384;

			data2[q] = ((int)((downloaddata[i]) & 0x3FFF));
			if ((data2[q] > 8192) && (u2 == 1))
				data2[q] -= 16384;

			q = q + 1;
		}

		if (niHAL->WriteReg(1, MCA_BASE + MCA_BASE_PREV +  RFA_MCA_PRECFG_TRIGGER_RESET) != NI_OK)
			return NI_ERROR;

		return NI_OK;
	}
	else
	{
		return NI_NO_DATA_AVAILABLE;
	}

}



NI_RESULT DDE3_API::DPP_Preview_SettingsSET(
	int mux1,
	int mux2,
	int dmux1,
	int dmux2,
	int triggersource,
	int int_trigger_val,
	int postlen,
	int rescale)
{
	if (niHAL->WriteReg(mux1 + (dmux1 << 3), MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_MUXSEL_DATA_A) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(mux2 + (dmux2 << 3), MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_MUXSEL_DATA_B) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(triggersource, MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_TRIGGERMUX) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(int_trigger_val, MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_COMPARATORVAL1) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(int_trigger_val, MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_COMPARATORVAL2) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(postlen, MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_POSTLEN) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(rescale, MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_RESCALE) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(1, MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_TRIGGER_ARM) != NI_OK)
		return NI_ERROR;

	MCAPostLen = postlen;

	return NI_OK;

}

#define RFA_MCA_TRIGGER_BASE			0x00200000		//trigger base address
#define RFA_MCA_TRIGGER_THRESHOLD		0x00000000		//soglia del trigger sul trapezio
#define RFA_MCA_TRIGGER_COEFF_M			0x00000003		//coefficente di dconvoluzione del trigger
#define RFA_MCA_TRIGGER_DELAY_A			0x00000004		//ritardo fisso a 45
#define RFA_MCA_TRIGGER_DELAY_B			0x00000005		//ritardo per campionare il filtro energia nel centro del FT
#define RFA_MCA_TRIGGER_DELAY_PEAKING	0x00000006		//ritardo per il peaking time del trapezio
#define RFA_MCA_TRIGGER_DALY_FT			0x00000007		//ritardo per il ft time del trapezio
#define RFA_MCA_TRIGGER_DELAY_S			0x00000008		//media del trigger
#define RFA_MCA_TRIGGER_HOLDOFF			0x00000002		//inibizione del trigger
#define RFA_MCA_TRIGGER_RESET			0x00000001		//reset del trigger

//#define RFA_MCA_TRIGGER_MODE			0x0001000
//#define RFA_MCA_TRIGGER_AVERAGE			0x0001000

#define RFA_MCA_FILTER_BASE				0x00400000		//filtro base address
#define RFA_MCA_FILTER_MEAN_ENABLE		0x00000001		//abilita la media
#define RFA_MCA_FILTER_GAIN				0x00000002		//guadagno del trapezio
#define RFA_MCA_FILTER_DELAY_FT			0x00000003		//ritardo che coprende il ft per generare il filtro
#define RFA_MCA_FILTER_DELAY_PEAKING	0x00000004		//ritardo sul peaking time
#define RFA_MCA_FILTER_MEAN				0x00000005		//numero di campioni da mediare
#define RFA_MCA_PILEUP_REJECTOR_ENABLE	0x00000006		//pileup rejector enable
#define RFA_MCA_PEAKING_HOLDOFF			0x00000007		//holdoff delay
#define RFA_MCA_FILTER_MIN				0x00000008		//limite minimo di energia
#define RFA_MCA_FILTER_MAX				0x00000009		//limite massimo di energia
#define RFA_MCA_FILTER_TAU_M			0x0000000A		//coefficiente M
#define RFA_MCA_FILTER_RESET			0x00000001		//reset filtro

#define RFA_MCA_SATURATION_ENABLE		0x000000F0		//abilita la saturazione
#define RFA_MCA_SATURATION_HOLDOFF		0x000000F1		//durata dell'holdoff in saturazione
#define RFA_MCA_SATURATION_LEVEL		0x000000F2		//livello sopra il quale va in saturazione


#define RFA_MCA_BASELINE_BASE			0x00300000		//baseline baseaddress
#define RFA_MCA_BASELINE_ENABLE			0x00000004		//deve essere 1
#define RFA_MCA_BASELINE_LEN			0x00000002		//lunghezza media
#define RFA_MCA_BASELINE_SHIFT			0x00000000		//lunghezza shift
#define RFA_MCA_BASELINE_HOLDOFF		0x00000003		//holdoff di baseline
//#define RFA_MCA_BASELINE_INBPROG		0x0001000
#define RFA_MCA_BASELINE_ADDSUB			0x00000005		//sottrai somma
#define RFA_MCA_BASELINE_RESET			0x00000001		//reset baseline



#define RFA_MCA_DECIMATOR_SCALE			0x00010000



#define RFA_MCA_SPECTRUM_BASE			0x00500000		//spettro base address
#define RFA_MCA_SPECTRUM_MODE			0x00000000
#define RFA_MCA_SPECTRUM_LIMIT_MODE		0x00000003
#define RFA_MCA_SPECTRUM_LIMIT_VALUE	0x00000004
#define RFA_MCA_SPECTRUM_LIMIT_RESET	0x00000005
#define RFA_MCA_SPECTRUM_ENABLE			0x00000001
#define RFA_MCA_SPECTRUM_FLUSH			0x00000002


//#define RFA_MCA_RESET_BASE				0x0001000
//#define RFA_MCA_RESET_DETECTOR			0x0001000
//#define RFA_MCA_RESET_LEVEL				0x0001000
//#define RFA_MCA_RESET_LEN_MIN			0x0001000
//#define RFA_MCA_RESET_HOLDOFF			0x0001000


NI_RESULT DDE3_API::DPP_Reset(
	)
{

	if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_RESET) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_RESET) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_BASELINE_BASE + RFA_MCA_BASELINE_RESET) != NI_OK)
		return NI_ERROR;

	if (niHAL->WriteReg(0, MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_RESET) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_RESET) != NI_OK)
		return NI_ERROR;
	if (niHAL->WriteReg(0, MCA_BASE + RFA_MCA_BASELINE_BASE + RFA_MCA_BASELINE_RESET) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}


NI_RESULT DDE3_API::DPP_Configure(
	int TRIGGER_Threshold,
	double TRIGGER_peaking,
	double TRIGGER_holdoff,
	double FILTER_tau,
	double FILTER_peaking,
	double FILTER_ft,
	int FILTER_mean,
	double FILTER_delay,
	double FILTER_gain,
	int SATURATION_level,
	int SATURATION_holdoff,
	int PEAKING_holdoff,
	int BASELINE_mean,
	int BASELINE_holdoff,
	int DECIMATOR_scale,
	int EWIN_min,
	int EWIN_max,
	unsigned int reset_detector,
	unsigned int reset_level,
	double reset_holdoff
	)
{


	double f_to_ts = 2.0 / (_clockFREQUENCY) * 1000000;

	if (niHAL->WriteReg(TRIGGER_Threshold, MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_THRESHOLD) != NI_OK)								// TRIGGER
		return NI_ERROR;

	

	if (niHAL->WriteReg((int)((double)(TRIGGER_peaking)/f_to_ts), MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_DELAY_PEAKING) != NI_OK)	//TRIGGER
		return NI_ERROR;

	

	if (niHAL->WriteReg((int)((double)(TRIGGER_holdoff) / f_to_ts), MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_HOLDOFF) != NI_OK)		//TRIGGER
		return NI_ERROR;

	if (niHAL->WriteReg((int)((double)(TRIGGER_peaking+0.1) / f_to_ts), MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_DALY_FT) != NI_OK)	//TRIGGER												//TRIGGER
		return NI_ERROR;

	
	if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_DELAY_S) != NI_OK)													//TRIGGER
		return NI_ERROR;

	int sample_point = (int32_t)(((double)(FILTER_delay)+0.650) / f_to_ts);

	if (niHAL->WriteReg(45, MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_DELAY_A) != NI_OK)												//TRIGGER
		return NI_ERROR;


	if (niHAL->WriteReg(sample_point, MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_DELAY_B) != NI_OK)										//TRIGGER
		return NI_ERROR;

	

	if (niHAL->WriteReg(SATURATION_level, MCA_BASE + ADC_BASE + RFA_MCA_SATURATION_LEVEL) != NI_OK)												//SATURATION
		return NI_ERROR;

	if (niHAL->WriteReg(SATURATION_holdoff, MCA_BASE + ADC_BASE + RFA_MCA_SATURATION_HOLDOFF) != NI_OK)											//SATURATION
		return NI_ERROR;

	if (niHAL->WriteReg(1, MCA_BASE + ADC_BASE + RFA_MCA_SATURATION_ENABLE) != NI_OK)															//SATURATION
		return NI_ERROR;


	int decimator = (0x1 << DECIMATOR_scale) - 1;

	
	//if (niHAL->WriteReg(decimator, MCA_BASE +  RFA_MCA_DECIMATOR_SCALE) != NI_OK)																//DECIMATOR
	//	return NI_ERROR;


	if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_BASELINE_BASE + RFA_MCA_BASELINE_ENABLE) != NI_OK)												//BASELINE
		return NI_ERROR;


	//int inbprog = (unsigned int)(BASELINE_mean / (double)DECIMATOR_scale);

	//if (niHAL->WriteReg(inbprog, MCA_BASE + RFA_MCA_BASELINE_BASE + RFA_MCA_BASELINE_INBPROG) != NI_OK)											//BASELINE
	//	return NI_ERROR;

	
	
	
	
		if (niHAL->WriteReg(BASELINE_mean, MCA_BASE + RFA_MCA_BASELINE_BASE + RFA_MCA_BASELINE_LEN) != NI_OK)										//BASELINE
		return NI_ERROR;

	unsigned int shift = (unsigned int)(log((double)BASELINE_mean+1) / log(2.0) + 0.5);

	if (niHAL->WriteReg(shift, MCA_BASE + RFA_MCA_BASELINE_BASE + RFA_MCA_BASELINE_SHIFT) != NI_OK)												//BASELINE
		return NI_ERROR;

	if (niHAL->WriteReg(0, MCA_BASE + RFA_MCA_BASELINE_BASE + RFA_MCA_BASELINE_ADDSUB) != NI_OK)												//BASELINE
		return NI_ERROR;

	//FILTER_peaking * 2 + FILTER_ft +
	if (niHAL->WriteReg((int)((double)( BASELINE_holdoff) / f_to_ts), MCA_BASE + RFA_MCA_BASELINE_BASE + RFA_MCA_BASELINE_HOLDOFF) != NI_OK)
		return NI_ERROR;																														//BASELINE

	double M = (FILTER_tau / f_to_ts) / (DECIMATOR_scale);
	int M0 = (int)(M);
	int M1 = (int)(M * 256);

	if (niHAL->WriteReg(M0, MCA_BASE + RFA_MCA_TRIGGER_BASE + RFA_MCA_TRIGGER_COEFF_M) != NI_OK)												// FILTER
		return NI_ERROR;
	if (niHAL->WriteReg(M1, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_TAU_M) != NI_OK)												// FILTER
		return NI_ERROR;
	


	
	int trap_pktime = (int)(FILTER_peaking / f_to_ts);
	int trap_ft = (int)(FILTER_ft / f_to_ts);

	if (niHAL->WriteReg(trap_pktime, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_DELAY_PEAKING) != NI_OK)								// FILTER
		return NI_ERROR;


	if (niHAL->WriteReg(trap_ft + trap_pktime, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_DELAY_FT) != NI_OK)						// FILTER
		return NI_ERROR;
	
	float correction = (float)(FILTER_gain / ((FILTER_peaking / f_to_ts)* M));
	unsigned int *icorrection;
	icorrection = (unsigned int *)&correction;
	if (niHAL->WriteReg(*icorrection, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_GAIN) != NI_OK)											// FILTER
		return NI_ERROR;

	



	if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_PILEUP_REJECTOR_ENABLE) != NI_OK)										// FILTER
		return NI_ERROR;

	if (niHAL->WriteReg((int)((double)(PEAKING_holdoff) / f_to_ts), MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_PEAKING_HOLDOFF) != NI_OK)		// FILTER
		return NI_ERROR;


	int average_counter = (0x1 << (FILTER_mean * 2)) - 1;
	if (FILTER_mean > 0)
	{
		if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_MEAN_ENABLE) != NI_OK)										// FILTER
			return NI_ERROR;
		if (niHAL->WriteReg(average_counter, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_MEAN) != NI_OK)								// FILTER
			return NI_ERROR;
	}
	else
	{
		if (niHAL->WriteReg(0, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_MEAN_ENABLE) != NI_OK)										// FILTER
			return NI_ERROR;
		if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_MEAN) != NI_OK)												// FILTER
			return NI_ERROR;
	}
	

	if (niHAL->WriteReg(EWIN_min, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_MIN) != NI_OK)											// FILTER	
		return NI_ERROR;
	if (niHAL->WriteReg(EWIN_max, MCA_BASE + RFA_MCA_FILTER_BASE + RFA_MCA_FILTER_MAX) != NI_OK)											// FILTER
		return NI_ERROR;


	



	
	if (niHAL->WriteReg(0, MCA_BASE + RFA_MCA_SPECTRUM_BASE + RFA_MCA_SPECTRUM_MODE) != NI_OK)												// SPECTRUM
		return NI_ERROR;


	/*
	unsigned int reslenpulse = (unsigned int)(reset_holdoff / (double)DECIMATOR_scale);

	if (niHAL->WriteReg(reset_detector, MCA_BASE + RFA_MCA_RESET_BASE + RFA_MCA_RESET_DETECTOR) != NI_OK)									//RESET
		return NI_ERROR;

	if (niHAL->WriteReg(reset_level, MCA_BASE + RFA_MCA_RESET_BASE + RFA_MCA_RESET_LEVEL) != NI_OK)											//RESET
		return NI_ERROR;

	if (niHAL->WriteReg(15, MCA_BASE + RFA_MCA_RESET_BASE + RFA_MCA_RESET_LEN_MIN) != NI_OK)												//RESET
		return NI_ERROR;

	if (niHAL->WriteReg((int)((double)(reslenpulse) / f_to_ts), MCA_BASE + RFA_MCA_RESET_BASE + RFA_MCA_RESET_HOLDOFF) != NI_OK)			//RESET
		return NI_ERROR;

	*/

	return NI_OK;

}

NI_RESULT DDE3_API::DPP_SpectrumCleanup(
	)
{
	int i;
	if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_SPECTRUM_BASE + RFA_MCA_SPECTRUM_FLUSH) != NI_OK)										//SPECTRUM
		return NI_ERROR;

	if (niHAL->WriteReg(0, MCA_BASE + RFA_MCA_SPECTRUM_BASE + RFA_MCA_SPECTRUM_FLUSH) != NI_OK)										//SPECTRUM
		return NI_ERROR;

	unsigned int *downloadDATA = new unsigned int[8192];
	niHAL->ReadArray(downloadDATA, RFA_MCA_SPECTRUM_BA, 8192);
	niHAL->ReadArray(downloadDATA, RFA_MCA_SPECTRUM_BA, 8192);
	niHAL->ReadArray(downloadDATA, RFA_MCA_SPECTRUM_BA, 8192);
	delete[] downloadDATA;

	for (i = 0; i < 16384; i++)
		MCAspectrum[i] = 0;

	return NI_OK;
}


NI_RESULT DDE3_API::DPP_GetSpectrum(
	uint32_t *spectrum,
	int partial_NCumulative
	)
{

	NI_RESULT Status;
	unsigned int *downloadDATA = new unsigned int[8192];
	uint32_t t1, t2;
	
	int q = 0;
	int i;
	Status = niHAL->ReadArray(downloadDATA, RFA_MCA_SPECTRUM_BA, 8192);

	
	
	for (i = 0; i<8192; i++)
	{
		
		t1 = ((downloadDATA[i] >> 0) & 0xFFFF);
		t2 = ((downloadDATA[i] >> 16) & 0xFFFF);
		MCAspectrum[q] += t1; 
		MCAspectrum[q+1] += t2;

		if (partial_NCumulative == 0)
		{
			spectrum[q] = MCAspectrum[q];
			spectrum[q+1] = MCAspectrum[q+1];
		}
		else
		{
			spectrum[q] = t1;
			spectrum[q + 1] = t1;
		}
		q += 2;
	}
	
	
	delete[] downloadDATA;

	
	return Status;


}

NI_RESULT DDE3_API::DPP_SpectrumRun(
	int enable,
	int SPECTRUM_Limit_mode,
	double SPECTRUM_Limit_value
	)
{

	if (niHAL->WriteReg(1, MCA_BASE + RFA_MCA_SPECTRUM_BASE + RFA_MCA_SPECTRUM_LIMIT_RESET) != NI_OK)										//SPECTRUM
		return NI_ERROR;

	if (niHAL->WriteReg(0, MCA_BASE + RFA_MCA_SPECTRUM_BASE + RFA_MCA_SPECTRUM_LIMIT_RESET) != NI_OK)										//SPECTRUM
		return NI_ERROR;

	if (niHAL->WriteReg(enable , MCA_BASE + RFA_MCA_SPECTRUM_BASE + RFA_MCA_SPECTRUM_ENABLE) != NI_OK)										//SPECTRUM
		return NI_ERROR;

	if (niHAL->WriteReg(SPECTRUM_Limit_mode, MCA_BASE + RFA_MCA_SPECTRUM_BASE + RFA_MCA_SPECTRUM_LIMIT_MODE) != NI_OK)						//SPECTRUM
		return NI_ERROR;

	if (niHAL->WriteReg(SPECTRUM_Limit_value, MCA_BASE + RFA_MCA_SPECTRUM_BASE + RFA_MCA_SPECTRUM_LIMIT_VALUE) != NI_OK)					//SPECTRUM
		return NI_ERROR;

	return NI_OK;
}



NI_RESULT DDE3_API::DPP_GetStats(
	uint64_t  *running_time,
	uint64_t  *live_time,
	uint64_t  *dead_time,
	uint64_t *partial_time,
	uint64_t *partial_live_time,
	uint64_t *partial_dead_time,
	unsigned int *total_in_cnt,
	unsigned int *total_out_cnt,
	unsigned int *total_piledup_cnt,
	unsigned int *total_saturation_cnt,
	unsigned int *total_resets_cnt,
	unsigned int *partial_in_cnt,
	unsigned int *partial_out_cnt,
	unsigned int *partial_piledup_cnt,
	unsigned int *partial_saturation_cnt,
	unsigned int *partial_resets_cnt,
	unsigned int *status,
	unsigned int *limitcnt,
	unsigned int *timecnt
	)
{

	return NI_OK;
}


NI_RESULT DDE3_API::DPP_Preview_ARMTrigger()
{
	if (niHAL->WriteReg(1, MCA_BASE + MCA_BASE_PREV + RFA_MCA_PRECFG_TRIGGER_ARM) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}
/*
	Lock/Unlock Flash
		mode	0		locked
				1		unlocked
*/

NI_RESULT DDE3_API::FlashLock(unsigned int mode)
{
	NI_STATUS Status;

	return NI_OK;
}

/*
	Flash write page
		pagen			page number
		data			page content
		datalen			page len
*/

NI_RESULT DDE3_API::FLASHWritePage(unsigned int pagen, unsigned char *data, unsigned int len)
{
	NI_STATUS Status;

	return NI_OK;
}


/*
	Flash erase page
		pagen			page number
*/

NI_RESULT DDE3_API::FLASHErasePage(unsigned int pagen)
{
	NI_STATUS Status;

	return NI_OK;
}




/*
	Write Security EEPROM
*/

NI_RESULT DDE3_API::SECWriteWord(unsigned int channel, unsigned int address, unsigned int word)
{
	if(niHAL->WriteReg(0, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(1 << 24, RFA_ICAPDNA_EEPROM ) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(1, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;
	Sleep (10);
	if(niHAL->WriteReg((address << 16) + (2 << 24), RFA_ICAPDNA_EEPROM ) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(1, RFA_ICAPDNA_WF ) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_ICAPDNA_WF ) != NI_OK)
		return NI_ERROR;
	Sleep (10);

	if(niHAL->WriteReg(word + (address << 16) + (4 << 24), RFA_ICAPDNA_EEPROM ) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(1, RFA_ICAPDNA_WF ) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_ICAPDNA_WF ) != NI_OK)
		return NI_ERROR;
	Sleep (10);
	
	return NI_OK;
}



/*
	Read Security EEPROM
*/

NI_RESULT DDE3_API::SECReadWord(unsigned int channel, unsigned int address, unsigned int *word)
{
	unsigned int w;
	if(niHAL->WriteReg(0, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg((address << 16) + (8 << 24), RFA_ICAPDNA_EEPROM ) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(1, RFA_ICAPDNA_WF ) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_ICAPDNA_WF ) != NI_OK)
		return NI_ERROR;

	Sleep (10);

	if(niHAL->ReadReg(&w, RFA_ICAPDNA_READEEPROM ) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;

	if(niHAL->WriteReg(1, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;
	if(niHAL->WriteReg(0, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;

	*word = w & 0xFFFF;
	
	return NI_OK;
}


NI_RESULT DDE3_API::WriteCalibrarionFlash(unsigned int channel, double offset, double gain, double chctv)
{
	unsigned int TEMP;

	float foffset;
	float fgain;
	float fchctv;

	 foffset = (float) offset;
	 fgain = (float)gain;
	 fchctv = (float) chctv;


	memcpy(&TEMP, &fgain, 4);
	if(SECWriteWord(0, 20 + 0 + 6*channel, (TEMP>>0) & 0xFFFF))
		return NI_ERROR;
	Sleep(10);
	if(SECWriteWord(0, 20 + 1 + 6*channel, (TEMP>>16) & 0xFFFF))
		return NI_ERROR;
	Sleep(10);

	memcpy(&TEMP, &foffset, 4);
	if(SECWriteWord(0, 20 + 2 + 6*channel, (TEMP>>0) & 0xFFFF))
		return NI_ERROR;
	Sleep(10);
	if (SECWriteWord(0, 20 + 3 + 6*channel, (TEMP>>16) & 0xFFFF))
		return NI_ERROR;

	Sleep(50);
	memcpy(&TEMP, &fchctv, 4);
	if (SECWriteWord(0, 20 + 4 + 6*channel, (TEMP>>0) & 0xFFFF))
		return NI_ERROR;
	Sleep(10);
	if (SECWriteWord(0, 20 + 5 + 6*channel, (TEMP>>16) & 0xFFFF))
		return NI_ERROR;
	Sleep(50);

	return NI_OK;
}



NI_RESULT DDE3_API::ReadCalibrarionFlash(unsigned int channel, double *offset, double *gain, double *chctv)
{
	unsigned int TEMP;
	unsigned int TEMP1;
	unsigned int TEMP2;

	float foffset;
	float fgain;
	float fchctv;

	SECReadWord(0, 20 + 0 + 6 * channel, &TEMP1);
		SECReadWord(0, 20 + 0 + 6 * channel, &TEMP1);
		SECReadWord(0, 20 + 0 + 6 * channel, &TEMP1);

	if (SECReadWord(0, 20 + 0 + 6 * channel, &TEMP1))
		return NI_ERROR;
	if (SECReadWord(0, 20 + 1 + 6 * channel, &TEMP2))
		return NI_ERROR;
	TEMP = TEMP1 + (TEMP2 << 16);
	memcpy(&fgain, &TEMP , 4);

	if (SECReadWord(0, 20 + 2 + 6 * channel, &TEMP1))
		return NI_ERROR;
	if (SECReadWord(0, 20 + 3 + 6 * channel, &TEMP2))
		return NI_ERROR;
	TEMP = TEMP1 + (TEMP2 << 16);
	memcpy(&foffset, &TEMP, 4);

	if (SECReadWord(0, 20 + 4 + 6 * channel, &TEMP1))
		return NI_ERROR;
	if (SECReadWord(0, 20 + 5 + 6 * channel, &TEMP2))
		return NI_ERROR;
	TEMP = TEMP1 + (TEMP2 << 16);
	memcpy(&fchctv, &TEMP, 4);

	*offset = foffset;
	*gain = fgain;
	*chctv = fchctv;


	return NI_OK;
}



NI_RESULT DDE3_API::SECReadUIDSN(UINT64 *UID, unsigned int *SN)
{
	unsigned int TTT[20];
	unsigned int v;
	SECReadWord(0,16,&v);

	if (niHAL->ReadArray(TTT, 0xFEFFFF00,18) != NI_OK)
		return NI_ERROR;

	*UID = (((UINT64)TTT[5]) << 32) + ((UINT64)TTT[4]); 
	*SN = v;
	return NI_OK;
}



NI_RESULT DDE3_API::SECWriteSN(unsigned int SN)
{
	return SECWriteWord(0,16,SN);
	
}



NI_RESULT DDE3_API::SECWritekey(int *key, int length)
{
	int i;
	if (length > 16)
		return NI_ERROR;
	for (i=0;i<length-1;i+=2)
	{
		SECWriteWord(0,i,key[i+1]);
		SECWriteWord(0,i+1,key[i]);
		Sleep(5);
	}
	return NI_OK;
}


NI_RESULT DDE3_API::SECWriteOPTIONSLIC(unsigned int KEY, unsigned int register_NOTunregister, unsigned int id)
{
	unsigned int v;
	if (KEY != 0x462DEA90)
		return NI_ERROR;

	SECReadWord(0, 17, &v);
	if (register_NOTunregister == 1)
	{
		v = v | (1 << id);
		SECWriteWord(0, 17, v);
	}
	else
	{
		v = v & !(1 << id);
	}
	
	return NI_OK;

}


NI_RESULT DDE3_API::SECReadActivationStatus(unsigned int *active, unsigned int *trial_counter, unsigned int *trial_expired)
{
	unsigned int TTT[20];
	
	if (niHAL->ReadArray(TTT, 0xFEFFFF00,18) != NI_OK)
		return NI_ERROR;

	*active = TTT[3] & 0x01;
	*trial_counter = (TTT[3] >> 3) & 0xFFF;
	*trial_expired = !( (TTT[3] >> 2) & 0x01);
	return NI_OK;
}

NI_RESULT DDE3_API::GetLiveData(unsigned int channel,
								double *run_time,
								double *sat_time,
								double *busy_time,
								double *real_time,
								UINT32 *cnt_event,
								UINT32 *sat_event,
								UINT32 *lost_event,
								double *measured_rate,
								UINT32 *real_event,
								UINT32 *busyflag,
								UINT32 *satflag,
								UINT32 *eflag)
{
	int i;
	//unsigned int * buffer  = new unsigned int [5000];
	unsigned int  buffer[32];
	unsigned int baseaddress;
	
	if (channel == 0)
		baseaddress = LIVE_BASEADDRESS_A;
		
	if (channel == 1)
		baseaddress = LIVE_BASEADDRESS_B;

	if (niHAL->ReadArray(buffer, baseaddress, 32) != NI_OK)
		return NI_ERROR;


	*run_time =  (double)( (((UINT64)buffer[0]) << 0) + (((UINT64)buffer[1]) << 32) ) / (_clockFREQUENCY);
	*sat_time =  (double)( (((UINT64)buffer[3]) << 0) + (((UINT64)buffer[2]) << 32) ) / (_clockFREQUENCY);
	*busy_time = (double) ( (((UINT64)buffer[5]) << 0) + (((UINT64)buffer[4]) << 32) ) / (_clockFREQUENCY);
	*real_time = (double) ( (((UINT64)buffer[7]) << 0) + (((UINT64)buffer[6]) << 32) ) / (_clockFREQUENCY);
	*cnt_event =  (UINT32)buffer[8];
	*sat_event =  (UINT32)buffer[9];
	*lost_event =  (UINT32)buffer[10];
	*measured_rate =  (double)buffer[11]  / (250000000/_clockFREQUENCY);
	*real_event =  (UINT32)buffer[12];
	*busyflag =  ((UINT32)buffer[13]>>2) & 0x01;
	*satflag =  ((UINT32)buffer[13]>>1) & 0x01;
	*eflag =  ((UINT32)buffer[13]>>0) & 0x01;

	return NI_OK;

}


NI_RESULT DDE3_API::ControlLFSR(	 bool allchannel,
									 unsigned int channel,
									 tLFSR_CONTROLPIN source,
									 unsigned int runstop,
									 bool perform_reset)
{
	unsigned int address;
	
	if (allchannel)
		address = LFSR_ALLCHANNEL;
	else
		address = LFSR_SINGLECHANNEL + __CHOFFSET * channel;

	if (perform_reset)
	{
		if(niHAL->WriteReg((unsigned int)source + (runstop << 2) + (1 << 3), address) != NI_OK)
			return NI_ERROR;
	}
	else
	{
		if(niHAL->WriteReg((unsigned int)source + (runstop << 2) , address) != NI_OK)
			return NI_ERROR;
	}
	if(niHAL->WriteReg((unsigned int)source + (runstop << 2) + (0 << 3), address) != NI_OK)
		return NI_ERROR;

	return NI_OK;
}




NI_RESULT DDE3_API::SMClockControl(unsigned int inclock, unsigned int outclock)
{
	int word = 0xAB30 + (inclock << 1) + outclock;
	return SECWriteWord(0, 17, word);
	
}



NI_RESULT DDE3_API::SMReadClockControl(unsigned int *inclock, unsigned int *outclock)
{
	unsigned int word;
	SECReadWord(0, 17, &word);
	if ((word >> 4) == 0xAB3)
	{
		*inclock = (word >> 1 & 0x1);
		*outclock = word & 0x1;
		return NI_OK;
	}
	else
	{
		*inclock = 0;
		*outclock = 0;
		return NI_ERROR;
	}


}



