#define TIMER_DELAY 1
#pragma once
#include "ETHUSB_HAL_LIB.h"
#include <stdint.h>


class DDE3_API {
public:
	DDE3_API::DDE3_API();
	DDE3_API::~DDE3_API();
	NI_RESULT DDE3_API::EthernetConnect(short int port, char *IPAddress);
	NI_RESULT DDE3_API::USBConnect(char *SN);
	NI_RESULT DDE3_API::CloseConnection();
	NI_RESULT DDE3_API::ConnectionStatus();

	NI_RESULT DDE3_API::USBEnumerate(tUSBDevice *pvArg1, unsigned int *numDevs);
	NI_RESULT DDE3_API::ETHEnumerate(tETHDevices *pvArg1);
	NI_RESULT DDE3_API::ETHGetSerialNumber(short int port, char *IPAddress, char *SN);


	NI_RESULT DDE3_API::DHA_WriteReg(UINT32 value, UINT32 address);
	NI_RESULT DDE3_API::DHA_ReadReg(UINT32 *value, UINT32 address);
	NI_RESULT DDE3_API::DHA_WriteArray(UINT32 *value, UINT32 address, UINT32 length);
	NI_RESULT DDE3_API::DHA_ReadArray(UINT32 *value, UINT32 address, UINT32 length);
	NI_RESULT DDE3_API::SetuDDERegister(UINT32 value, UINT32 address);
	NI_RESULT DDE3_API::GetuDDERegister(UINT32 *value, UINT32 address);
	
	NI_RESULT DDE3_API::ConfigureDAC(unsigned int channel, unsigned int ENABLE);
	NI_RESULT DDE3_API::ConfigureCLOCKGEN();

	NI_RESULT DDE3_API::ReadHardwareProperties();
	NI_RESULT DDE3_API::EnableChanel(unsigned int channel, unsigned int ENABLE);
	NI_RESULT DDE3_API::ConfigureEnergy(unsigned int channel, unsigned int MODE, unsigned int ENERGY);
	NI_RESULT DDE3_API::ProgramSpectrum(unsigned int channel, unsigned int *spectrum, unsigned int spectrumLen);
	NI_RESULT DDE3_API::ConfigureTimebase(	unsigned int channel, 
											unsigned int MODE, 
											double RATE, 
											unsigned long DeadTime, 
											bool Parallelizable);

	NI_RESULT DDE3_API::ConfigureDIO(unsigned int channel, unsigned int INTRIGGER, unsigned int OUTTRIGGER);


	NI_RESULT DDE3_API::ConfigureShapeGenerator(	unsigned int channel,
												int *shape,
												unsigned int shape_id,
												unsigned int shape_length, 
												int multishape_id,
												unsigned int rising_falling_crosspoint,
												unsigned int interpolator_factor_rising,
												unsigned int interpolator_factor_falling,
												bool reconfigure_shape,
												bool enable_shape);

	NI_RESULT DDE3_API::ConfigureMultishape(	unsigned int channel,
												double prob2,
												double prob3,
												double prob4,
												bool enable);

	
	NI_RESULT DDE3_API::ConfigureBaselineDrift(	unsigned int channel,
												int *baseline,
												unsigned int baseline_length, 
												unsigned int interpolator_factor_slow,
												unsigned int interpolator_factor_fast,
												bool reconfigure_drift,
												bool enable_shape,
												bool reset);

	NI_RESULT DDE3_API::ConfigureDRC(unsigned int channel, unsigned int RISETIME, unsigned int FALLTIME, unsigned int ENABLE);
	NI_RESULT DDE3_API::ConfigureNOISE(unsigned int channel, unsigned int RANDM, unsigned int GAUSSM, unsigned int DRIFTM, unsigned int FLIKERM, unsigned int FLIKERCorner);
	NI_RESULT DDE3_API::ConfigureGeneral(unsigned int channel, double gain, int offset, unsigned int invert, unsigned int OUTFILTER, unsigned int ANALOGSEL);
	double DDE3_API::ChannelsToVoltage(int channel, int channels);
	unsigned int DDE3_API::VoltageToChannels(unsigned int channel, double voltage);


