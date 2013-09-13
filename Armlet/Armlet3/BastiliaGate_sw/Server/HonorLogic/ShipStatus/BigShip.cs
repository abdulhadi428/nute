using System.Linq;
using HonorInterfaces;

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

        private static readonly string[] Names = { "Атака", "Защита", "Импеллер", "Навигация", "Жизнеобеспечение", "Шлюзы", "Паруса", "Реактор" };

        public override string GetSubsystemName(int subSystemNum)
        {
            return Names[subSystemNum];
        }

        protected override bool IsReactorDamaged
        {
            get { return !GetAllSubsystemsStatus().Single(s => s.SubSystemNum == 7).Repaired; }
        }

        
    }
}
