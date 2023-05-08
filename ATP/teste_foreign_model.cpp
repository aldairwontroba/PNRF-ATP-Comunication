#include "stdafx.h"
#include "windows.h"
#include "stdlib.h"

#define BUF_SIZE 64

#define yes 0
#define no 1

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

TCHAR MapObjATP_Value_reader[]=TEXT("ATP_Value_Mapping_reader");
TCHAR MapObjATP_WorkEvent_reader[]=TEXT("WorkEvent_reader");
TCHAR MapObjATP_BackEvent_reader[]=TEXT("BackEvent_reader");

HANDLE workHandle_reader;
HANDLE backHandle_reader;

DWORD timeout_reader;

HANDLE hMapFile_Value_reader;
LPCTSTR pBuf_Value_reader;

struct ChannelsBufferValue_reader
{
    double VOLTAGE;
    double CURRENT;
    double ATPtime;
    double ATPtimeStart;
    double ATPtimeStop;
    double ATPtimeStep;
    double TstartTime;
    double errorMsg;
};


ChannelsBufferValue_reader ChannelsBufferATP_reader;


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void error_reporter(char* msg, int x, bool y)
{
	printf("\n %s %d", msg, x);
	if (y) system("PAUSE");
	exit(1);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
extern "C" 
{
    void  outsix_(char *, int *); 

    void c_reader_i__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[]);
    void c_reader_m__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[]);

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void c_reader_i__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[])
{
	double ATPtime      = xin_ar[0];
	double ATPtimeStart = xin_ar[1];
	double ATPtimeStop  = xin_ar[2];
	double ATPtimeStep  = xin_ar[3];
	double voltage = xin_ar[4];

	double TstartTime = xdata_ar[0];

	ChannelsBufferATP_reader.VOLTAGE = voltage;
	ChannelsBufferATP_reader.ATPtime = ATPtime;
	ChannelsBufferATP_reader.ATPtimeStart = ATPtimeStart;
	ChannelsBufferATP_reader.ATPtimeStop = ATPtimeStop;
	ChannelsBufferATP_reader.ATPtimeStep = ATPtimeStep;
	ChannelsBufferATP_reader.TstartTime = TstartTime;

  //Creat a Event Handle
	workHandle_reader  = CreateEvent (NULL, TRUE, FALSE, MapObjATP_WorkEvent_reader); // Manually. no signal
	if (workHandle_reader == NULL) printf("CreateEvent failed (%d)\n", GetLastError());
    
  //Creat a Event Handle
	backHandle_reader  = CreateEvent (NULL, TRUE, FALSE, MapObjATP_BackEvent_reader); // Manually. no signal
	if (backHandle_reader == NULL) printf("CreateEvent failed (%d)\n", GetLastError());
    
  //Create File Mapping for Values to be send from ATP 
	hMapFile_Value_reader = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 32, MapObjATP_Value_reader);
  //Check if creation success
	if(hMapFile_Value_reader == NULL || hMapFile_Value_reader == INVALID_HANDLE_VALUE) 
		error_reporter("Cannot create file mapping:",GetLastError(), false);
	else printf("\nCreate File Mapping VALUE success!\n");
	
  //Map View of File
	pBuf_Value_reader = (LPTSTR)MapViewOfFile(hMapFile_Value_reader, FILE_MAP_ALL_ACCESS, 0, 0, 32);
  //Check if Create success
	if(pBuf_Value_reader==NULL) error_reporter("Cannot Create Map View of File:",GetLastError(), false);
 	
  	CopyMemory((PVOID)pBuf_Value_reader, &ChannelsBufferATP_reader,sizeof(ChannelsBufferATP_reader));

  	if (! SetEvent(workHandle_reader) ) printf("SetEvent failed (%d)\n", GetLastError());
    
    printf("\nSetEvent realizado!\n\nEsperando programa .pnrf reader responder\n");

	timeout_reader  = WaitForSingleObject(backHandle_reader, INFINITE);
	
	if (!ResetEvent(backHandle_reader)) error_reporter("backHandle_reader failed:",GetLastError(), true);
	
  	printf("\nConexao estabelecida com sucesso!\n");

    return;
}

//------------------------------------------------------------------------------

void c_reader_m__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[])
{
	double ATPtime = xin_ar[0];
	double voltage = xin_ar[4];

	ChannelsBufferATP_reader.VOLTAGE = voltage;
	ChannelsBufferATP_reader.ATPtime = ATPtime;

  	CopyMemory((PVOID)pBuf_Value_reader, &ChannelsBufferATP_reader,sizeof(ChannelsBufferATP_reader));

  	if (! SetEvent(workHandle_reader) ) printf("SetEvent failed (%d)\n", GetLastError());
    
	timeout_reader  = WaitForSingleObject(backHandle_reader, INFINITE);

	if (!ResetEvent(backHandle_reader)) error_reporter("backHandle_reader failed:",GetLastError(), true);

	CopyMemory(&ChannelsBufferATP_reader, (PVOID)pBuf_Value_reader, sizeof(ChannelsBufferATP_reader));
	
	xvar_ar[0] = ChannelsBufferATP_reader.CURRENT;
	xout_ar[0] = xvar_ar[0];

    return;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------