	NI_RESULT DDE3_API::ReprogramLFSREnergy(ULONG64 seed, unsigned int channel);
	NI_RESULT DDE3_API::ReprogramLFSRTimebase(ULONG64 seed,  unsigned int channel);
	NI_RESULT DDE3_API::ReprogramLFSRMultishape(ULONG64 seed, unsigned int channel);
	NI_RESULT DDE3_API::ReprogramLFSRNoise(ULONG64 seed, unsigned int channel);
	NI_RESULT DDE3_API::UpdateDisplayStatus(unsigned int channel, int timemode, int rate, int ratep, char *time_str, int energy_mode, int energy, char *energy_str, char *shape_str, int live, int funcgen, int funcgen_mvolt, int funcgen_freq, char *funcgen_str);
	NI_RESULT DDE3_API::SetAnalogDatapath(unsigned int channel, unsigned int filter, unsigned int hvlv);


	NI_RESULT DDE3_API::GetStat(unsigned int channel, unsigned int *cps,  unsigned int *live, UINT64 *ccounter, unsigned int *overflow);
	NI_RESULT DDE3_API::ResetCCounter(unsigned int channel);
	NI_RESULT DDE3_API::GetSignalLoopback(unsigned int channel, INT32 *data, unsigned int *len);
	NI_RESULT DDE3_API::ResetSpectrum(unsigned int channel);
	NI_RESULT DDE3_API::GetSpectrumLoopback(unsigned int channel, UINT32 *data, unsigned int *len);
	NI_RESULT DDE3_API::ActiveteGetUID(unsigned int *key1, unsigned int *key2);
	NI_RESULT DDE3_API::ActiveteRegister(unsigned char *data);
	NI_RESULT DDE3_API::HardwareInfo(unsigned int *HWREV, unsigned int *UCREV, unsigned int *FWREV, unsigned int *HWOPTIONS, unsigned int *DEVICEMODEL);
	NI_RESULT DDE3_API::Reboot(unsigned int mode);
	NI_RESULT DDE3_API::CheckRunningMode(unsigned int *mode);
	NI_RESULT DDE3_API::FlashLock(unsigned int mode);
	NI_RESULT DDE3_API::FLASHWritePage(unsigned int pagen, unsigned char *data, unsigned int len);
	NI_RESULT DDE3_API::FLASHErasePage(unsigned int pagen);
	NI_RESULT DDE3_API::SECWriteWord(unsigned int channel, unsigned int address, unsigned int word);
	NI_RESULT DDE3_API::SECReadWord(unsigned int channel, unsigned int address, unsigned int *word);
	NI_RESULT DDE3_API::IsActivated(unsigned int *data);
	NI_RESULT DDE3_API::WriteCalibrarionFlash(unsigned int channel, double offset, double gain, double chctv);
	NI_RESULT DDE3_API::ReadCalibrarionFlash(unsigned int channel, double *offset, double *gain, double *chctv);

	NI_RESULT DDE3_API::ConfigureLFSR(unsigned int channel, unsigned long SEED, t_LFSR_ID LFSR_ID, t_LFSR_OPERATION Operation);
	
	NI_RESULT DDE3_API::SetInputAnalogDatapath(	unsigned int channel, 
												unsigned int input_imp,
												unsigned int input_div,
												unsigned int input_mode,
												unsigned int input_scale,
												unsigned int input_gain,
												double offset
												);
	NI_RESULT DDE3_API::SetInputAnalogMix(		    unsigned int channel, 
												double gain_a,
												 int offset_a,
												double gain_b,
												 int offset_b,
												unsigned int enable_a,
												unsigned int enable_b,
												unsigned int inv_a,
												unsigned int inv_b
									);

	NI_RESULT DDE3_API::DelayAndCorrelationControl( unsigned int correlation_mode,
												unsigned int enableCchannel,
												double delay);

	NI_RESULT DDE3_API::EnergyMux( unsigned int channel,
								 unsigned int mode);
	NI_RESULT DDE3_API::TimebaseMux( unsigned int channel,
								 unsigned int mode);



