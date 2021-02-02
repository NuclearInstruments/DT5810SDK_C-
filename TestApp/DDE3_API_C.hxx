#define MAX_NUMBER_OF_DEVICE 100

#include "NIErrorCode.h"

#include <stdint.h>

#ifdef DDE3_API_EXPORTS
#define DDE3_API_API extern "C" __declspec(dllexport)
#else
#define DDE3_API_API extern "C" __declspec(dllimport)
#endif

/*
	FUNCTION: NI_Startup()
	To be call before any other operations

	INPUT: 

	OUTPUT:
	NI_RESULT			->				Success/Error code
	
*/
DDE3_API_API NI_RESULT NI_Startup();

/*
FUNCTION: NI_AttachNewDevice()
Open connection to the device

INPUT:
	CONNECTION_MODE		->				always 0
	*IPAddress_or_SN	->	[string]	Serial number of the device to be connected
	TCPPort				->				always 0
	UDPPort				->				always 0
	*handle				->	[int*]		pointer to the returned handle

OUTPUT:
	NI_RESULT			->				Success/Error code
*/
DDE3_API_API NI_RESULT NI_AttachNewDevice(int CONNECTION_MODE, 
									  char *IPAddress_or_SN, 
									  int TCPPort, 
									  int UDPPort, 
									  int *handle);

/*
FUNCTION: NI_DeleteDevice()
Disconnect device

INPUT:
	handle				->	[int]		Handle to the connected device
OUTPUT:
	NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT NI_DeleteDevice(int handle);


/*
FUNCTION: NI_ConnectionStatus()
Check connection status

INPUT:
status				->	[int*]		Connection status
									0: not connected/invalid firmware
									1: connected
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel
OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API int NI_ConnectionStatus( int *status,
								  int handle, 
								  int channel	);


/*
FUNCTION: NI_USBEnumerate()
Enumerate connected devices

INPUT:
pvArg1				->	[tUSBDevice*]		List of detected device
numDevs				->	[uint*]				Number of detected emulatod
OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT NI_USBEnumerate( tUSBDevice *pvArg1, unsigned int *numDevs	);

DDE3_API_API NI_RESULT NI_ETHEnumerate(tETHDevices *pvArg1);

DDE3_API_API NI_RESULT ETHGetSerialNumber(short int port, char *IPAddress, char *SN);

DDE3_API_API NI_RESULT NI_DHA_WriteReg(UINT32 value, 
									  UINT32 address,
									  int handle, 
									  int channel	);

DDE3_API_API NI_RESULT NI_DHA_ReadReg(UINT32 *value, 
									  UINT32 address,
									  int handle, 
									  int channel	);

DDE3_API_API NI_RESULT NI_DHA_WriteArray(UINT32 *value, 
									  UINT32 address,
									  UINT32 length,
									  int handle, 
									  int channel	);

DDE3_API_API NI_RESULT NI_DHA_ReadArray(UINT32 *value, 
									  UINT32 address,
									  UINT32 length,
									  int handle, 
									  int channel	);


/*
FUNCTION: EnableChanel
Enable channel output

INPUT:
ENABLE				->	[uint]		Enable/Disable channel
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel
OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT EnableChanel(UINT32 ENABLE,
									  int handle, 
									  int channel	);

/*
FUNCTION: ConfigureEnergy
Configure energy generation

INPUT:
MODE				->	[uint]		Energy generation mode
									0	fixed energy
									1	spectrum
ENERGY				->	[uint]		0 .. 2^15 fixed energy amplitude
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel
OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ConfigureEnergy(	
										UINT32 MODE,
										UINT32 ENERGY,
										int handle, 
										int channel	);

/*
FUNCTION: ProgramSpectrum
Write spectrum

INPUT:
spectrum			->	[*uint]		Array with spectrum data. 
spectrumLen			->	[uint]		Number of samples in spectrum array. MAX 2^16
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel
OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ProgramSpectrum(	
										UINT32 *spectrum,
										UINT32 spectrumLen,
										int handle, 
										int channel	);


/*
FUNCTION: ConfigureTimebase
Select time base generator

INPUT:
MODE				->	[uint]		Timebase mode
									0		fixed rate
									1		poisson
									2		external trigger

RATE				->	[double]	Output rate in Hz
DeatTime			->	[uint]		Deadtime in ns in non paralizzable machine is selected
Parallelizable		->	[bool]		Parallelizable/non-Parallelizable selector
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel
OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ConfigureTimebase(	
											UINT32 MODE, 
											double RATE, 
											UINT64 DeatTime, 
											bool Parallelizable,
											int handle, 
											int channel	);

/*
FUNCTION: ConfigureDIO
Configure digital output trigger

INPUT:
OUTTRIGGEREN		->	[uint]		Enable/Disable output trigger
OUTTRIGGERLEN		->	[uint]		Length in ns of output trigger signal length
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel
OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ConfigureDIO(	
										UINT32 OUTTRIGGEREN,
										UINT32 OUTTRIGGERLEN,
										int handle, 
										int channel	);


/*
FUNCTION: ConfigureDRC
Configre Digital RC exponential generator

INPUT:
RISETIME			->	[uint]		Risetime in ns
FALLTIME			->	[uint]		Falltime in ns
ENABLE				->	[int]		Enable Digital RC
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel
OUTPUT:
NI_RESULT			->				Success/Error code

*/

