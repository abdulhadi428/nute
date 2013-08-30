using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using HonorInterfaces;
using HonorLogic.ShipStatus.RanmaPlates;

namespace HonorLogic.ShipStatus
{
    public class LakShip: ShipBase
    {
        public event Action<ShipSubsystemStatus> SubsystemUpdated;

        private Timer plateCheckerTimer;
        private RanmaPlate lakRanmaPlate;
        private List<ShipSubsystemStatus> subsystems = new List<ShipSubsystemStatus>()
            {
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 0},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 1},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 2},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 3}
            };

        public LakShip()
        {
            plateCheckerTimer = new Timer(checkPlateStatus, null, new TimeSpan(0, 0, 10), new TimeSpan(0, 0, 10));
        }
        public override void InitializeRanmaPlate()
        {
             lakRanmaPlate = new RanmaPlate(PhysicalGateID[0], 4);
        }

        public override int BoardCount
        {
            get
            {
                return 4;
            }
        }

        private void checkPlateStatus(object obj)
        {
            lakRanmaPlate.InitiateUpdatePlateInfo();

            bool SimulatorShouldBeNoticed = false;
            for (int i = 0; i < 4; i++)
            {
                var newSeverity = lakRanmaPlate.plateStatusList.First(a => a.SubSystemNum == i).Severity;
                if (subsystems.First(a => a.SubSystemNum == i).Severity != newSeverity)
                {
                    subsystems.First(a => a.SubSystemNum == i).Severity = newSeverity;
                    //Дергаем ивент для ГУЯ
                    var handler = SubsystemUpdated;
                    if (handler != null)
                    {
                        handler(subsystems.First(a => a.SubSystemNum == i));
                    }
                    SimulatorShouldBeNoticed = true;
                }
            }
            if (SimulatorShouldBeNoticed)
            {
                //Надо бы сказать Рандиру
            }
        }

        public override bool SetSubsystemStatus(ShipSubsystemStatus ranmaStatus)
        {
            if (ranmaStatus.SubSystemNum > 3) { return false; }
            if (ranmaStatus.SubSystemNum < 0) { return false; }

            subsystems[ranmaStatus.SubSystemNum].Severity = ranmaStatus.Severity;
            
            if (ranmaStatus.Severity != RanmaRepairSeverity.Ready)
            {
                var ranmaTable = RanmaSubsystemStatusFactory.GenerateRanmaSubsystemStatus(ranmaStatus.Severity);
                lakRanmaPlate.SetPlateSubsystemInfo(ranmaStatus.SubSystemNum, ranmaStatus.Severity, ranmaTable);
                
            }
            //2) Известить рандира на симуляторе
            return true;
        }

        public override List<ShipSubsystemStatus> GetAllSubsystemsStatus()
        {
            return subsystems;
        }
    }
}
