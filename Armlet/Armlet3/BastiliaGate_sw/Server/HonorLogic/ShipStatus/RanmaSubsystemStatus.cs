using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HonorLogic.ShipStatus
{
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
