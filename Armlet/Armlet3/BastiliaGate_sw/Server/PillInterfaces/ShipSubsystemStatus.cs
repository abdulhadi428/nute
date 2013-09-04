namespace HonorInterfaces
{
    public class ShipSubsystemStatus
    {
        public int SubSystemNum { get; set; }
        public RanmaRepairSeverity Severity { get; set; }
        public ushort RepairedStatus { get; set; }

        public bool Repaired
        {
            get { return RepairedStatus == 0; }
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