DDE3_API_API NI_RESULT ConfigureDRC(	
										UINT32 RISETIME,
										UINT32 FALLTIME,
										UINT32 ENABLE,
										int handle, 
										int channel	);

/*
FUNCTION: ConfigureShapeGenerator
Configre Memory based pulse generator

INPUT:
shape						->	[*uint]		Array of 4096 elements with pulse shape
shape_id					->	[uint]		Memory where to write the shape. 
											You must write shape in all the 16 memories
											Different shape in different memory can be programmed
											for multishape
shape_length				->	[int]		Length in sample of the shape in the memory
multishape_id				->	[int]		In in the multishape generator (0..3)
											Multishape memory id must be placed ordered in the
											16 memories
rising_falling_crosspoint	->	[int]		interpolator rise time ending point
interpolator_factor_rising	->	[int]		interpolation ratio on rising 
interpolator_factor_falling	->	[int]		interpolation ratio on falling tail
reconfigure_shape			->	[int]		write data on memory
enable_shape				->	[int]		enable in multishape array

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel
OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ConfigureShapeGenerator(	INT32 *shape,
												UINT32 shape_id,
												UINT32 shape_length, 
												int multishape_id,
												unsigned int rising_falling_crosspoint,
												unsigned int interpolator_factor_rising,
												unsigned int interpolator_factor_falling,
												bool reconfigure_shape,
												bool enable_shape,
												int handle,
												int channel);

/*
FUNCTION: ConfigureShapeGenerator
Configre multishape probability

INPUT:
prob2				->	[double]	Probability for multishape_id = 1
prob3				->	[double]	Probability for multishape_id = 2
prob4				->	[double]	Probability for multishape_id = 3

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

:: the probability for multishape_id =0 prob1 = 1-prob2-prob3-prob4
:: the sum prob1+prob2+prob3+prob4 must be 1

OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ConfigureMultishape(		double prob2,
												double prob3,
												double prob4,
												bool enable,
												int handle,
												int channel);

/*
FUNCTION: ConfigureBaselineDrift
Configre baseline drift generator

INPUT:
baseline						->	[int]		Baseline keypoint array
baseline_length					->	[uint]		Length of the baseline keypoint array
interpolator_factor_slow		->	[uint]		Interpolation factor for slow keypoint
interpolator_factor_fast		->	[uint]		Interpolation factor for slow keypoint
reconfigure_drift				->	[bool]		Reconfigure baseline point memory
enable_shape					->	[bool]		Enable baseline drift
reset							->	[bool]		Reset baseline drift

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ConfigureBaselineDrift(	int *baseline,
												unsigned int baseline_length, 
												unsigned int interpolator_factor_slow,
												unsigned int interpolator_factor_fast,
												bool reconfigure_drift,
												bool enable_shape,
												bool reset,
												int handle,
												int channel);



/*
FUNCTION: ConfigureNOISE
Configre noise generator

INPUT:
RANDM				->	[uint]		Magnitude of random noise (max 2^16)
GAUSSM				->	[uint]		Magnitude of white gauss noise (max 2^16)
DRIFTM				->	[uint]		Magnitude of random walk (max 2^16)
FLIKERM				->	[uint]		Magnitude of fliker noise (max 2^16)
FLIKERCorner		->	[uint]		Fliker noise corner point 
									0	:	100Hz
									1	:	500Hz
									2	:	1KHz
									3	:	5KHz
									4	:	10KHz
									5	:	50KHz
									6	:	100KHz
									7	:	500KHz
									8	:	1MHz

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ConfigureNOISE(	
										UINT32 RANDM,
										UINT32 GAUSSM,
										UINT32 DRIFTM,
										UINT32 FLIKERM,
										UINT32 FLIKERCorner,
										int handle, 
										int channel	);


/*
FUNCTION: ConfigureGeneral
Configure channel general parameters

INPUT:
GAIN				->	[double]	analog output gain
OFFSET				->	[uint]		analog offset (-2^15 .. 2^15)
INVERT				->	[uint]		0: positive polarity
									1: invert signal
OUTFILTER			->	[uint]		analog output filter
									0: filter disable
									1: filter enable
ANALOGSEL			->	[uint]		Analog output mode
									0: high performance
									1: high range

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ConfigureGeneral(	
										double GAIN,
										INT32 OFFSET,
										UINT32 INVERT,
										UINT32 OUTFILTER,
										UINT32 ANALOGSEL,
										int handle, 
										int channel	);

/*
FUNCTION: ChannelsToVoltage
Convert LSB in volt using internal instrument calibration

INPUT:
CHANNELS			->	[int]		LSB input

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
voltage				->	[double]	calibrated voltage for the specific channel

*/
DDE3_API_API double ChannelsToVoltage(	
										INT32 CHANNELS,
										int handle, 
										int channel	);

