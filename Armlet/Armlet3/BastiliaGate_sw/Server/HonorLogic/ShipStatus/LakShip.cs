using System.Diagnostics;

namespace HonorLogic.ShipStatus
{
    public class LakShip: ShipBase
    {

        public override int SubsystemsCount
        {
            get
            {
                return 4;
            }
        }

        protected override int PlatesCount
        {
            get { return 1; }
        }

        protected override void SyncSubsystem(int i)
        {
            Debug.Assert(PlatesCount == 1);
            //Doesn't need to sync plates
        }
    }
}
