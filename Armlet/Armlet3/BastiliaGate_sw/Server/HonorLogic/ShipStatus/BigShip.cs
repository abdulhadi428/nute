namespace HonorLogic.ShipStatus
{
    public class BigShip: ShipBase
    {
        protected override int SubsystemsCount
        {
            get { return 8; }
        }

        protected override int PlatesCount
        {
            get { return 2; }
        }
    }
}