	NI_RESULT DDE3_API::SetDIO(		DIO_IN_MODE Dio_In_A, 
									DIO_IN_MODE Dio_In_B, 
									DIO_OUT_MODE Dio_Out_A,
									DIO_OUT_MODE Dio_Out_B,
									unsigned int DIO_Out_PulseLen,
									double delayA,
									double delayB
							);



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
								UINT32 *eflag);

	NI_RESULT DDE3_API::SetRunControlMode(unsigned int channel, tRUNCONTROL_MODE rmode, double limitvalue);
	NI_RESULT DDE3_API::RunControlResetStat(unsigned int channel);
	NI_RESULT DDE3_API::RunControlResetStart(unsigned int channel);

	NI_RESULT DDE3_API::ProgramDataPhase(unsigned int channel, unsigned int phase);



	NI_RESULT DDE3_API::ControlLFSR(	bool allchannel,
										unsigned int channel,
										tLFSR_CONTROLPIN source,
										unsigned int runstop,
										bool perform_reset);

	NI_RESULT DDE3_API::RunControlEnable(unsigned int channel, bool enable);

	NI_RESULT DDE3_API::WriteDAC(unsigned int channel, unsigned int addr, unsigned int val);
	NI_RESULT DDE3_API::ReadDAC(unsigned int channel, unsigned int addr, unsigned int val, unsigned int *readdata);
	NI_RESULT DDE3_API::ProgramDDR(UINT32 *data1, UINT32 *time1, UINT32 length1, UINT32 *data2, UINT32 *time2, UINT32 length2, UINT32 memorymode1, UINT32 memorymode2);
	NI_RESULT DDE3_API::EmulatorAWGModeControl(UINT32 mode, UINT32 channel);
	NI_RESULT DDE3_API::EmulatorAWGProgramScaler(UINT32 scaler, UINT32 channel);

	NI_RESULT DDE3_API::SyncToDevice();
	NI_RESULT DDE3_API::SyncFromDevice();

	NI_RESULT DDE3_API::Load_image(char *fpath);
	NI_RESULT DDE3_API::Load_psf(char *fpath);

	NI_RESULT DDE3_API::SECWritekey(int *key, int length);
	NI_RESULT DDE3_API::SECWriteSN(unsigned int SN);
	NI_RESULT DDE3_API::SECReadUIDSN(UINT64 *UID, unsigned int *SN);
	NI_RESULT DDE3_API::SECReadActivationStatus(unsigned int *active, unsigned int *trial_counter, unsigned int *trial_expired);

	NI_RESULT DDE3_API::ConfigureCLOCKGENDT5810B(int freq);

	NI_RESULT DDE3_API::ConfigureTR(unsigned int channel, unsigned int ENABLE, unsigned int RISETIME, unsigned int LIMITHIGH, unsigned int SCALE);



	NI_RESULT DDE3_API::MCA_ReadPreview(int *data1,
		int *data2,
		int *Digital1,
		int *Digital2,
		int u1,
		int u2);

	NI_RESULT DDE3_API::DPP_Preview_SettingsSET(
		int mux1,
		int mux2,
		int dmux1,
		int dmux2,
		int triggersource,
		int int_trigger_val,
		int postlen,
		int rescale);

	NI_RESULT DDE3_API::DPP_Preview_ARMTrigger();


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
		);

	NI_RESULT DDE3_API::DPP_SpectrumCleanup(
		);

	NI_RESULT DDE3_API::DPP_GetSpectrum(
		uint32_t *spectrum,
		int partial_NCumulative
		);

	NI_RESULT DDE3_API::DPP_SpectrumRun(
		int enable,
		int SPECTRUM_Limit_mode,
		double SPECTRUM_Limit_value
		);

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
		);


	NI_RESULT DDE3_API::DPP_Reset(
		);

	NI_RESULT DDE3_API::SECWriteOPTIONSLIC(unsigned int KEY, 
		unsigned int register_NOTunregister, 
		unsigned int id);

	void DDE3_API::Allocate();
	void DDE3_API::Destroy();

	NI_RESULT DDE3_API::SMClockControl(unsigned int inclock, unsigned int outclock);
	NI_RESULT DDE3_API::SMReadClockControl(unsigned int *inclock, unsigned int *outclock);

private:
	typedef enum 
	{
		DT5810 = 0,
		DT5810B = 1
	}t_INSTRUMENTVERSION ;

	t_INSTRUMENTVERSION  INSTRUMENTVERSION;

	NI_RESULT DDE3_API::cfg1000_in_out(int clk_in, int clk_out);

	int MCAPostLen;

	NI_RESULT DDE3_API::UnlockDisplays();
	void DDE3_API::programReg(unsigned int channel, unsigned int addr, unsigned int val);
	
	NIHAL::NI_HAL *niHAL;

	double _clockFREQUENCY;
	double _gOFFSET[4];
	double _gGAIN[4];
	double _VoltageChannelFactor[4];
	int _ChannelsCount;
	unsigned int peout1;

	uint32_t *MCAspectrum;
	protected:
};

	


