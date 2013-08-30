using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using HonorInterfaces;
using HonorLogic.ShipStatus.RanmaPlates;

namespace HonorLogic.ShipStatus
{
    public class BigShip: ShipBase
    {
        private Timer plateCheckerTimer;
        public event Action<ShipSubsystemStatus> SubsystemUpdated;

        private RanmaPlate[] bigShipRanmaPlates;
        private List<ShipSubsystemStatus> subsystems = new List<ShipSubsystemStatus>()
            {
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 0},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 1},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 2},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 3},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 4},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 5},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 6},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 7}
            };

        public BigShip()
        {
            plateCheckerTimer = new Timer(checkPlateStatus, null, new TimeSpan(0, 0, 10), new TimeSpan(0, 0, 10));
        }
        public override void InitializeRanmaPlate()
        {
            bigShipRanmaPlates = new RanmaPlate[] { new RanmaPlate(PhysicalGateID[0], 8), new RanmaPlate(PhysicalGateID[1], 8) };
        }

        private void checkPlateStatus(object obj)
        {
            foreach (var bigShipRanmaPlate in bigShipRanmaPlates)
            {
                bigShipRanmaPlate.InitiateUpdatePlateInfo();
            }

            for (int i = 0; i < 8; i ++)
            {
                var firstPlate = bigShipRanmaPlates[0].plateStatusList.First(a => a.SubSystemNum == i);
                var secondPlate = bigShipRanmaPlates[1].plateStatusList.First(a => a.SubSystemNum == i);
                if (((firstPlate.Severity == RanmaRepairSeverity.Ready) &&
                     (secondPlate.Severity != RanmaRepairSeverity.Ready)) ||
                    ((firstPlate.Severity != RanmaRepairSeverity.Ready) &&
                     (secondPlate.Severity == RanmaRepairSeverity.Ready)))
                {
                    bigShipRanmaPlates[0].SetPlateSubsystemInfo(i, RanmaRepairSeverity.Ready, new RanmaSubsystemTable(0));
                    bigShipRanmaPlates[1].SetPlateSubsystemInfo(i, RanmaRepairSeverity.Ready, new RanmaSubsystemTable(0));
                }
            }

            bool SimulatorShouldBeNoticed = false;
            for (int i = 0; i < 8; i++)
            {
                var newSeverity = bigShipRanmaPlates[0].plateStatusList.First(a => a.SubSystemNum == i).Severity;
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

        public override int BoardCount
        {
            get
            {
                return 8;
            }
        }
        
        public override bool SetSubsystemStatus(ShipSubsystemStatus ranmaStatus)
        {
            if (ranmaStatus.SubSystemNum > 7) { return false; }
            if (ranmaStatus.SubSystemNum < 0) { return false; }

            subsystems[ranmaStatus.SubSystemNum].Severity = ranmaStatus.Severity;
            if (ranmaStatus.Severity != RanmaRepairSeverity.Ready)
            {
                var ranmaTable = RanmaSubsystemStatusFactory.GenerateRanmaSubsystemStatus(ranmaStatus.Severity);
                foreach (var bigShipRanmaPlate in bigShipRanmaPlates)
                {
                    bigShipRanmaPlate.SetPlateSubsystemInfo(ranmaStatus.SubSystemNum, ranmaStatus.Severity, ranmaTable);
                }
                
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
