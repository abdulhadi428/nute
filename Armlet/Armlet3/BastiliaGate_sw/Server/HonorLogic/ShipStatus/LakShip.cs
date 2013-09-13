namespace HonorLogic.ShipStatus
{
    public class LakShip: ShipBase
    {
        protected override int SubsystemsCount
        {
            get
            {
                return 5;
            }
        }

        protected override int PlatesCount
        {
            get { return 1; }
        }

        private static readonly string[] Names = { "Атака", "Защита", "Импеллер", "Навигация", "Жизнеобеспечение", "Шлюзы", "Паруса", "Реактор" };

        public override string GetSubsystemName(int subSystemNum)
        {
            return Names[subSystemNum];
        }

        protected override bool IsReactorDamaged
        {
            get { return false; }
        }
    }
}
