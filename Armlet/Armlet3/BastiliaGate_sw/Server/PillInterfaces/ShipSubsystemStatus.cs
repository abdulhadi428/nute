using System;

namespace PillInterfaces
{
    [Serializable]
    public class ShipSubsystemStatus
    {
        public int SubSystemNum { get; set; }
        public RanmaRepairSeverity Severity { get; set; }
    }
}
