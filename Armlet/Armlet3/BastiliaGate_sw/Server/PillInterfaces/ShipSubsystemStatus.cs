using System;

namespace HonorInterfaces
{
    public class ShipSubsystemStatus
    {
        public int SubSystemNum { get; set; }
        public RanmaRepairSeverity Severity { get; set; }
        public ushort RepairedStatus { get; set; }
    }
}
