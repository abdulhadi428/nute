using System.Diagnostics;
using System.Linq;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public class BigShip: ShipBase
    {
        protected override void SyncSubsystem(int i)
        {
            Debug.Assert(PlatesCount == 2);

            var firstPlate = RanmaPlates[0].plateStatusList.First(a => a.SubSystemNum == i);
            var secondPlate = RanmaPlates[1].plateStatusList.First(a => a.SubSystemNum == i);
            if (((firstPlate.Severity == RanmaRepairSeverity.Ready) &&
                 (secondPlate.Severity != RanmaRepairSeverity.Ready)) ||
                ((firstPlate.Severity != RanmaRepairSeverity.Ready) &&
                 (secondPlate.Severity == RanmaRepairSeverity.Ready)))
            {
                RanmaPlates[0].SetPlateSubsystemInfo(i, RanmaRepairSeverity.Ready, new RanmaSubsystemTable(0));
                RanmaPlates[1].SetPlateSubsystemInfo(i, RanmaRepairSeverity.Ready, new RanmaSubsystemTable(0));
            }
        }

        public override int SubsystemsCount
        {
            get { return 8; }
        }

        protected override int PlatesCount
        {
            get { return 2; }
        }
    }
}
