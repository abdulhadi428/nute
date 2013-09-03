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

        private static readonly string[] Names = { "Атака", "Защита", "Импеллер", "Навигация", "Паруса", "Шлюзы", "Жизнеобеспечение", "Реактор" };

        public override string GetSubsystemName(int subSystemNum)
        {
            return Names[subSystemNum];
        }
    }
}
