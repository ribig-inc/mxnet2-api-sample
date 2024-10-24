/*****************************************************************************/
/* MXTEA.CS  -  XTEA Encryption/Decryption Algorithm                         */
/*                                                                           */
/* (C) TDi GmbH                                                              */
/*                                                                           */
/* Implementation for C#                                                     */
/*****************************************************************************/
/* The 'Extended Tiny Encryption Algorithm' (XTEA) by David Wheeler and      */
/* Roger Needham of the Cambridge Computer Laboratory.                       */
/* XTEA is a Feistel cipher with XOR and AND addition as the non-linear      */
/* mixing functions.                                                         */
/* Takes 64 bits (8 Bytes block) of data in Data[0] and Data[1].             */
/* Returns 64 bits of encrypted data in Data[0] and Data[1].                 */
/* Takes 128 bits of key in Key[0] - Key[3].                                 */
/*****************************************************************************/

using System;

namespace MATRIX_ENCRYPT
{
    /// <summary>
    /// Summary description for MxCrypt.
    /// </summary>
    public class MxCrypt
    {
        // Ctor
        public MxCrypt()
        {
        }

        static public void MxApp_Encrypt(uint[] Data, uint[] Key)
        {
		unchecked
		{
		   uint y = Data[0];
		   uint z = Data[1];
		   uint sum   = 0;
		   uint delta = 0x9E3779B9;
		   uint n=32;

		   while(n-->0)
		   {
			Data[0] += ((Data[1]<<4 ^ Data[1]>>5) + Data[1]) ^ (sum + Key[sum&3]);
			sum += delta;
			Data[1] += ((Data[0]<<4 ^ Data[0]>>5) + Data[0]) ^ (sum + Key[sum>>11 & 3]);
		   }
		}
        }

        static public void MxApp_Decrypt(uint[] Data, uint[] Key)
        {
		unchecked
		{
		   uint y = Data[0];
		   uint z = Data[1];
		   uint sum   = 0xC6EF3720;
		   uint delta = 0x9E3779B9;
		   uint n=32;

		   while(n-->0)
		   {
			Data[1] -= ((Data[0]<<4 ^ Data[0]>>5) + Data[0]) ^ (sum + Key[sum>>11 & 3]);
			sum -= delta;
			Data[0] -= ((Data[1]<<4 ^ Data[1]>>5) + Data[1]) ^ (sum + Key[sum&3]);
		   }
		}
        }
    }
}


