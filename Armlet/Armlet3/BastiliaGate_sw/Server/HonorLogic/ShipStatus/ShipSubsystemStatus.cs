using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HonorLogic.ShipStatus
{
    public class ShipSubsystemStatus
    {
        public int SubSystemNum { get; set; }
        public RanmaRepairSeverity Severity { get; set; }
        public RanmaSubsystemStatus RanmaStatus { get; set; }
    }
}
