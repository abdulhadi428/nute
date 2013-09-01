namespace HonorLogic.ShipStatus
{
    public class LakShip: ShipBase
    {
        protected override int SubsystemsCount
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
    }
}
