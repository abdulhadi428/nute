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
    }
}
