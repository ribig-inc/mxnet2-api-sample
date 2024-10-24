/**************************************************************/
/* MATRIX.JAVA                                                */
/* Matrix Class Implementation for Java                       */
/*                                                            */
/* Copyright (C) TDi GmbH                                     */
/**************************************************************/

//-------------------------------------------------------------
// If you need to use class packaging, uncomment the following 
// line, but do not change the package name.
// Using class packaging, the Matrix.class have to be placed 
// in: CLASSPATH/de/matrixlock
//-------------------------------------------------------------
//package de.matrixlock;


import java.io.IOException;

public class MxNet2   // Do not change this Class name
{

    private static String libName;

    static {
        libName = System.mapLibraryName("mxnet2api");
        try {
            System.loadLibrary("mxnet2api");
        }
        catch(UnsatisfiedLinkError e) {
            System.err.println("Unable to load library [" + libName + "]");
            throw e;
        }
    }

    public static native short rInit_MatrixAPI();

    public static native short rRelease_MatrixAPI();

    public static native int rGetVersionAPI();

    public static native int rGetVersionDRV();

    public static native int rGetVersionDRV_USB();

    public static native void rSetW95Access(short modus);

    public static native short rGetPortAdr(short p);

    public static native short rDongle_Count(short PortNr);

    public static native int rDongle_Version(short DongleNr, short PortNr);

    public static native int rDongle_Model(short DongleNr, short PortNr);

    public static native short rDongle_MemSize(short DongleNr, short PortNr);

    public static native short rDongle_ReadData(int UserCode, int[] pDataIn, short Count,
                                                             short DongleNr, short PortNr);

    public static native short rDongle_ReadDataEx(int UserCode, int[] pDataIn, short Fpos, short Count,
                                                               short DongleNr, short PortNr);

    public static native int rDongle_ReadSerNr(int UserCode, short DongleNr, short PortNr);

    public static native short rDongle_WriteData(int UserCode, int[] pDataOut, short Count,
                                                              short DongleNr, short PortNr);

    public static native short rDongle_WriteDataEx(int UserCode, int[] DataOut, short Fpos, short Count,
                                                                short DongleNr, short PortNr);

    public static native short rDongle_WriteKey(int UserCode, int[] pKeyData, short DongleNr, short PortNr);

    public static native short rDongle_GetKeyFlag(int UserCode, short DongleNr, short PortNr);

    public static native short rDongle_EncryptData(int UserCode, int[] pDataBlock,
                                                                short DongleNr, short PortNr);

    public static native short rDongle_DecryptData(int UserCode, int[] pDataBlock,
                                                                short DongleNr, short PortNr);

    
    public static native short rSetConfig_MatrixNet(short nAccess, String nFile);

    public static native int rGetConfig_MatrixNet(short Category);

    public static native short rLogIn_MatrixNet(int UserCode, short AppSlot, short DongleNr);

    public static native short rLogOut_MatrixNet(int UserCode, short AppSlot, short DongleNr);

    public static native void MxNet_SetIP(String ip);
    public static native int  MxNet_SetPort(int port);

}

