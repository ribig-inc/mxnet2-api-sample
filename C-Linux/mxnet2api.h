
/*****************************************************************************/
/*  Include File for Visual C/C++ (32- and 64-Bit)                           */
/*****************************************************************************/
#ifndef _MATRIXRTC_API_H
#define _MATRIXRTC_API_H

#include "mxtypes.h" 

typedef struct
{
  _mxINT16 LPT_Nr;
  _mxINT16 LPT_Adr;
  _mxINT16 DNG_Cnt;
} DNGINFO;



#if defined(__APPLE__) || defined(__unix__)
	#ifndef __WATCOMC__
		#define WINAPI
	#endif
#else
   #if !defined(WINAPI)
      #define WINAPI __stdcall
   #endif
#endif

#if !defined(BOOL)
  #define BOOL int
#endif

#if !defined(FALSE)
  #define FALSE 0
#endif

#if !defined(TRUE)
  #define TRUE 1
#endif

#if !defined(LOWORD)
    #define LOWORD(l)           ((_mxINT16)(((_mxINT32)(l)) & 0xffff))
#endif

#if !defined(HIWORD)
    #define HIWORD(l)           ((_mxINT16)((((_mxINT32)(l)) >> 16) & 0xffff))
#endif

#ifdef __cplusplus
extern "C" {
#endif

	_mxINT16  WINAPI rInit_MatrixAPI(void);
	_mxINT16  WINAPI rRelease_MatrixAPI(void);
	_mxINT32  WINAPI rGetVersionAPI(void);
	_mxINT32  WINAPI rGetVersionDRV(void);
	_mxINT32  WINAPI rGetVersionDRV_USB(void);
	_mxINT16  WINAPI rGetPortAdr(_mxINT16);
	_mxINT16  WINAPI rDongle_ReadData(_mxINT32, _mxINT32*, _mxINT16, _mxINT16, _mxINT16);
	_mxINT16  WINAPI rDongle_ReadDataEx(_mxINT32, _mxINT32*, _mxINT16, _mxINT16, _mxINT16, _mxINT16);
	_mxINT32  WINAPI rDongle_ReadSerNr(_mxINT32, _mxINT16, _mxINT16);
	_mxINT16  WINAPI rDongle_Find(void);
	_mxINT32  WINAPI rDongle_Version(_mxINT16, _mxINT16);
	_mxINT32  WINAPI rDongle_Model(_mxINT16, _mxINT16);
	_mxINT16  WINAPI rDongle_MemSize(_mxINT16, _mxINT16);
	_mxINT16  WINAPI rDongle_Count(_mxINT16);

	_mxINT16  WINAPI rDongle_EncryptData(_mxINT32 UserCode, _mxINT32* DataBlock, _mxINT16 DongleNr, _mxINT16 PortNr);
	_mxINT16  WINAPI rDongle_DecryptData(_mxINT32 UserCode, _mxINT32* DataBlock, _mxINT16 DongleNr, _mxINT16 PortNr);

	_mxINT16  WINAPI rSetConfig_MatrixNet(_mxINT16, char*);
	_mxINT16  WINAPI rLogIn_MatrixNet(_mxINT32, _mxINT16, _mxINT16);
	_mxINT16  WINAPI rLogOut_MatrixNet(_mxINT32, _mxINT16, _mxINT16);
	_mxINT32  WINAPI rGetConfig_MatrixNet(_mxINT16 Category);

	_mxINT16 WINAPI rDongle_GetLoginCount(_mxINT32 UserCode, _mxINT16 appSlot, _mxINT16 dNr, _mxINT16 PortNr);
	_mxINT16 WINAPI rDongle_GetLoginCountEx(_mxINT32 UserCode, _mxINT16 appSlot, _mxINT16 dNr, _mxINT16 PortNr, char** ppszClientNames);
	_mxINT32 WINAPI rDongle_GetDetectTime(_mxINT32 UserCode, _mxINT32* detectTime, _mxINT16 dNr, _mxINT16 PortNr);


	char* WINAPI MxNet_SetIP(char* _ip);
	_mxINT32 WINAPI MxNet_SetPort(_mxINT32 _port);
	_mxINT32 WINAPI MxNet_SetTimeOut(_mxINT32 nTimeOut);


#ifdef __cplusplus
}
#endif

#endif  // _MATRIXRTC_API_H

