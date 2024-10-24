/*****************************************************************************/
/*  MXAPI.CS   MS-WINDOWS Win32 (95/98/ME/NT/2K/XP)                          */
/*                                                                           */
/*  (C) TDi GmbH                                                             */
/*                                                                           */
/*  Defines to acces the Matrix API in C#                                    */
/*****************************************************************************/

namespace MXAPI
{
    using System;
    using System.Runtime.InteropServices;   /* Required namespace for the DllImport method */
    using System.Text;

	public struct DNGINFO
	{
		public short LPT_Nr;
		public short LPT_Adr;
		public short DNG_Cnt;
	};

	public class Matrix
	{
		/* This c#-class will import the Matrix API classes */	
		
		[DllImport("mxnet2_api.DLL", EntryPoint="rInit_MatrixAPI", CallingConvention = CallingConvention.StdCall)]
		public static extern short rInit_MatrixAPI();

		[DllImport("mxnet2_api.DLL", EntryPoint="rRelease_MatrixAPI", CallingConvention = CallingConvention.StdCall)]
		public static extern short rRelease_MatrixAPI();

		[DllImport("mxnet2_api.DLL", EntryPoint="rGetVersionAPI", CallingConvention = CallingConvention.StdCall)]
		public static extern int rGetVersionAPI();

		[DllImport("mxnet2_api.DLL", EntryPoint="rGetVersionDRV_USB", CallingConvention = CallingConvention.StdCall)]
		public static extern int rGetVersionDRV_USB();

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_Find", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_Find();

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_Version", CallingConvention = CallingConvention.StdCall)]
		public static extern int rDongle_Version(short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_Model", CallingConvention = CallingConvention.StdCall)]
		public static extern int rDongle_Model(short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_MemSize", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_MemSize(short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_Count", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_Count(short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_ReadData", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_ReadData(int UserCode, int[] Data, short Count, short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_ReadDataEx", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_ReadDataEx(int UserCode, ref int Data, short Fpos, short Count, short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_ReadSerNr", CallingConvention = CallingConvention.StdCall)]
		public static extern int rDongle_ReadSerNr(int UserCode, short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_WriteData", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_WriteData(int UserCode, ref int Data, short Count, short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_WriteDataEx", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_WriteDataEx(int UserCode, ref int Data, short Fpos, short Count, short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_WriteKey", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_WriteKey(int UserCode, ref int KeyData, short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_GetKeyFlag", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_GetKeyFlag(int UserCode, short DngNr, short Port);

        [DllImport("mxnet2_api.DLL", EntryPoint = "rSetConfig_MatrixNet", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
		public static extern short rSetConfig_MatrixNet(short nAccess, string nFile);

		[DllImport("mxnet2_api.DLL", EntryPoint="rGetConfig_MatrixNet", CallingConvention = CallingConvention.StdCall)]
		public static extern int rGetConfig_MatrixNet(short Category);

		[DllImport("mxnet2_api.DLL", EntryPoint="rLogIn_MatrixNet", CallingConvention = CallingConvention.StdCall)]
		public static extern short rLogIn_MatrixNet(int UserCode, short AppSlot, short DngNr);

		[DllImport("mxnet2_api.DLL", EntryPoint="rLogOut_MatrixNet", CallingConvention = CallingConvention.StdCall)]
		public static extern short rLogOut_MatrixNet(int UserCode, short AppSlot, short DngNr);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_EncryptData", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_EncryptData(int UserCode, ref int DataBlock, short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="rDongle_DecryptData", CallingConvention = CallingConvention.StdCall)]
		public static extern short rDongle_DecryptData(int UserCode, ref int DataBlock, short DngNr, short Port);

		[DllImport("mxnet2_api.DLL", EntryPoint="MxNet_SetPort", CallingConvention = CallingConvention.StdCall)]
		public static extern int MxNet_SetPort(int port);

        [DllImport("mxnet2_api.DLL", EntryPoint = "rDongle_GetDetectTime", CallingConvention = CallingConvention.StdCall)]
        public static extern int rDongle_GetDetectTime(int UserCode, int[] detectTime, short DngNr, short Port);

        [DllImport("mxnet2_api.DLL", EntryPoint = "rDongle_GetLoginCount", CallingConvention = CallingConvention.StdCall)]
        public static extern short rDongle_GetLoginCount(int UserCode, short appSlot, short DngNr, short Port);

        [DllImport("mxnet2_api.DLL", EntryPoint = "rDongle_GetLoginCountEx", CallingConvention = CallingConvention.StdCall)]
        public static extern short rDongle_GetLoginCountEx(int UserCode, short appSlot, short DngNr, short Port, out string ppszClientNames );

        [DllImport("mxnet2_api.DLL", EntryPoint = "rReset_MatrixAPI", CallingConvention = CallingConvention.StdCall)]
        public static extern void rReset_MatrixAPI();


   }
}

