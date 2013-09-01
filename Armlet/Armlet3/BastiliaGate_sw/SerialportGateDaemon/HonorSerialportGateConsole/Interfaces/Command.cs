using System;
using System.IO;
using System.Text;

namespace HonorSerialportGateConsole.Interfaces
{
    public abstract class Command
    {
        public static string ByteArrayToHexString(byte[] ba)
        {
            StringBuilder hex = new StringBuilder(ba.Length * 2);
            foreach (byte b in ba)
                hex.AppendFormat("{0:X2}", b);
            return hex.ToString();
        }

        public static string SanitiseStringFromComas(String inputString)
        {
            
            return inputString.Replace(",", "").Trim(new char[]{'#'});
        }

        public static byte[] HexStringToByteArray(String hexString)
        {
            int NumberChars = hexString.Length / 2;
            byte[] bytes = new byte[NumberChars];
            StringReader sr = new StringReader(hexString);
            for (int i = 0; i < NumberChars; i++)
                bytes[i] = Convert.ToByte(new string(new char[2] { (char)sr.Read(), (char)sr.Read() }), 16);
            sr.Dispose();
            return bytes;
        }

        public byte CommandCodeByte { get; protected set; }                        //One of 4 enums
        public byte[] Data { get; protected set; }                            //Data payload (0-252 bytes)
        
        public string ConvertToString()
        {
            return "#" + ByteArrayToHexString(new byte[] {CommandCodeByte}) + ByteArrayToHexString(Data);
        }
        
    }
}
