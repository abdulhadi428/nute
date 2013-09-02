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
        public event Action OnlineChanged;
        private readonly List<ShipSubsystemStatus> _subsystems = new List<ShipSubsystemStatus>();
        public List<RoomInfo> Rooms { get; set; }
        protected abstract int SubsystemsCount { get; }
        protected abstract int PlatesCount { get; }

        public IEnumerable<byte> ShipRoomsIDs
        {
            get
            {
                return Rooms.Select(r => r.Id).ToList();
            } 
        }

        public string Name
        {
            get
            {
                return string.Format("{0} (гейт{1} {2})", ShipName, PhysicalGateID.Length > 1 ? "ы" : "",
                    string.Join(", ", PhysicalGateID));
            }
        }

        public  Guid ShipGuid { get; set; }
        public  byte[] PhysicalGateID { get; set; }
        public  string ShipName { get; set; }
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
                return _ranmaPlates.All(p => p.Online   );
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
                bigShipRanmaPlate.Refresh();
            }
        }

        private bool UpdateSubsystemsForPlates()
        {
            //Plates already synced, so use first
            return UpdateSubsytemsForPlate(_ranmaPlates.First());
        }

        public void SetSubsystemStatus(ShipSubsystemStatus ranmaStatus)
        {
            Debug.Assert(ranmaStatus.SubSystemNum < SubsystemsCount);
            Debug.Assert(ranmaStatus.SubSystemNum >= 0);

            _subsystems[ranmaStatus.SubSystemNum].Severity = ranmaStatus.Severity;
            
            SetSubsytemSeverityToAll(ranmaStatus.SubSystemNum, ranmaStatus.Severity);
            
            //2) Известить рандира на симуляторе
        }


        public void InitializeRanmaPlate()
        {
            Debug.Assert(PlatesCount == PhysicalGateID.Length);
            _ranmaPlates = PhysicalGateID.Select(CreateRanmaPlate).ToArray();
        }

        private RanmaPlate CreateRanmaPlate(byte gateId)
        {
            var ranmaPlate = new RanmaPlate(SubsystemsCount, Model.GetGateModel(gateId));
            ranmaPlate.OnlineChanged += RaiseOnlineChanged;
            return ranmaPlate;
        }

        private void RaiseOnlineChanged()
        {
            var handler = OnlineChanged;
            if (handler != null)
            {
                handler();
            }
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
            if (!IsOnline)
            {
                return;
            }
            for (var i = 0; i < SubsystemsCount; i ++)
            {
                SyncSubsystem(i);
            }

            var simulatorShouldBeNoticed = UpdateSubsystemsForPlates();
            if (simulatorShouldBeNoticed)
            {
                //Надо бы сказать Рандиру
            }

            InitializeUpdatePlatesInfo();
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
            var severities = _ranmaPlates.Select(plate => plate.GetSubsystemSeverity(i)).ToArray();
            var hasReadyPlate = severities.Any(p => p == RanmaRepairSeverity.Ready);
            var hasBrokenPlate = severities.Any(p => p != RanmaRepairSeverity.Ready);

            if (hasReadyPlate && hasBrokenPlate)
            {
                SetSubsytemSeverityToAll(i, RanmaRepairSeverity.Ready);
            }
        }
    }
}
