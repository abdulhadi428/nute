using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public class LakShip: ShipBase
    {
        private List<ShipSubsystemStatus> subsystems = new List<ShipSubsystemStatus>()
            {
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 0},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 1},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 2},
                new ShipSubsystemStatus() {Severity = RanmaRepairSeverity.Ready, SubSystemNum = 3}
            };

        public override int BoardCount
        {
            get
            {
                return 4;
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
                //1) Прописхнуть в гейт до ранма-панельки
                
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
