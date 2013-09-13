namespace HonorInterfaces
{
    public class ShipSubsystemStatus
    {
        public int SubSystemNum { get; set; }
        public bool IsFirstLineOfDefence { get; set; }
        public RanmaRepairSeverity Severity { get; set; }
        public ushort EffectiveTable { get; set; }
        public ushort RealTable { get; set; }

        public bool Repaired
        {
            get { return EffectiveTable == 0; }
        }

        public RanmaRepairSeverity ResultSeverity
        {
            get
            {
                return Repaired ? RanmaRepairSeverity.NotDamaged : Severity;
            }
        }
    }
}