/*
FUNCTION: VoltageToChannels
Convert Volt in LSB using internal instrument calibration

INPUT:
VOLTAGE			->	[int]		Voltage input

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
CHANNELS				->	[double]	calibrated LSB for the specific channel

*/
DDE3_API_API double VoltageToChannels(	
										double VOLTAGE,
										int handle, 
										int channel	);



/*
FUNCTION: GetStat
Read emulation statistics

INPUT:
CPS					->	[uint]		count rate
LIVE				->	[uint]		live time in %
CCOUNTER			->	[uint]		total generated events
OVERFLOW			->	[uint]		events in analog overflow

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT GetStat(	
										UINT32 *CPS,
										UINT32 *LIVE,
										UINT64 *CCOUNTER,
										UINT32 *OVERFLOW,
										int handle, 
										int channel	);

DDE3_API_API NI_RESULT ResetCCounter(	
										int handle, 
										int channel	);

/*
FUNCTION: GetSignalLoopback
Read loopback emulated signal for monitor

INPUT:
DATA				->	[uint]		preallocated array of 16K samples
LEN					->	[uint]		number of valid point in DATA

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT GetSignalLoopback(	
										INT32 *DATA,
										UINT32 *LEN,
										int handle, 
										int channel	);


/*
FUNCTION: GetSpectrumLoopback
Read loopback emulated spectrum for monitor

INPUT:
DATA				->	[uint]		preallocated array of 16K samples
LEN					->	[uint]		number of valid point in DATA

handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT GetSpectrumLoopback(	
										UINT32  *DATA,
										UINT32 *LEN,
										int handle, 
										int channel	);

/*
FUNCTION: ResetSpectrum
Reset spectrum monitor

INPUT:
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
NI_RESULT			->				Success/Error code

*/
DDE3_API_API NI_RESULT ResetSpectrum(	
										int handle, 
										int channel	);

