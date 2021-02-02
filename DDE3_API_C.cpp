#include "stdafx.h"
#include "DDE3_API_C.h"
#include "DDE3_API.h"
struct 
{
	DDE3_API *niDEE3;
	int connection_mode;
	int valid;
} Devices[MAX_NUMBER_OF_DEVICE];


DDE3_API_API NI_RESULT NI_Startup()
{
	return NI_OK;
}

DDE3_API_API NI_RESULT NI_AttachNewDevice(int CONNECTION_MODE, char *IPAddress_or_SN, int TCPPort, int UDPPort, int *handle)
{
	int newdevid;
	int i;
	newdevid =-1;

	

		for (i=0;i<MAX_NUMBER_OF_DEVICE;i++)
		{
			if (Devices[i].valid == 0)
			{
				newdevid = i;
				break;
			}
		}
		if (newdevid == -1)
		{
			return NI_TOO_MANY_DEVICES_CONNECTED;
		}
		else
		{
			*handle = newdevid;
			Devices[*handle].niDEE3 = new DDE3_API () ;
			Devices[*handle].niDEE3->Allocate ();
			if (CONNECTION_MODE==NI_CONNECT_USB)
			{
				if (Devices[*handle].niDEE3->USBConnect (IPAddress_or_SN) == NI_OK )
				{
					Devices[*handle].valid = 1;
					Devices[*handle].connection_mode = NI_CONNECT_USB;
					return NI_OK;
				}
			}
			else
			{
				if (Devices[*handle].niDEE3->EthernetConnect (TCPPort,IPAddress_or_SN) ==NI_OK)
				{
					Devices[*handle].valid = 1;
					Devices[*handle].connection_mode = NI_CONNECT_ETH;
					return NI_OK;
				}

			}

			//Connection failed
			Devices[*handle].niDEE3->Destroy ();
			delete Devices[*handle].niDEE3;
			*handle = 0;
			return NI_ERROR;
		}

	
}

