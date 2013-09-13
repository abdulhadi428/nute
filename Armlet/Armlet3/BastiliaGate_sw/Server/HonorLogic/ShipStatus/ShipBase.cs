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
        private static bool[] FirstLineOfDefenceMask = { true, true, true, false, false, true, true, false };
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

        public void DamageRoom(byte roomID, byte percentage, byte hit_type)
        {
            Model.SendRoomHit(roomID, percentage, (HitType)hit_type);
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

        private IEnumerable<IArmletInfo> PeopleOnThisShip
        {
            get { return Model.PeopleIDsInCertainRooms(Rooms.Select(r => r.Id)); }
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
                _subsystems.Add(new ShipSubsystemStatus
                    {
                        Severity = RanmaRepairSeverity.NotDamaged,
                        SubSystemNum = i,
                        IsFirstLineOfDefence = FirstLineOfDefenceMask[i]
                    });
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
                {
                    subsystem.EffectiveTable = GetEffectiveTable(repairedStatusFromPlate, i);
                    continue;
                };

                subsystem.Severity = severityFromPlate;
                subsystem.EffectiveTable = GetEffectiveTable(repairedStatusFromPlate, i);

                subsystem.RealTable = _ranmaPlates.First()[i].RealTable;
                InvokeSubsystemUpdated(subsystem);
                simulatorShouldBeNoticed = true;
            }
            return simulatorShouldBeNoticed;
        }

        private ushort GetEffectiveTable(bool repairedStatusFromPlate, int i)
        {
            return repairedStatusFromPlate ? (ushort) 0 : _ranmaPlates.First()[i].EffectiveTable;
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
            _subsystems[ranmaStatus.SubSystemNum].EffectiveTable = 0x1111;

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

        private void SetSubsytemSeverityToAll(int subSystemNum, RanmaRepairSeverity ranmaRepairSeverity)
        {
            var repairTable = RanmaSubsystemStatusFactory.GenerateRanmaSubsystemStatus(ranmaRepairSeverity).Bytes;
            foreach (var ranmaPlate in _ranmaPlates)
            {
                ranmaPlate.SetSubsystemSeverity(subSystemNum, ranmaRepairSeverity, repairTable);
            }
        }

        public void DamageShip(byte damage)
        {
            
            ShipSubsystemStatus subSystemToDamage;
            
            //Выбираем какую подсистему поразить
            if (ShipIsUnderFrontLineProtection)   //Если "передние" системы попрежнему держатся, то выбираем из них
            {
                subSystemToDamage = _subsystems.Where(a => a.IsFirstLineOfDefence == true).Random();
            }
            else                                //Ну а если нет - то выбираем из любых
            {
                subSystemToDamage = _subsystems.Random();
            }

            byte reducedDamage = damage;
            if (!subSystemToDamage.IsFirstLineOfDefence)  //Если мы мочим "прикрытые" системы, то damage равен самой пробитой системе из передних
            {
                reducedDamage = Math.Min(damage, _subsystems.Where(a => a.IsFirstLineOfDefence == true).Select(a => (byte) a.ResultSeverity).Max());
            }

            byte finalDamage = Math.Max(reducedDamage, (byte) subSystemToDamage.Severity);
            subSystemToDamage.Severity = (RanmaRepairSeverity)finalDamage;
            SendSeverityToRanmaPlate(subSystemToDamage);
           
            
            
            var targetRoom = RoomsWithPeople.RandomOrDefault();
            if (targetRoom != 0)
            {
                byte hitProbability = 0;
                if (damage == 1) hitProbability = 10;
                if (damage == 2) hitProbability = 20;
                if (damage == 3) hitProbability = 50;
                Model.SendRoomHit(targetRoom, hitProbability, HitType.Random);
            }

            var shipPeople = PeopleOnThisShip.ToArray();
            if (shipPeople.Any())
            {
                foreach (var armletInfo in shipPeople)
                {
                    Model.SendShowMessage(armletInfo,
                                      "Ощутимо тряхнуло! Кажется в корабль влетела ракета!");
                }
            }

            SaveToSimulator();
        }

       

       

        public void DestroyShip()
        {
            DamageShip(3);
            
        }

        public abstract string GetSubsystemName(int subSystemNum);

        public void ApplyEffects() // Regulat ship effects
        {
            ReactorEffect();
            LifeSupportEffect();
        }

        private void ReactorEffect()
        {
            if (_subsystems.Count == 8)
            {
                byte hitProbability = 0;
                switch (_subsystems[7].ResultSeverity)
                {
                    case (RanmaRepairSeverity.NotDamaged):
                        hitProbability = 1;
                        break;
                    case (RanmaRepairSeverity.Easy):
                        hitProbability = 2;
                        break;
                    case (RanmaRepairSeverity.Medium):
                        hitProbability = 3;
                        break;
                    case (RanmaRepairSeverity.Hard):
                        hitProbability = 4;
                        break;
                }
                
                foreach (var reactorRoom in ReactorRooms)
                {
                    Model.SendRoomHit(reactorRoom, hitProbability, HitType.Radiation);
                }
                if (IsReactorDamaged)
                {
                    var shipPeople = PeopleOnThisShip.ToArray();
                    if (shipPeople.Any())
                    {
                        Model.SendShowMessage(shipPeople.Random(),
                                              "Ты услышал странный шум от реактора, кажется с ним что-то не так.");
                    }
                }
            }
        }

        private void HitOneRandomRoomWithPeople(byte probability)
        {
            var targetRoom = RoomsWithPeople.RandomOrDefault();
            Model.SendRoomHit(targetRoom, probability, HitType.Random);
        }
        private void LifeSupportEffect()
        {
            switch (_subsystems[4].ResultSeverity)
            {
                
                case (RanmaRepairSeverity.Easy):
                    HitOneRandomRoomWithPeople(1);
                    break;
                case (RanmaRepairSeverity.Medium):
                    HitOneRandomRoomWithPeople(1);
                    HitOneRandomRoomWithPeople(1);
                    break;
                case (RanmaRepairSeverity.Hard):
                    HitOneRandomRoomWithPeople(1);
                    HitOneRandomRoomWithPeople(1);
                    HitOneRandomRoomWithPeople(1);
                    break;
            }

            if (_subsystems[4].ResultSeverity != RanmaRepairSeverity.NotDamaged)
            {
                var shipPeople = PeopleOnThisShip.ToArray();
                if (shipPeople.Any())
                {
                    Model.SendShowMessage(PeopleOnThisShip.Random(),
                                          "Ты сообразил, что с системой жизнеобеспечения что-то явно не так! Надо что-то делать!");
                }
            }

        }

        protected abstract bool IsReactorDamaged { get; }
        protected bool ShipIsUnderFrontLineProtection
        {
            get
            {
                return
                    GetAllSubsystemsStatus()
                        .Where(a => a.IsFirstLineOfDefence == true)
                        .All(a => a.ResultSeverity == RanmaRepairSeverity.NotDamaged);
            }
        }
        
        public IEnumerable<byte> ReactorRooms
        {
            get
            {
                return Rooms.Where(r => r.IsReactor).Select(r => r.Id);
            }
        }
    }
}
