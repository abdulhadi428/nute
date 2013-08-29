using System;
using System.Linq;

namespace HonorInterfaces
{
    
    [Serializable]
    public class RanmaSubsystemStatus
    {
        private byte[] _bytes = { 0x03, 0x03 };

        public byte[] Bytes
        {
            get { return _bytes; }
        }

        public RanmaSubsystemStatus(ushort logicTable)
        {
            var bytesBeforeEndianCheck = BitConverter.GetBytes(logicTable);
            if ( BitConverter.IsLittleEndian == false)
                _bytes = bytesBeforeEndianCheck;
            else
                _bytes = (byte[])bytesBeforeEndianCheck.Reverse().ToArray();
        }
    }
}
