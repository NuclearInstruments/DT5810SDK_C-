#include "stdafx.h"
#include <windows.h>
#include "ftd2xx.h"
#include<iostream>
#include <time.h>

#include "CYPRESS_CPP_DLL.h"



#include <stdexcept>

#define RFA_ICAPDNA_WF					0xFEFFFF00
#define RFA_ICAPDNA_EEPROM				0xFEFFFF01
#define RFA_ICAPDNA_READEEPROM			0xFEFFFF07

static GUID NIUSBDRV_GUID = { 0xae18aa60, 0x7f6a, 0x11d4, 0x97, 0xdd, 0x0, 0xe, 0x2, 0x29, 0xb9, 0x59 };

using namespace std;

namespace NIUSBPHY
{

	NI_USBPHY::NI_USBPHY()
	{
		int i;
		i=1;

		m_selectedUSBDevice = new CCyUSBDevice(CypressHnd, NIUSBDRV_GUID, true); //CYUSBDRV_GUID

		inbuf = new UCHAR[8 * 1024576];
		outbuf = new UCHAR[8 * 1024576];
		return;
	}
	NI_USBPHY::~NI_USBPHY()
	{
		int i;
		i=1;
		return;
	}

NI_RESULT NI_USBPHY::EnumerateUsbDevice (tUSBDevice *pvArg1, unsigned int *numDevs)
{
	char *BufPtrs[255]; 
	char Buffer[255][64];
	unsigned int i;
	unsigned int nDevs;

	unsigned int flag;

    CCyUSBDevice	*USBDevice;
	USBDevice = new CCyUSBDevice(CypressHnd, NIUSBDRV_GUID , true); //CYUSBDRV_GUID
    
    
    if (USBDevice != NULL) 
    {
        int nInsertionCount = 0;
        int nDeviceCount = USBDevice->DeviceCount();
        for (int nCount = 0; nCount < nDeviceCount; nCount++ )
        {
            
            USBDevice->Open(nCount);
            
			strcpy_s(pvArg1[nCount].DESC, USBDevice->DeviceName);
			wcstombs(pvArg1[nCount].SN, USBDevice->SerialNumber, sizeof (pvArg1[nCount].SN) );
			pvArg1[nCount].id = nCount;
			pvArg1[nCount].pid = USBDevice->ProductID;
			pvArg1[nCount].vid = USBDevice->VendorID;
        
            USBDevice->Close();

			if (strstr(pvArg1[nCount].DESC, "WestBridge") != NULL)
				continue;
			OpenDeviceBySerialNumber(pvArg1[nCount].SN);
			
			//commutate bus on usb
			WriteReg(0, 0xFFFFFFFE);
			WriteReg(0, 0xFFFFFFFE);
			WriteReg(0, 0xFFFFFFFE);

			SECReadWord(0,16,&pvArg1[nCount].symbSN);
			ReadReg(&flag,0xFEFFFF03);
			pvArg1[nCount].trial = flag;


			ReadReg(&pvArg1[nCount].release,0xFFF00000);

			CloseConnection();
        }
        delete USBDevice;
        
		*numDevs = nDeviceCount;
		return NI_OK;
    }
	else
		return NI_ERROR_INTERFACE;

 
}


NI_RESULT NI_USBPHY::OpenDeviceBySerialNumber (char *SN)
{
	FT_STATUS ftStatus;
	char tmpSN[256];
	int deviceIdtoOpen = -1;
	CCyUSBDevice	*USBDevice;
	USBDevice = new CCyUSBDevice(CypressHnd, NIUSBDRV_GUID, true); //CYUSBDRV_GUID
    
	ConnectionStatus = NI_NOTCONNECTED;
    if (USBDevice != NULL) 
    {
        int nInsertionCount = 0;
        int nDeviceCount = USBDevice->DeviceCount();
        for (int nCount = 0; nCount < nDeviceCount; nCount++ )
        {
            
            USBDevice->Open(nCount);
			wcstombs(tmpSN, USBDevice->SerialNumber, 256 );

			if (strcmp(tmpSN, SN) == 0)
				deviceIdtoOpen = nCount;

            USBDevice->Close();

        }
        delete USBDevice;
        
    }

	if (deviceIdtoOpen>=0)
	{
		 m_selectedUSBDevice->Open(deviceIdtoOpen);
		if (! m_selectedUSBDevice->IsOpen()) 
			return NI_ERROR_INTERFACE;

		ConnectionStatus = NI_CONNECTED;

		int interfaces = this->m_selectedUSBDevice->AltIntfcCount()+1;
		OUTEndpt = m_selectedUSBDevice->BulkOutEndPt;
		INEndpt = m_selectedUSBDevice->BulkInEndPt;
		OUTEndpt->TimeOut = 4000;
		INEndpt->TimeOut = 4000;

		return NI_OK;
	}
	else
	{
		return NI_ERROR_INTERFACE;
	}


}


NI_RESULT NI_USBPHY::OpenDeviceByDeviceId (int devId)
{
	FT_STATUS ftStatus;
	
	
	if (devId>=0)
	{
		 m_selectedUSBDevice->Open(devId);
		if (! m_selectedUSBDevice->IsOpen()) 
			return NI_ERROR_INTERFACE;

		ConnectionStatus = NI_CONNECTED;

		int interfaces = this->m_selectedUSBDevice->AltIntfcCount()+1;
		OUTEndpt = m_selectedUSBDevice->BulkOutEndPt;
		INEndpt = m_selectedUSBDevice->BulkInEndPt;
		OUTEndpt->TimeOut = 4000;
		INEndpt->TimeOut = 4000;

		return NI_OK;
	}
	else
	{
		return NI_ERROR_INTERFACE;
	}


}

NI_RESULT NI_USBPHY::CloseConnection ()
{
	FT_STATUS ftStatus;
	m_selectedUSBDevice->Close ();
	if (m_selectedUSBDevice->IsOpen()) 
		return NI_ERROR_INTERFACE;
	else
		return NI_OK;
}

/*
NI_RESULT NI_USBPHY::WriteToFPGA (unsigned int *d, unsigned int addr, unsigned int length)
{
		
		unsigned int ll;
		unsigned int aa;

		LONG translen;
		DWORD maxTransferSize;
		DWORD remaining;
		DWORD size_in_byte;
		DWORD current_word;
		DWORD current_transfer_len;
		
		if (ConnectionStatus != NI_CONNECTED)
			return NI_ERROR_NOTCONNECTED;

		if (length > 16777216)
			return NI_ERROR_TRANSFER_MAX_LENGTH;

		if (m_selectedUSBDevice->bSuperSpeed == true)
			maxTransferSize = 8190;//1 * 1024576 - 8;
		else
			maxTransferSize = 8190;//4 * 1024576 - 8;


		remaining = length;
        current_word = 0;

		while(remaining > 0)
		{
			if (remaining * 4 > maxTransferSize)
                current_transfer_len = maxTransferSize / 4;
            else
                current_transfer_len = remaining;           

			size_in_byte = current_transfer_len * 4;
			ll = current_transfer_len;
			aa = addr + current_word;

			outbuf[3] = 0x63;
			outbuf[2] = (ll >> 16) & 0xFF;
			outbuf[1] = (ll >> 8) & 0xFF;
			outbuf[0] = (ll >> 0) & 0xFF;
			outbuf[7] = (aa >> 24) & 0xFF;
			outbuf[6] = (aa >> 16) & 0xFF;
			outbuf[5] = (aa >> 8) & 0xFF;
			outbuf[4] = (aa >> 0) & 0xFF;

			memcpy(&outbuf[8], &d[current_word], size_in_byte);
			translen = size_in_byte+8;
			if (OUTEndpt->XferData(outbuf, translen) == FALSE)
				return NI_ERROR_INTERFACE;

			current_word = current_word + current_transfer_len;
			remaining = remaining - current_transfer_len;

		}


		return NI_OK;
	
}


NI_RESULT NI_USBPHY::ReadFromFPGA (unsigned int *d, unsigned int addr, unsigned int length)
{
		
		unsigned int ll;
		unsigned int aa;
		
		
		LONG translen;
		DWORD maxTransferSize;
		DWORD remaining;
		DWORD size_in_byte;
		DWORD current_word;
		DWORD current_transfer_len;
		LONG size_input_buffer;
		DWORD roundto;



		if (ConnectionStatus != NI_CONNECTED)
			return NI_ERROR_NOTCONNECTED;

		if (length > 16777216)
			return NI_ERROR_TRANSFER_MAX_LENGTH;

		if (m_selectedUSBDevice->bSuperSpeed == true)
		{
			roundto = 16384;
			maxTransferSize = 1 * 1024576 ;
		}
		else
		{
			roundto = 8192;
			maxTransferSize = 4 * 1024576 ;
		}
	
		remaining = length;
        current_word = 0;

		while(remaining > 0)
		{
			if (remaining * 4 > maxTransferSize)
                current_transfer_len = maxTransferSize / 4;
            else
                current_transfer_len = remaining;           

			size_in_byte = current_transfer_len * 4;
			size_input_buffer = ceil ((double)size_in_byte/(double) roundto) * roundto;

			ll = current_transfer_len;
			aa = addr + current_word;

			outbuf[3] = 0x61;
			outbuf[2] = (ll >> 16) & 0xFF;
			outbuf[1] = (ll >> 8) & 0xFF;
			outbuf[0] = (ll >> 0) & 0xFF;
			outbuf[7] = (aa >> 24) & 0xFF;
			outbuf[6] = (aa >> 16) & 0xFF;
			outbuf[5] = (aa >> 8) & 0xFF;
			outbuf[4] = (aa >> 0) & 0xFF;

			
			translen = 8;
			if (OUTEndpt->XferData(outbuf, translen) == FALSE)
				return NI_ERROR_INTERFACE;

			if (INEndpt->XferData(&inbuf[0], size_input_buffer) == FALSE)
				return NI_ERROR_INTERFACE;

			current_word = current_word + current_transfer_len;
			remaining = remaining - current_transfer_len;

			memcpy(d, inbuf, size_in_byte);

		}

	

		return NI_OK;
}
*/
NI_RESULT NI_USBPHY::WriteToFPGA(unsigned int *d, unsigned int addr, unsigned int length)
{

	unsigned int ll;
	unsigned int aa;

	LONG translen;
	DWORD maxTransferSize;
	DWORD remaining;
	DWORD size_in_byte;
	DWORD current_word;
	DWORD current_transfer_len;

	if (ConnectionStatus != NI_CONNECTED)
		return NI_ERROR_NOTCONNECTED;

	if (length > 16777216)
		return NI_ERROR_TRANSFER_MAX_LENGTH;

	if (m_selectedUSBDevice->bSuperSpeed == true)
		maxTransferSize = 8 * 1024576 - 8;
	else
		maxTransferSize = 8 * 1024576 - 8;


	//OUTEndpt->Reset();
	remaining = length;
	current_word = 0;

	while (remaining > 0)
	{
		if (remaining * 4 > maxTransferSize)
			current_transfer_len = maxTransferSize / 4;
		else
			current_transfer_len = remaining;

		size_in_byte = current_transfer_len * 4;
		ll = current_transfer_len;
		aa = addr + current_word;

		outbuf[3] = 0xAB;
		outbuf[2] = 0xBA;
		outbuf[1] = 0xFF;
		outbuf[0] = 0xF1;
		outbuf[7] = (aa >> 24) & 0xFF;
		outbuf[6] = (aa >> 16) & 0xFF;
		outbuf[5] = (aa >> 8) & 0xFF;
		outbuf[4] = (aa >> 0) & 0xFF;
		outbuf[11] = (ll >> 24) & 0xFF;
		outbuf[10] = (ll >> 16) & 0xFF;
		outbuf[9] = (ll >> 8) & 0xFF;
		outbuf[8] = (ll >> 0) & 0xFF;



		CCyIsoPktInfo pk;

		memcpy(&outbuf[12], &d[current_word], size_in_byte);
		translen = size_in_byte + 12;
		if (OUTEndpt->XferData(outbuf, translen, &pk, true) == FALSE)
			return NI_ERROR_INTERFACE;

		current_word = current_word + current_transfer_len;
		remaining = remaining - current_transfer_len;

	}


	return NI_OK;

}



NI_RESULT NI_USBPHY::ReadFromFPGA(unsigned int *d, unsigned int addr, unsigned int length)
{

	unsigned int ll;
	unsigned int aa;


	LONG translen;
	DWORD maxTransferSize;
	DWORD remaining;
	DWORD size_in_byte;
	DWORD current_word;
	DWORD current_transfer_len;
	LONG size_input_buffer;
	LONG size_input_buffer2;
	DWORD roundto;



	INEndpt->Reset();
	INEndpt->TimeOut = 40000;
	if (ConnectionStatus != NI_CONNECTED)
		return NI_ERROR_NOTCONNECTED;

	if (length > 16777216)
		return NI_ERROR_TRANSFER_MAX_LENGTH;

	if (m_selectedUSBDevice->bSuperSpeed == true)
	{
		roundto = 16384;
		maxTransferSize = 8 * 1024576;
	}
	else
	{
		addr = addr - 1;
		roundto = 512;
		maxTransferSize = 8 * 1024576;
	}

	remaining = length;
	current_word = 0;

	while (remaining > 0)
	{

		if (remaining * 4 > maxTransferSize)
			current_transfer_len = maxTransferSize / 4;
		else
			current_transfer_len = remaining;

		size_in_byte = current_transfer_len * 4;
		size_input_buffer = ceil((double)(size_in_byte + 8) / (double)roundto) * roundto;

		ll = current_transfer_len + 2;
		aa = addr + current_word;

		outbuf[3] = 0xAB;
		outbuf[2] = 0xBA;
		outbuf[1] = 0xFF;
		outbuf[0] = 0xF0;
		outbuf[7] = (aa >> 24) & 0xFF;
		outbuf[6] = (aa >> 16) & 0xFF;
		outbuf[5] = (aa >> 8) & 0xFF;
		outbuf[4] = (aa >> 0) & 0xFF;
		outbuf[11] = (ll >> 24) & 0xFF;
		outbuf[10] = (ll >> 16) & 0xFF;
		outbuf[9] = (ll >> 8) & 0xFF;
		outbuf[8] = (ll >> 0) & 0xFF;

		translen = 12;
		if (OUTEndpt->XferData(outbuf, translen) == FALSE)
			return NI_ERROR_INTERFACE;

		//Sleep(50);
		CCyIsoPktInfo pk;
		bool res = false;
		size_input_buffer2 = size_input_buffer;
		do {
			size_input_buffer = size_input_buffer2;
			res = INEndpt->XferData(&inbuf[0], size_input_buffer, &pk, false);
		} while ((size_input_buffer != 0) && (res == true));

		memcpy(&d[current_word], &inbuf[4], size_in_byte);

		current_word = current_word + current_transfer_len;
		remaining = remaining - current_transfer_len;



	}



	return NI_OK;
}

NI_RESULT NI_USBPHY::WriteReg (unsigned int regVal, unsigned int addr)
{

	return WriteToFPGA(&regVal, addr,1);

}

NI_RESULT NI_USBPHY::ReadReg (unsigned int *regVal, unsigned int addr)
{

	return ReadFromFPGA(regVal, addr,1);

}



NI_RESULT NI_USBPHY::SECReadWord(unsigned int channel, unsigned int address, unsigned int *word)
{
	unsigned int w;
	if(WriteReg(0, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;

	if(WriteReg((address << 16) + (8 << 24), RFA_ICAPDNA_EEPROM ) != NI_OK)
		return NI_ERROR;

	if(WriteReg(1, RFA_ICAPDNA_WF ) != NI_OK)
		return NI_ERROR;
	if(WriteReg(0, RFA_ICAPDNA_WF ) != NI_OK)
		return NI_ERROR;

	Sleep (10);

	if(ReadReg(&w, RFA_ICAPDNA_READEEPROM ) != NI_OK)
		return NI_ERROR;
	if(WriteReg(0, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;

	if(WriteReg(1, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;
	if(WriteReg(0, RFA_ICAPDNA_WF) != NI_OK)
		return NI_ERROR;

	*word = w & 0xFFFF;
	
	return NI_OK;
}
}