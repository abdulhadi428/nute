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

        private static readonly string[] Names = {"Атака", "Защита", "Импеллер", "Навигация", "Паруса", "Шлюзы", "Жизнеобеспечение", "Реактор"};

        public override string GetSubsystemName(int subSystemNum)
        {
            return Names[subSystemNum];
        }
    }
}
