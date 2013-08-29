using System;
using System.Collections.Generic;

namespace HonorInterfaces
{
    public interface IGateModel
    {
        bool PillOnline { get; }
        bool Online { get; }
        IEnumerable<IPillType> PillTypes { get; }
        event Action PillOnlineChanged;
        event Action GateOnlineChanged;
        event Action<byte[]> PillDataArrived;
        void ActivatePin();
        void WritePill(int p, int charges, byte pillAddress = 0x00);
        void RefreshPillStatus(byte pillAddress = 0x00);
    }
}