DDE3_API_API NI_RESULT ActiveteGetUID(	
										unsigned int  *key1,
										unsigned int  *key2,
										int handle);


DDE3_API_API NI_RESULT ActiveteRegister(	
										unsigned char  *DATA,
										int handle);

DDE3_API_API NI_RESULT IsActivated(	
										unsigned int  *DATA,
										int handle);

DDE3_API_API NI_RESULT HardwareInfo(	
										UINT32  *HWREV,
										UINT32  *FWREV,
										UINT32  *UCREV,
										UINT32  *HWOPTIONS,
										UINT32  *DEVICEMODEL,
										int handle);

DDE3_API_API NI_RESULT Reboot(			UINT32 MODE,
										int handle);

DDE3_API_API NI_RESULT CheckRunningMode(
										UINT32 *MODE,
										int handle);

DDE3_API_API NI_RESULT FlashLock(			
										UINT32 MODE,
										int handle);

DDE3_API_API NI_RESULT FLASHWritePage(			
										UINT32 PAGEN,
										UCHAR *DATA,
										UINT32 LEN,
										int handle);

DDE3_API_API NI_RESULT FLASHErasePage(			
										UINT32 PAGEN,
										int handle);


DDE3_API_API NI_RESULT SECWriteWord(
										UINT32 address,
										UINT32 word,
										int handle, 
										int channel	);

DDE3_API_API NI_RESULT SECReadWord(
										UINT32 address,
										UINT32 *word,
										int handle, 
										int channel	);

DDE3_API_API NI_RESULT WriteCalibrarionFlash(
												double offset, 
												double gain, 
												double chctv,
												int handle,
												unsigned int channel);

DDE3_API_API NI_RESULT ReadCalibrarionFlash(
	double *offset,
	double *gain,
	double *chctv,
	int handle,
	unsigned int channel);


DDE3_API_API NI_RESULT ConfigureLFSR(	UINT64 SEED, 
										t_LFSR_ID LFSR_ID, 
										t_LFSR_OPERATION Operation,
										int handle,
										unsigned int channel);



DDE3_API_API NI_RESULT UpdateDisplayStatus(int timemode, 
										   int rate, 
										   int ratep, 
										   char *time_str, 
										   int energy_mode, 
										   int energy, 
										   char *energy_str, 
										   char *shape_str, 
										   int live,
										   int funcgen, 
										   int funcgen_mvolt, 
										   int funcgen_freq,
										   char *funcgen_str,
										   int handle,
										   unsigned int channel);

/*
FUNCTION: SetAnalogDatapath
Configure analog datapath for DT5810B

INPUT:
filter				->	[int]		Enable analog filter
hvlv				->	[int]		HV/LV output datapath
handle				->	[int]		Handle to the connected device
channel				->	[int]		emulator channel

OUTPUT:
NI_RESULT			->				Success/Error code

*/

DDE3_API_API NI_RESULT SetAnalogDatapath    (unsigned int filter, 
											 unsigned int hvlv,
											 int handle,
										     unsigned int channel);



DDE3_API_API NI_RESULT SetInputAnalogDatapath(	unsigned int input_imp,
												unsigned int input_div,
												unsigned int input_mode,
												unsigned int input_scale,
												unsigned int input_gain,
												double offset,
												int handle,
												unsigned int channel);



DDE3_API_API NI_RESULT  SetInputAnalogMix(		double gain_a,
												int offset_a,
												double gain_b,
												int offset_b,
												unsigned int enable_a,
												unsigned int enable_b,
												unsigned int inv_a,
												unsigned int inv_b,
												int handle,
												unsigned int channel);



DDE3_API_API NI_RESULT SetDIO(		unsigned int Dio_In_A, 
									unsigned int Dio_In_B, 
									unsigned int Dio_Out_A,
									unsigned int Dio_Out_B,
									unsigned int DIO_Out_PulseLen,
									double delayA,
									double delayB,
									int handle
							);



DDE3_API_API NI_RESULT  DelayAndCorrelationControl( unsigned int correlation_mode,
													unsigned int enableCchannel,
													double delay,
													int handle);

