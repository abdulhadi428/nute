using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using HonorInterfaces;
using HonorLogic.ShipStatus.Ranma;
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

        public bool IsOnline
        {
            get
            {
                return _ranmaPlates.All(p => p.Online   );
            }
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
                _subsystems.Add(new ShipSubsystemStatus {Severity = RanmaRepairSeverity.NotDamaged, SubSystemNum = i});
            }
        }

        public List<ShipSubsystemStatus> GetAllSubsystemsStatus()
        {
            return _subsystems;
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
            var simulatorShouldBeNoticed = false;
            for (var i = 0; i < SubsystemsCount; i++)
            {
                var subsystem = _subsystems.First(a => a.SubSystemNum == i);
                var severityFromPlate = _ranmaPlates.First()[i].Severity;
                var repairedStatusFromPlate = _ranmaPlates.Any(plate => plate[i].Repaired);

                if (subsystem.Severity == severityFromPlate && subsystem.Repaired == repairedStatusFromPlate)
                    continue;
                subsystem.Severity = severityFromPlate;
                subsystem.RepairedStatus = _ranmaPlates.First()[i].RepairedStatus;
                InvokeSubsystemUpdated(subsystem);
                simulatorShouldBeNoticed = true;
            }
            return simulatorShouldBeNoticed;
        }

        public void SetSubsystemStatusFromGUI(ShipSubsystemStatus ranmaStatus)
        {
            Debug.Assert(ranmaStatus.SubSystemNum < SubsystemsCount);
            Debug.Assert(ranmaStatus.SubSystemNum >= 0);

            SendSeverityToRanmaPlate(ranmaStatus);

            SaveToSimulator();
        }

        private void SaveToSimulator()
        {
            NetworkLevel.NetworkDeliveryLevel.NetworkDelivery.ShipDamageServiceInstance.SendToSimulator(this.ShipGuid, _subsystems);
        }

        private void SendSeverityToRanmaPlate(ShipSubsystemStatus ranmaStatus)
        {
            _subsystems[ranmaStatus.SubSystemNum].Severity = ranmaStatus.Severity;

            SetSubsytemSeverityToAll(ranmaStatus.SubSystemNum, ranmaStatus.Severity);
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

            var simulatorShouldBeNoticed = UpdateSubsystemsForPlates();
            if (simulatorShouldBeNoticed)
            {
                SaveToSimulator();
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

        public void DamageShip(byte damage)
        {
            for (var i = 0; i < damage; i++)
            {
                DamageShipOnce();
            }
            SaveToSimulator();
            var targetRoom = RoomsWithPeople.RandomOrDefault();
            if (targetRoom != 0)
            {
                Model.SendRoomHit(targetRoom, (byte) ((damage - 2)*25 + 50), HitType.Random);
            }
        }

        private void DamageShipOnce()
        {
            var targetSubsystem = TargetSubsystems.Random();
            targetSubsystem.Severity += 1;
            SendSeverityToRanmaPlate(targetSubsystem);
        }

        private IEnumerable<ShipSubsystemStatus> TargetSubsystems
        {
            get
            {
                return _subsystems.Where(s => s.Severity != RanmaRepairSeverity.Max);
            }
        }

        public void DestroyShip()
        {
            throw new NotImplementedException();
        }

        public abstract string GetSubsystemName(int subSystemNum);

        public void ApplyEffects()
        {
            ReactorEffect();
        }

        private void ReactorEffect()
        {
            if (IsReactorDamaged)
            {
                foreach (var reactorRoom in ReactorRooms)
                {
                    Model.SendRoomHit(reactorRoom, 10, HitType.Radiation);
                }
            }
        }

        protected abstract bool IsReactorDamaged { get; }

        public IEnumerable<byte> ReactorRooms
        {
            get
            {
                return Rooms.Where(r => r.IsReactor).Select(r => r.Id);
            }
        }
    }
}
