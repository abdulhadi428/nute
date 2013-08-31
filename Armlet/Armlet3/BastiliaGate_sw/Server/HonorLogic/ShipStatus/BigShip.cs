using System.Diagnostics;
using System.Linq;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public class BigShip: ShipBase
    {
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