DDE3_API_API NI_RESULT NI_DeleteDevice(int handle)
{
	if (Devices[handle].valid == 1)
	{

		Devices[handle].valid = 0;
		Devices[handle].niDEE3->Destroy ();
		delete Devices[handle].niDEE3;
		return NI_OK;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}



DDE3_API_API int NI_ConnectionStatus( int *status,
								  int handle, 
								  int channel	)
{
	*status = 0;
	if (Devices[handle].valid == 1)
	{
		*status = Devices[handle].niDEE3->ConnectionStatus( );
		return NI_OK;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}



DDE3_API_API NI_RESULT NI_USBEnumerate( tUSBDevice *pvArg1, unsigned int *numDevs	)
{
	NI_RESULT Status;
	DDE3_API enumClass;
	return enumClass.USBEnumerate(pvArg1,numDevs);
}

DDE3_API_API NI_RESULT NI_ETHEnumerate(tETHDevices *pvArg1)
{
	NI_RESULT Status;
	DDE3_API enumClass;
	return enumClass.ETHEnumerate(pvArg1);
}

DDE3_API_API NI_RESULT ETHGetSerialNumber(short int port, char *IPAddress, char *SN)
{
	NI_RESULT Status;
	DDE3_API enumClass;
	return enumClass.ETHGetSerialNumber(port,IPAddress, SN);
}


DDE3_API_API NI_RESULT NI_DHA_WriteReg(UINT32 value, 
									  UINT32 address,
									  int handle, 
									  int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->DHA_WriteReg(value, address);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT NI_DHA_ReadReg(UINT32 *value, 
									  UINT32 address,
									  int handle, 
									  int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->DHA_ReadReg(value, address);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT NI_DHA_WriteArray(UINT32 *value, 
									  UINT32 address,
									  UINT32 length,
									  int handle, 
									  int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->DHA_WriteArray(value, address, length);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT NI_DHA_ReadArray(UINT32 *value, 
									  UINT32 address,
									  UINT32 length,
									  int handle, 
									  int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->DHA_ReadArray(value, address, length);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT EnableChanel(UINT32 ENABLE,
									  int handle, 
									  int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->EnableChanel(channel, ENABLE);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT ConfigureEnergy(	
										UINT32 MODE,
										UINT32 ENERGY,
										int handle, 
										int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureEnergy(channel, MODE, ENERGY);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT ProgramSpectrum(	
										UINT32 *spectrum,
										UINT32 spectrumLen,
										int handle, 
										int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ProgramSpectrum(channel, spectrum, spectrumLen);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT ConfigureTimebase(	
											UINT32 MODE, 
											double RATE, 
											UINT64 DeadTime, 
											bool Parallelizable,
											int handle, 
											int channel	)

{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureTimebase(channel, MODE, RATE, DeadTime, Parallelizable);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT ConfigureDIO(	
										UINT32 OUTTRIGGEREN,
										UINT32 OUTTRIGGERLEN,
										int handle, 
										int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureDIO(channel, OUTTRIGGEREN, OUTTRIGGERLEN);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT ConfigureDRC(	
										UINT32 RISETIME,
										UINT32 FALLTIME,
										UINT32 ENABLE,
										int handle, 
										int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureDRC(channel, RISETIME, FALLTIME,ENABLE);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT ConfigureTR( unsigned int ENABLE, 
									unsigned int RISETIME, 
									unsigned int LIMITHIGH, 
									unsigned int SCALE,
									int handle,
									int channel)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureTR(channel, ENABLE, RISETIME, LIMITHIGH, SCALE);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

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
												int channel)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureShapeGenerator(	channel,
																	shape,
																	shape_id,
																	shape_length,
																	multishape_id,
																	rising_falling_crosspoint,
																	interpolator_factor_rising,
																	interpolator_factor_falling,
																	reconfigure_shape,
																	enable_shape);

		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT ConfigureMultishape(		double prob2,
												double prob3,
												double prob4,
												bool enable,
												int handle,
												int channel)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureMultishape(		channel,
																	prob2,
																	prob3,
																	prob4,
																	enable);

		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}



DDE3_API_API NI_RESULT ConfigureBaselineDrift(	int *baseline,
												unsigned int baseline_length, 
												unsigned int interpolator_factor_slow,
												unsigned int interpolator_factor_fast,
												bool reconfigure_drift,
												bool enable_shape,
												bool reset,
												int handle,
												int channel)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureBaselineDrift(		channel,
																		baseline,
																		baseline_length, 
																		interpolator_factor_slow,
																		interpolator_factor_fast,
																		reconfigure_drift,
																		enable_shape,
																		reset);

		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}





DDE3_API_API NI_RESULT ConfigureNOISE(	
										UINT32 RANDM,
										UINT32 GAUSSM,
										UINT32 DRIFTM,
										UINT32 FLIKERM,
										UINT32 FLIKERCorner,
										int handle, 
										int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureNOISE(channel, RANDM, GAUSSM, DRIFTM, FLIKERM, FLIKERCorner);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT ConfigureGeneral(	
										double GAIN,
										INT32 OFFSET,
										UINT32 INVERT,
										UINT32 OUTFILTER,
										UINT32 ANALOGSEL,
										int handle, 
										int channel	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->ConfigureGeneral(channel, GAIN, OFFSET, INVERT, OUTFILTER, ANALOGSEL);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}




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
										   unsigned int channel)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niDEE3->UpdateDisplayStatus( channel,
											timemode, 
										    rate, 
										    ratep, 
										    time_str, 
										    energy_mode, 
										    energy, 
										    energy_str, 
										    shape_str, 
										    live,
											funcgen,
											funcgen_mvolt,
											funcgen_freq,
											funcgen_str
										    );
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API double ChannelsToVoltage(	
										INT32 CHANNELS,
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ChannelsToVoltage(channel, CHANNELS);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API double VoltageToChannels(	
										double VOLTAGE,
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->VoltageToChannels(channel, VOLTAGE);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT GetStat(	
										UINT32 *CPS,
										UINT32 *LIVE,
										UINT64 *CCOUNTER,
										UINT32 *OVERFLOW,
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->GetStat(channel, CPS, LIVE, CCOUNTER, OVERFLOW);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT ResetCCounter(	
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ResetCCounter(channel);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT GetSignalLoopback(	
										INT32 *DATA,
										UINT32 *LEN,
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->GetSignalLoopback(channel, DATA, LEN);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT GetSpectrumLoopback(	
									UINT32  *DATA,
										UINT32 *LEN,
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->GetSpectrumLoopback(channel, DATA, LEN);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT ResetSpectrum(	
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ResetSpectrum (channel);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT ActiveteGetUID(	
										unsigned int  *key1,
										unsigned int  *key2,
										int handle)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ActiveteGetUID(key1,key2);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT ActiveteRegister(	
										unsigned char  *DATA,
										int handle)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ActiveteRegister(DATA);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT HardwareInfo(	
										UINT32  *HWREV,
										UINT32  *FWREV,
										UINT32 *UCREV,
										UINT32  *HWOPTIONS,
										UINT32  *DEVICEMODEL,
										int handle)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->HardwareInfo(HWREV, UCREV, FWREV, HWOPTIONS, DEVICEMODEL);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT Reboot(			UINT32 MODE,
										int handle)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->Reboot(MODE);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT CheckRunningMode(
										UINT32 *MODE,
										int handle)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->CheckRunningMode(MODE);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT FlashLock(			
										UINT32 MODE,
										int handle)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->FlashLock(MODE);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT FLASHWritePage(			
										UINT32 PAGEN,
										UCHAR *DATA,
										UINT32 LEN,
										int handle)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->FLASHWritePage(PAGEN, DATA, LEN);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT FLASHErasePage(			
										UINT32 PAGEN,
										int handle)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->FLASHErasePage(PAGEN);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT SECWriteWord(
										UINT32 address,
										UINT32 word,
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SECWriteWord (channel, address, word);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT SECReadWord(
										UINT32 address,
										UINT32 *word,
										int handle, 
										int channel	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SECReadWord (channel, address, word);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT IsActivated(
										UINT32 *word,
										int handle	)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->IsActivated (word);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT WriteCalibrarionFlash(
												double offset, 
												double gain, 
												double chctv,
												int handle,
												unsigned int channel)
{
	
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->WriteCalibrarionFlash(channel, offset, gain, chctv);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT ReadCalibrarionFlash(
	double *offset,
	double *gain,
	double *chctv,
	int handle,
	unsigned int channel)
{

	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ReadCalibrarionFlash(channel, offset, gain, chctv);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT ConfigureLFSR(	UINT64 SEED, 
										t_LFSR_ID LFSR_ID, 
										t_LFSR_OPERATION Operation,
										int handle,
										unsigned int channel)
{

	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ConfigureLFSR(channel, SEED, LFSR_ID, Operation);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}

}


DDE3_API_API NI_RESULT SetAnalogDatapath    (unsigned int filter, 
											 unsigned int hvlv,
											 int handle,
										     unsigned int channel)
{

	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SetAnalogDatapath(channel, filter, hvlv);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}

}


DDE3_API_API NI_RESULT SetInputAnalogDatapath(	unsigned int input_imp,
												unsigned int input_div,
												unsigned int input_mode,
												unsigned int input_scale,
												unsigned int input_gain,
												double offset,
												int handle,
												unsigned int channel)
{

	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SetInputAnalogDatapath( channel, 
												input_imp,
												input_div,
												input_mode,
												input_scale,
												input_gain,
												offset
												);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}

}




DDE3_API_API NI_RESULT  SetInputAnalogMix(		double gain_a,
												int offset_a,
												double gain_b,
												int offset_b,
												unsigned int enable_a,
												unsigned int enable_b,
												unsigned int inv_a,
												unsigned int inv_b,
												int handle,
												unsigned int channel)
{

	if (Devices[handle].valid == 1)
	{


		return Devices[handle].niDEE3->SetInputAnalogMix(		channel, 
										gain_a,
										offset_a,
										gain_b,
										offset_b,
										enable_a,
										enable_b,
										inv_a,
										inv_b
									);

	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}





DDE3_API_API NI_RESULT SetDIO(		unsigned int Dio_In_A, 
									unsigned int Dio_In_B, 
									unsigned int Dio_Out_A,
									unsigned int Dio_Out_B,
									unsigned int DIO_Out_PulseLen,
									double delayA,
									double delayB,
									int handle
							)
{
	if (Devices[handle].valid == 1)
	{

		DIO_Out_PulseLen =
			DIO_Out_PulseLen / 4 -1;
		if (DIO_Out_PulseLen < 2) DIO_Out_PulseLen = 2;
		return Devices[handle].niDEE3->SetDIO(	(DIO_IN_MODE) Dio_In_A, 
										(DIO_IN_MODE)  Dio_In_B,
										(DIO_OUT_MODE) Dio_Out_A,
										(DIO_OUT_MODE) Dio_Out_B,
										DIO_Out_PulseLen,
										delayA,
										delayB
									);

	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT  DelayAndCorrelationControl( unsigned int correlation_mode,
													unsigned int enableCchannel,
													double delay,
													int handle)

{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DelayAndCorrelationControl( correlation_mode,
													enableCchannel,
													delay);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}

}






DDE3_API_API NI_RESULT  EnergyMux(	 unsigned int mode,
									 int handle,
									 unsigned int channel)

{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->EnergyMux(	channel,
													mode);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}

}
DDE3_API_API NI_RESULT  TimebaseMux( unsigned int mode,
									 int handle,
									 unsigned int channel)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->TimebaseMux(	channel,
													mode);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}




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
								unsigned int channel)

{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->GetLiveData( channel,
								run_time,
								sat_time,
								busy_time,
								real_time,
								cnt_event,
								sat_event,
								lost_event,
								measured_rate,
								real_event,
								busyflag,
								satflag,
								eflag);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT  SetRunControlMode(	unsigned int rmode, 
											double limitvalue,
											int handle,
											unsigned int channel)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SetRunControlMode( channel,
								(tRUNCONTROL_MODE) rmode,
								limitvalue);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT  RunControlResetStat(int handle,
											unsigned int channel)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->RunControlResetStat( channel );
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}
DDE3_API_API NI_RESULT  RunControlResetStart(int handle,
											 unsigned int channel)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->RunControlResetStart( channel );
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT  ControlLFSR(	bool allchannel,
										unsigned int channel,
										unsigned int source,
										unsigned int runstop,
										bool perform_reset,
										int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ControlLFSR(		allchannel,
														channel,
														(tLFSR_CONTROLPIN)source,
														runstop,
														perform_reset);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT  RunControlEnable(	bool enable, 
										int handle, 
										unsigned int channel)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->RunControlEnable( channel, enable);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}




DDE3_API_API NI_RESULT  ProgramDDR(	unsigned int *data1,
									unsigned int *time1,
									unsigned int  length1, 
									unsigned int  *data2, 
									unsigned int *time2,
									unsigned int  length2, 
									unsigned int  memorymode1, 
									unsigned int  memorymode2,
									int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->ProgramDDR(data1, time1, length1, data2, time2, length2, memorymode1, memorymode2);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}



DDE3_API_API NI_RESULT  EmulatorAWGModeControl(unsigned int mode,
	int handle,
	unsigned int channel)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->EmulatorAWGModeControl(mode, channel);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT  EmulatorAWGProgramScaler(unsigned int scaler,
	int handle,
	unsigned int channel)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->EmulatorAWGProgramScaler(scaler, channel);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}




DDE3_API_API NI_RESULT	SECReadUIDSN	(UINT64 *UID, 
										 unsigned int *SN,
										 int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SECReadUIDSN	(UID, SN);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT	SECWritekey(	int *key, 
										int length,
										int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SECWritekey(key, length);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT	SECWriteSN(		unsigned int SN,
										int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SECWriteSN( SN );
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT	SECReadActivationStatus(unsigned int *active, 
												unsigned int *trial_counter, 
												unsigned int *trial_expired,
												int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SECReadActivationStatus( active, trial_counter, trial_expired);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}

}


DDE3_API_API NI_RESULT MCA_ReadPreview(int *data1,
	int *data2,
	int *Digital1,
	int *Digital2,
	int u1,
	int u2,
	int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->MCA_ReadPreview(data1, data2, Digital1, Digital2, u1, u2);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT DPP_Preview_SettingsSET(
	int mux1,
	int mux2,
	int dmux1,
	int dmux2,
	int triggersource,
	int int_trigger_val,
	int postlen,
	int rescale,
	int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DPP_Preview_SettingsSET(mux1, mux2, dmux1, dmux2, triggersource, int_trigger_val, postlen, rescale);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


DDE3_API_API NI_RESULT DPP_Preview_ARMTrigger(int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DPP_Preview_ARMTrigger();
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}



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
	)

{

	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DPP_Configure(
			TRIGGER_Threshold,
			TRIGGER_peaking,
			TRIGGER_holdoff,
			FILTER_tau,
			FILTER_peaking,
			FILTER_ft,
			FILTER_mean,
			FILTER_delay,
			FILTER_gain,
			SATURATION_level,
			SATURATION_holdoff,
			PEAKING_holdoff,
			BASELINE_mean,
			BASELINE_holdoff,
			DECIMATOR_scale,
			EWIN_min,
			EWIN_max,
			reset_detector,
			reset_level,
			reset_holdoff);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}

}



DDE3_API_API NI_RESULT DPP_Reset(
		int handle
	)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DPP_Reset();
	}
	else
	{
		return NI_INVALID_HANDLE;
	}


}
DDE3_API_API NI_RESULT DPP_SpectrumCleanup(int handle
	)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DPP_SpectrumCleanup();
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT DPP_GetSpectrum(
	uint32_t *spectrum,
	int partial_NCumulative,
	int handle
	)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DPP_GetSpectrum(
			spectrum,
			partial_NCumulative);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT DPP_SpectrumRun(
	int enable,
	int SPECTRUM_Limit_mode,
	double SPECTRUM_Limit_value,
	int handle
	)

{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DPP_SpectrumRun(
			enable,
			SPECTRUM_Limit_mode,
			SPECTRUM_Limit_value);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

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
	)

{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->DPP_GetStats(
			running_time,
			live_time,
			dead_time,
			partial_time,
			partial_live_time,
			partial_dead_time,
			total_in_cnt,
			total_out_cnt,
			total_piledup_cnt,
			total_saturation_cnt,
			total_resets_cnt,
			partial_in_cnt,
			partial_out_cnt,
			partial_piledup_cnt,
			partial_saturation_cnt,
			partial_resets_cnt,
			status,
			limitcnt,
			timecnt

			);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}

}

DDE3_API_API NI_RESULT NI_SMClockControl(	unsigned int inclock, 
											unsigned int outclock, 
											int handle)
{

	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SMClockControl(
			inclock,
			outclock);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

DDE3_API_API NI_RESULT NI_SMReadClockControl(unsigned int *inclock, unsigned int *outclock, int handle)
{
	if (Devices[handle].valid == 1)
	{
		return Devices[handle].niDEE3->SMReadClockControl(
			inclock,
			outclock);
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}