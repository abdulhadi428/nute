using System;
using System.Collections.Generic;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public class BigShip: ShipBase
    {
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
                //1) Прописхнуть в гейт до ранма-панельки
                //2) Известить рандира на симуляторе
            }
            return true;
        }

        public override List<ShipSubsystemStatus> GetAllSubsystemsStatus()
        {
            return subsystems;
        }
    }
}
