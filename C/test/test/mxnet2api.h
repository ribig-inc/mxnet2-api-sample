#ifndef _MXNET2API_H
#define _MXNET2API_H

#include <windows.h>
#include "mxtypes.h"     // platform independent data types

#ifdef __cplusplus
extern "C" {
#endif


_mxINT16  __stdcall rInit_MatrixAPI(HINSTANCE hInst=NULL);
_mxINT16  __stdcall rRelease_MatrixAPI(void);
_mxINT32  __stdcall rGetVersionAPI(void);
_mxINT32  __stdcall rGetVersionDRV(void);
_mxINT32  __stdcall rGetVersionDRV_USB(void);
void      __stdcall rSetW95Access(_mxINT16);
_mxINT16  __stdcall rGetPortAdr(_mxINT16);
_mxINT16  __stdcall rDongle_WriteData(_mxINT32, _mxINT32*, _mxINT16, _mxINT16, _mxINT16);
_mxINT16  __stdcall rDongle_WriteDataEx(_mxINT32, _mxINT32*, _mxINT16, _mxINT16, _mxINT16, _mxINT16);
_mxINT16  __stdcall rDongle_ReadData(_mxINT32, _mxINT32*, _mxINT16, _mxINT16, _mxINT16);
_mxINT16  __stdcall rDongle_ReadDataEx(_mxINT32, _mxINT32*, _mxINT16, _mxINT16, _mxINT16, _mxINT16);
_mxINT32  __stdcall rDongle_ReadSerNr(_mxINT32, _mxINT16, _mxINT16);
_mxINT16  __stdcall rDongle_Exit(void);
_mxINT16  __stdcall rDongle_Find(void);
_mxINT32  __stdcall rDongle_Version(_mxINT16, _mxINT16);
_mxINT32  __stdcall rDongle_Model(_mxINT16, _mxINT16);
_mxINT16  __stdcall rDongle_MemSize(_mxINT16, _mxINT16);
_mxINT16  __stdcall rDongle_Count(_mxINT16);
_mxINT16  __stdcall rPausePrinterActivity(void);
_mxINT16  __stdcall rResumePrinterActivity(void);
_mxINT16  __stdcall rSetConfig_MatrixNet(_mxINT16, char*);
_mxINT16  __stdcall rLogIn_MatrixNet(_mxINT32, _mxINT16, _mxINT16);
_mxINT16  __stdcall rLogOut_MatrixNet(_mxINT32, _mxINT16, _mxINT16);
_mxINT32  __stdcall rGetConfig_MatrixNet(_mxINT16 Category);

_mxINT16  __stdcall rDongle_WriteKey(_mxINT32 UserCode, 
                                    _mxINT32 *KeyData, 
                                    _mxINT16 DongleNr, 
                                    _mxINT16 PortNr);

_mxINT16  __stdcall rDongle_GetKeyFlag(_mxINT32 UserCode, _mxINT16 DongleNr, _mxINT16 PortNr);

_mxINT16  __stdcall rDongle_EncryptData(_mxINT32 UserCode, _mxINT32 *DataBlock, _mxINT16 DongleNr, _mxINT16 PortNr);
_mxINT16  __stdcall rDongle_DecryptData(_mxINT32 UserCode, _mxINT32 *DataBlock, _mxINT16 DongleNr, _mxINT16 PortNr);

_mxINT16  __stdcall rDongle_SetDriverFlag(_mxINT32 UserCode, _mxINT16 DriverFlag, _mxINT16 DongleNr, _mxINT16 PortNr);
_mxINT16  __stdcall rDongle_GetDriverFlag(_mxINT32 UserCode, _mxINT16 DongleNr, _mxINT16 PortNr);

WCHAR*   __stdcall MxNet_SetIP(WCHAR*);
_mxINT32 __stdcall MxNet_SetPort(_mxINT32);
_mxINT32 __stdcall MxNet_SetTimeOut(_mxINT32);

_mxINT32 __stdcall rDongle_GetDetectTime(_mxINT32 UserCode, _mxINT32* timeBuffer, _mxINT16 dNr, _mxINT16 pNr);
_mxINT32 WINAPI rDongle_GetLoginCount(_mxINT32 UserCode, _mxINT16 appSlot, _mxINT16 dNr, _mxINT16 PortNr);
_mxINT32 WINAPI rDongle_GetLoginCountEx(_mxINT32 UserCode, _mxINT16 appSlot, _mxINT16 dNr, _mxINT16 PortNr, char** ppszClientNames);

void  __stdcall rReset_MatrixAPI(void);

#ifdef __cplusplus
}
#endif


#endif  // _MXNET2API_H

