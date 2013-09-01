using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using HonorInterfaces;
using HonorLogic.ShipStatus.RanmaPlates;
using HonorLogic.Storage;
using HonorUtils;

namespace HonorLogic.ShipStatus
{
    public abstract class ShipBase : IShip
    {
        private readonly List<ShipSubsystemStatus> _subsystems = new List<ShipSubsystemStatus>();
        public List<RoomInfo> Rooms { get; set; }
        public abstract int SubsystemsCount { get; }
        protected abstract int PlatesCount { get; }

        public List<byte> ShipRoomsIDs
        {
            get
            {
                return Rooms.Select(r => r.Id).ToList();
            } 
        }

        public  Guid ShipGuid { get; set; }
        public  int[] PhysicalGateID { get; set; }
        public  string Name { get; set; }
        public IGlobalModel Model { get; set; }
        public string GetRoomName(byte roomsID)
        {
            return FindRoomName(roomsID) ?? GetDefaultRoomName(roomsID);
        }

        public event Action<ShipSubsystemStatus> SubsystemUpdated;

        internal static string GetDefaultRoomName(byte roomsID)
        {
            return "Отсек " + roomsID;
        }

        internal string FindRoomName(byte roomsID)
        {
            return (from roomInfo in Rooms where roomInfo.Id == roomsID select roomInfo.Name).SingleOrDefault();
        }

        public void HitRandomRoom(RanmaRepairSeverity severity)
        {
            Model.SendRoomHit(RoomsWithPeople.RandomOrDefault(), GetHitChanceFromSeverity(severity));
        }

        public bool IsOnline
        {
            get
            {
                //TODO Show correct status
                return false;
            }
        }

        private byte GetHitChanceFromSeverity(RanmaRepairSeverity severity)
        {
            return (byte) (((byte) severity - 2)*25 + 50);
        }

        private IEnumerable<byte> RoomsWithPeople
        {
            get
            {
                return Rooms.Select(r => r.Id).Intersect(Model.RoomsWithPeople);
            }
        }

        private void InvokeSubsystemUpdated(ShipSubsystemStatus subsSytem)
        {
//Дергаем ивент для ГУЯ
            var handler = SubsystemUpdated;
            if (handler != null)
            {
                handler(subsSytem);
            }
        }

        private readonly Timer _plateCheckerTimer;
        private RanmaPlate[] _ranmaPlates;

        protected ShipBase()
        {
            _plateCheckerTimer = new Timer(obj => CheckPlateStatus(), null, new TimeSpan(0, 0, 10), new TimeSpan(0, 0, 10));

// ReSharper disable once DoNotCallOverridableMethodsInConstructor
            //SubsystemsCount doesn't rely on dervied constructor status, so this is ok
            for (var i = 0; i < SubsystemsCount; i++)
            {
                _subsystems.Add(new ShipSubsystemStatus {Severity = RanmaRepairSeverity.Ready, SubSystemNum = i});
            }
        }

        public List<ShipSubsystemStatus> GetAllSubsystemsStatus()
        {
            return _subsystems;
        }

        private bool UpdateSubsytemsForPlate(RanmaPlate ranmaPlate)
        {
            var simulatorShouldBeNoticed = false;
            for (var i = 0; i < SubsystemsCount; i++)
            {
                var newSeverity = ranmaPlate.GetSubsystemSeverity(i);
                var subsSytem = _subsystems.First(a => a.SubSystemNum == i);
                if (subsSytem.Severity == newSeverity) continue;
                subsSytem.Severity = newSeverity;
                InvokeSubsystemUpdated(subsSytem);
                simulatorShouldBeNoticed = true;
            }
            return simulatorShouldBeNoticed;
        }

        private void InitializeUpdatePlatesInfo()
        {
            foreach (var bigShipRanmaPlate in _ranmaPlates)
            {
                bigShipRanmaPlate.InitiateUpdatePlateInfo();
            }
        }

        private bool UpdateSubsystemsForPlates()
        {
            var ranmaPlate = _ranmaPlates.First();
            //Plates already synced, so used first
            var simulatorShouldBeNoticed = UpdateSubsytemsForPlate(ranmaPlate);
            return simulatorShouldBeNoticed;
        }

        public bool SetSubsystemStatus(ShipSubsystemStatus ranmaStatus)
        {
            if (ranmaStatus.SubSystemNum >= SubsystemsCount) { return false; }
            if (ranmaStatus.SubSystemNum < 0) { return false; }

            _subsystems[ranmaStatus.SubSystemNum].Severity = ranmaStatus.Severity;
            if (ranmaStatus.Severity != RanmaRepairSeverity.Ready)
            {
                SetSubsytemSeverityToAll(ranmaStatus.SubSystemNum, ranmaStatus.Severity);
            }
            //2) Известить рандира на симуляторе
            return true;
        }


        public void InitializeRanmaPlate()
        {
            Debug.Assert(PlatesCount == PhysicalGateID.Length);
            _ranmaPlates = PhysicalGateID.Select(gateId => new RanmaPlate(gateId, SubsystemsCount)).ToArray();
        }

        public void Dispose()
        {
            if (_plateCheckerTimer != null)
            {
                _plateCheckerTimer.Dispose();
            }
        }

        private void CheckPlateStatus()
        {
            InitializeUpdatePlatesInfo();

            for (var i = 0; i < SubsystemsCount; i ++)
            {
                SyncSubsystem(i);
            }

            var simulatorShouldBeNoticed = UpdateSubsystemsForPlates();
            if (simulatorShouldBeNoticed)
            {
                //Надо бы сказать Рандиру
            }
        }

        private void SetSubsytemSeverityToAll(int i, RanmaRepairSeverity ranmaRepairSeverity)
        {
            foreach (var ranmaPlate in _ranmaPlates)
            {
                ranmaPlate.SetSubsystemSeverity(i, ranmaRepairSeverity);
            }
        }

        private void SyncSubsystem(int i)
        {
            var subsystemCopies = _ranmaPlates.Select(plate => plate.GetSubsystem(i)).ToArray();
            var hasReadyPlate = subsystemCopies.Any(p => p.Severity == RanmaRepairSeverity.Ready);
            var hasBrokenPlate = subsystemCopies.Any(p => p.Severity != RanmaRepairSeverity.Ready);

            if (hasReadyPlate && hasBrokenPlate)
            {
                SetSubsytemSeverityToAll(i, RanmaRepairSeverity.Ready);
            }
        }
    }
}