DDE3_API_API NI_RESULT  EnergyMux(	 unsigned int mode,
									 int handle,
									 unsigned int channel);

DDE3_API_API NI_RESULT  TimebaseMux( unsigned int mode,
									 int handle,
									 unsigned int channel);




DDE3_API_API NI_RESULT  GetLiveData(
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
								UINT32 *eflag,
								int handle,
								unsigned int channel);

DDE3_API_API NI_RESULT  SetRunControlMode(	unsigned int rmode, 
											double limitvalue,
											int handle,
											unsigned int channel);
DDE3_API_API NI_RESULT  RunControlResetStat(int handle, unsigned int channel);
DDE3_API_API NI_RESULT  RunControlResetStart(int handle, unsigned int channel);


DDE3_API_API NI_RESULT  ControlLFSR(	bool allchannel,
										unsigned int channel,
										unsigned int source,
										unsigned int runstop,
										bool perform_reset,
										int handle);

DDE3_API_API NI_RESULT  RunControlEnable(	bool enable, 
										int handle, 
										unsigned int channel);

DDE3_API_API NI_RESULT	SECReadUIDSN	(UINT64 *UID, 
										 unsigned int *SN,
										 int handle);

DDE3_API_API NI_RESULT	SECWritekey(	int *key, 
										int length,
										int handle);

DDE3_API_API NI_RESULT	SECWriteSN(		unsigned int SN,
										int handle);

DDE3_API_API NI_RESULT	SECReadActivationStatus(unsigned int *active, 
												unsigned int *trial_counter, 
												unsigned int *trial_expired,
												int handle);


DDE3_API_API NI_RESULT  EmulatorAWGProgramScaler(unsigned int scaler,
												 int handle,
												 unsigned int channel);

DDE3_API_API NI_RESULT  EmulatorAWGModeControl(	unsigned int mode,
												int handle,
												unsigned int channel);

DDE3_API_API NI_RESULT  ProgramDDR(unsigned int *data1,
										unsigned int *time1,
										unsigned int  length1,
										unsigned int  *data2,
										unsigned int *time2,
										unsigned int  length2,
										unsigned int  memorymode1,
										unsigned int  memorymode2,
										int handle);

DDE3_API_API NI_RESULT ConfigureTR(	unsigned int ENABLE,
									unsigned int RISETIME,
									unsigned int LIMITHIGH,
									unsigned int SCALE,
									int handle,
									int channel);

DDE3_API_API NI_RESULT MCA_ReadPreview(int *data1,
	int *data2,
	int *Digital1,
	int *Digital2,
	int u1,
	int u2,
	int handle);

DDE3_API_API NI_RESULT DPP_Preview_SettingsSET(
	int mux1,
	int mux2,
	int dmux1,
	int dmux2,
	int triggersource,
	int int_trigger_val,
	int postlen,
	int rescale,
	int handle);

DDE3_API_API NI_RESULT DPP_Preview_ARMTrigger(int handle);


DDE3_API_API NI_RESULT DPP_Configure(
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
	double reset_holdoff,
	int handle
	);


DDE3_API_API NI_RESULT DPP_SpectrumCleanup(int handle
	);


DDE3_API_API NI_RESULT DPP_GetSpectrum(
	uint32_t *spectrum,
	int partial_NCumulative,
	int handle
	);

DDE3_API_API NI_RESULT DPP_SpectrumRun(
	int enable,
	int SPECTRUM_Limit_mode,
	double SPECTRUM_Limit_value,
	int handle
	);

DDE3_API_API NI_RESULT DPP_GetStats(
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
	unsigned int *timecnt,
	int handle
	);



DDE3_API_API NI_RESULT DPP_Reset(int handle
	);


DDE3_API_API NI_RESULT NI_SMClockControl(unsigned int inclock, unsigned int outclock, int handle);

DDE3_API_API NI_RESULT NI_SMReadClockControl(unsigned int *inclock, unsigned int *outclock, int handle);
