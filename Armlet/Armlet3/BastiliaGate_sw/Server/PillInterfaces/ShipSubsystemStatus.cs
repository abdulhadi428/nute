using System;

namespace HonorInterfaces
{
    [Serializable]
    public class ShipSubsystemStatus
    {
        public int SubSystemNum { get; set; }
        public RanmaRepairSeverity Severity { get; set; }
    }
}
