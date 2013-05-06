using System;

namespace PillInterfaces
{
    public interface IArmletInfo
    {

        byte Id { get; }
        byte Room { get; }
        byte BloodLevel { get; }
        string Name { get; }
        string Status { get; }
        byte Regen { get; }
        byte Toxic { get; }
        void SetStatus(string status);
        void SetName(string text);
        event Action ArmletUpdated;
        void SetRegen(byte regen);
        void SetToxic(byte toxic);
    }
}