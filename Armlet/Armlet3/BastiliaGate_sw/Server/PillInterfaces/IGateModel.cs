using System;
using System.Collections.Generic;

namespace PillInterfaces
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
        void WritePill(int p, int charges);
        void RefreshPillStatus();
    }
}