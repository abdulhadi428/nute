using System;

namespace HonorInterfaces
{
    public interface IArmletInfo
    {
        byte Id { get; }
        string RoomName { get; }
        byte BloodLevel { get; }
        string Name { get; }
        string Status { get; }
        byte Regen { get; }
        byte Toxic { get; }
        byte Pulse { get; }
        byte Temp { get; }
        void SetStatus(string status);
        void SetName(string text);
        event Action ArmletUpdated;
        void SetRegen(byte regen);
    }
}