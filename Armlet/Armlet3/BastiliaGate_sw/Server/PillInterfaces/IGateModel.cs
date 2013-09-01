using System;
using System.Collections.Generic;

namespace HonorInterfaces
{
    public interface IGateModel
    {
        bool DeviceOnline { get; }
        bool Online { get; }
        IEnumerable<IPillType> PillTypes { get; }
        event Action DeviceOnlineChanged;
        event Action GateOnlineChanged;
        event Action<byte[]> DeviceDataArrived;
        void ActivatePin();
        void WritePill(int p, int charges, byte pillAddress = 0x00);
        void RefreshPillStatus(byte pillAddress = 0x00);
        void ReadPlate();
        void WritePlate(byte pillAddress, int subSystemNum, byte[] repairTable);
    }
}