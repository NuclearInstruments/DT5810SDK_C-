#include <windows.h>
#pragma once
typedef ULONG	NI_RESULT;

#define		NI_OK									0x00000000
#define		NI_ERROR_GENERIC						0x00000001
#define		NI_ERROR_INTERFACE						0x00000002
#define		NI_ERROR_FPGA							0x00000003
#define		NI_ERROR_TRANSFER_MAX_LENGTH			0x00000004
#define		NI_ERROR_NOTCONNECTED					0x00000005
#define		NI_NO_DATA_AVAILABLE					0x00000006
#define		NI_TOO_MANY_DEVICES_CONNECTED			0x00000007
#define		NI_INVALID_HANDLE						0x00000008
#define		NI_INVALID_HARDWARE						0x00000009
#define		NI_PARAM_OUT_OF_RANGE					0x0000000A
#define		NI_PARAM_INVALID						0x0000000B


typedef ULONG	NI_STATUS;
#define		NI_ONLINE				0x00000000
#define		NI_ERROR				0x00000001
#define		NI_OFFLINE				0x00000002
#define		NI_UCBOOTLOADER			0x00000003
#define		NI_FPGABOOTLOADER		0x00000004


#define		NI_CONNECTED			0x00000001
#define		NI_NOTCONNECTED			0x00000000


#define		GET_VERSION		1
#define		ERASE_FLASH		2
#define		UPLOAD_FW		3
#define		READ_CRC	    4
#define		RUN_APPLICATION	5
#define		ERASE_CONFIG	0x10
#define		WRITE_CONFIG	0x11
#define		READ_CONFIG		0x12
#define		REBOOT_BOARD	0xFF


#define		NUCLEAR_INSTRUMENTS_PIC32_BOOTLOADER	0x01
#define		NUCLEAR_INSTRUMENTS_DPP_ETHERNET		0x02
#define		NUCLEAR_INSTRUMENTS_EMULATOR_ETHERNET	0x03

typedef struct _tUSBDevice
{
	unsigned int id;
	unsigned int symbSN;
	unsigned int trial;
	char SN[64];
	char DESC[64];
	unsigned int vid;
	unsigned int pid;
	unsigned int release;
	
} tUSBDevice;

typedef struct _tETHDevice
{
	unsigned char HW_CODE[4];
	unsigned char pic_version_high;
	unsigned char pic_version_low;
	char serial_number[8];
	unsigned char ip_address[4];
	unsigned char mac_address[6];
	unsigned int UDP_port;
	unsigned int TCP_port;
	char SIPAddress[16];
} tETHDevice;


typedef struct _tETHDevices
{
	int Count;
	tETHDevice Devices[32];
} tETHDevices;

typedef struct _tBOARDConfig
{
	unsigned char IPAddress[4];
	unsigned char NETMask[4];
	unsigned char IPGateway[4];
	unsigned char PriDNS[4];
	unsigned char MAC[6];
	unsigned int UDPPort;
	unsigned int HW_Code_Number;
	unsigned char CompanyInfo[256];
	unsigned char FriendlyName[128];
	unsigned int HW_Revision;
	unsigned int HW_Bootloader_Delay;
	unsigned char HW_SerialNumber[8];
	unsigned int TCPPort;
} tBOARDConfig;



typedef struct {
	char						ModelName[12];
	unsigned int				Model;
	unsigned int				Channels;
	unsigned int				FormFactor;
	unsigned int				FamilyCode;
	char						ROC_FirmwareRel[20];
	char						AMC_FirmwareRel[40];
	unsigned int				SerialNumber;
	unsigned int				PCB_Revision;
	unsigned int				ADC_NBits;
	unsigned int				HVChannels;
	unsigned int				USBOption;
	unsigned int				ETHOption;
	unsigned int				WIFIOption;
	unsigned int				BTOption;
	unsigned int				POEOption;
} tNI_BoardInfo;

#define	NI_DPPCHANNEL_DPP2580_STANDARD_ANALOG_INPUT				0x0001;

typedef struct _tChannel
{
	unsigned int TYPE;
	
} tChannel;


typedef enum{
	CTRL_ON = 1,
	CTRL_OFF = 0
} t_on_off;

typedef enum{
	LFSR_ENERGY=0,
	LFSR_TIMEBASE=1,
	LFSR_MULTISHAPE=2, 
	LFSR_NOISE_GAUSS=3,
	LFSR_NOISE_RN=4,
	LFSR_NOISE_RW=5,
	LFSR_NOISE_FLIKR=6
} t_LFSR_ID;

typedef enum{
	LFSR_REPROGRAM=0,
	LFSR_RESET=1,
	LFSR_START=2, 
	LFSR_PAUSE=3,
	LFSR_RESTART=3
} t_LFSR_OPERATION;



typedef enum
{
	IN_OFF = 0,
	TRIGGER_GATE_0 = 1,
	TRIGGER_GATE_1 = 2,
	GATE_0 =3,
	GATE_1 =4,
	TRIGGER_IN_0 = 5,
	TRIGGER_IN_1 = 6,
	RUNSTOP = 7,
	STEPOVER = 8,
	RESET_LFSR = 9,
	BASELINERESET = 10
}DIO_IN_MODE ;

typedef enum
{
	OUT_OFF = 0,
	RUN_0 = 1,
	RUN_1 = 2,
	TRIGGER_OUT_0 = 3,
	TRIGGER_OUT_1 = 4,
	PULSE_0 = 5,
	PULSE_1 = 6,
	BUSY_0 = 7,
	BUSY_1 = 8,
	SATURATION_0 = 9,
	SATURATION_1 = 10,
	LOOP_0 = 11,
	LOOP_1 = 12
	
}DIO_OUT_MODE ;


typedef enum
{
	RUNGATE_FREERUNNING = 0,
	RUNGATE_RUNTIME = 1,
	RUNGATE_REALTIME = 2,
	RUNGATE_CNTEVENT =3,
	RUNGATE_REALEVENT =4,
	RUNGATE_STOP = 5
}tRUNCONTROL_MODE ;


typedef enum
{
	LFSR_USB = 0,
	LFSR_RUN_EXTERNAL = 1,
	LFSR_RESET_EXTERNAL = 2,
	LFSR_ALL_EXTERNAL = 3	
}tLFSR_CONTROLPIN ;

#define NIDPP_cfg_OSCILLOSCOPE_READOUTCOMPLETED					0x10000
#define NIDPP_cfg_OSCILLOSCOPE_ARMTRIGGER						0x10001
#define NIDPP_cfg_OSCILLOSCOPE_POSTLEN							0x10002
#define NIDPP_cfg_OSCILLOSCOPE_MUX1_SELECT						0x10003
#define NIDPP_cfg_OSCILLOSCOPE_MUX2_SELECT						0x10004
#define NIDPP_cfg_OSCILLOSCOPE_TRIGGERSOURCE					0x10005
#define NIDPP_cfg_OSCILLOSCOPE_INTTRIGGER1_VAL					0x10006
#define NIDPP_cfg_OSCILLOSCOPE_INTTRIGGER2_VAL					0x10007
#define NIDPP_cfg_OSCILLOSCOPE_DECIMATOR						0x10008
#define NIDPP_cfg_OSCILLOSCOPE_RESET							0x1000B
#define NIDPP_cfg_OSCILLOSCOPE_OUTCPS							0x1000C
#define NIDPP_cfg_OSCILLOSCOPE_INCPS							0x1000D


#define		NI_CONNECT_USB									0x00000000
#define		NI_CONNECT_ETH									0x00000001
#define		NI_CONNECT_BT									0x00000002
