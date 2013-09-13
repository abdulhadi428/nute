using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using HonorLogic.ShipStatus;
using HonorLogic.Storage;
using NetworkLevel.NetworkDeliveryLevel;
using NetworkLevel.WCFServices;
using HonorInterfaces;

namespace HonorLogic
{
    public sealed class GlobalModel : IGlobalModel
    {
        private readonly ArmletStorage _armletStorage = new ArmletStorage();
        private readonly ArmletList _armletList = new ArmletList();
        
        private readonly ShipList _shipList = new ShipList();
        private readonly ShipStorage _shipStorage = new ShipStorage();

        private readonly IArmletDeliveryServece _armletService;
        private readonly IGateDeliveryService _gateDeliveryService;
        private readonly ShipDamageServiceObj _shipDamageService;
        private readonly Lazy<GateModel>[] _gates;

        private Timer heartBeat_timer;
        private Timer effectTimer;


        public GlobalModel(IArmletDeliveryServece armletService, IGateDeliveryService gateService, ShipDamageServiceObj shipDamageService)
        {
            _armletService = armletService;
            _gateDeliveryService = gateService;
            _shipDamageService = shipDamageService;

            _gates = new Lazy<GateModel>[Byte.MaxValue + 1];
            for (var gateIndex = 0; gateIndex <= Byte.MaxValue; gateIndex++)
            {
                var index = (byte) gateIndex; //To prevent closure problems. C# 5.0 will fix it
                _gates[gateIndex] = new Lazy<GateModel>(() => new GateModel(gateService, index));
            }

            for (var armletIndex = 1; armletIndex <= 120; armletIndex++)
            {
                _armletList.CreateIfNeeded((byte)armletIndex, armletId => _armletStorage.CreateObject(armletId, this));
            }

            foreach (var shipIdx in _shipStorage.Keys)
            {
                _shipList.CreateIfNeeded(shipIdx, id => _shipStorage.CreateObject(id, this));
            }
            
            _armletService.ArmletsStatusUpdate +=ArmletServiceArmletsStatusUpdate;
            gateService.GateConnected += OnNewGateOnline;
            _armletService.ArmletSuccess += OnArmletListSuccess;
            _armletService.ArmletSendsData += _armletService_ArmletSendsData;
            shipDamageService.ShipDamaged += ShipDamageServiceOnShipDamaged;
            shipDamageService.ShipDestroyed += ShipDamageServiceOnShipDestroyed;
            shipDamageService.SimulatorDisconnectedEvent  += ShipDamageServiceOnSimulatorDisconnectedEvent;


// ReSharper disable once ObjectCreationAsStatement
            heartBeat_timer = new Timer(SendHeartBeat, 0, 0, 1 * 1000);
            // ReSharper disable once ObjectCreationAsStatement
            effectTimer = new Timer(ApplyShipEffects, 0, 0, 60*1000);
        }

        private void ShipDamageServiceOnSimulatorDisconnectedEvent()
        {
            var handler = SimulatorDisconnected;
            if (handler != null) handler();
        }

        private void ShipDamageServiceOnShipDestroyed(Guid guid)
        {
            if (_shipList.HasShipWithThisGuid(guid))
            {
                _shipList.GetShipByGuid(guid).DestroyShip();
            }
           
        }

        private void ShipDamageServiceOnShipDamaged(Guid guid, byte b)
        {
            if (_shipList.HasShipWithThisGuid(guid))
            {
                _shipList.GetShipByGuid(guid).DamageShip(b);
            }
            else
            {
                _shipDamageService.SendToSimulator(guid,
                                                   new List<ShipSubsystemStatus>
                                                       {
                                                           new ShipSubsystemStatus()
                                                               {
                                                                   SubSystemNum = 0,
                                                                   EffectiveTable = 0xAAAA,
                                                                   Severity = RanmaRepairSeverity.Hard
                                                               },
                                                            new ShipSubsystemStatus()
                                                               {
                                                                   SubSystemNum = 1,
                                                                   EffectiveTable = 0xAAAA,
                                                                   Severity = RanmaRepairSeverity.Hard
                                                               },
                                                            new ShipSubsystemStatus()
                                                               {
                                                                   SubSystemNum = 2,
                                                                   EffectiveTable = 0xAAAA,
                                                                   Severity = RanmaRepairSeverity.Hard
                                                               },
                                                            new ShipSubsystemStatus()
                                                               {
                                                                   SubSystemNum = 3,
                                                                   EffectiveTable = 0xAAAA,
                                                                   Severity = RanmaRepairSeverity.Hard
                                                               },
                                                             new ShipSubsystemStatus()
                                                               {
                                                                   SubSystemNum = 4,
                                                                   EffectiveTable = 0xAAAA,
                                                                   Severity = RanmaRepairSeverity.Hard
                                                               },
                                                             new ShipSubsystemStatus()
                                                               {
                                                                   SubSystemNum = 5,
                                                                   EffectiveTable = 0xAAAA,
                                                                   Severity = RanmaRepairSeverity.Hard
                                                               },
                                                              new ShipSubsystemStatus()
                                                               {
                                                                   SubSystemNum = 6,
                                                                   EffectiveTable = 0xAAAA,
                                                                   Severity = RanmaRepairSeverity.Hard
                                                               },
                                                            new ShipSubsystemStatus()
                                                               {
                                                                   SubSystemNum = 7,
                                                                   EffectiveTable = 0xAAAA,
                                                                   Severity = RanmaRepairSeverity.Hard
                                                               }

                                                       });
            }
        }

        private static void SendHeartBeat(object state)
        {
            var gates = GateConnectionPool.GateConnections.Values.ToArray(); // TODO FIXME possible not thread-safe
            foreach (var gate in gates)
            {
                try
                {
                    gate.SendHeartbeat();
                }
// ReSharper disable once EmptyGeneralCatchClause
                catch
                {
                    //Intentionally left blank
                }
                
            }
        }

        void _armletService_ArmletSendsData(byte armletId, byte[] payload)
        {
            if (payload.Length > 3)
            {
                _armletList.Get(armletId).SetToxic(payload[3]);
            }
        }

        private void OnArmletListSuccess(byte armletId)
        {
            _armletList.Get(armletId).SetStatus("Сообщение доставлено");
            OnArmletListUpdated();
        }

        private void ArmletServiceArmletsStatusUpdate(PlayerUpdate[] obj)
        {
            foreach (var playerStatusUpdate in obj)
            {
                //_armletList.CreateIfNeeded(playerStatusUpdate.ArmletID, CreateArmlet);
                _armletList.UpdateArmlet(playerStatusUpdate);
            }

            OnArmletListUpdated();
        }
        
        public IEnumerable<IArmletInfo> GetArmlets()
        {
            return _armletList.GetAll();
        }

        public event Action ArmletListUpdated;
        public event Action<byte> NewGateOnline;
        public event Action SimulatorDisconnected;

        public void SendMessageToAll(string text)
        {
            SendPayloadToAll(CreateMessagePayload(text));
        }

        private static byte[] CreateMessagePayload(string text)
        {
            if (text.Length > 120)
            {
                throw new Exception("Message too long");
            }
            return CreateStringPayload(MessageId.MSG_SHOW_MESSAGE, string.Format("* {0}: {1}\n\n", DateTime.Now.ToShortTimeString(), text));
        }

        private void OnNewGateOnline(byte obj)
        {
            var handler = NewGateOnline;
            if (handler != null) handler(obj);
        }

        public IGateModel GetGateModel(byte gateId)
        {
            return _gates[gateId].Value;
        }

        private void OnArmletListUpdated()
        {
            var handler = ArmletListUpdated;
            if (handler != null) handler();
        }

        private void SendPayload(IArmletInfo armlet, byte[] payload)
        {
            armlet.SetStatus("Отправляется сообщение");
            OnArmletListUpdated();
            try
            {
                _armletService.DeliverToSingleArmlet(armlet.Id, payload);
            }
            catch (GateNotConnectedException exc)
            {
                foreach (var gateId in exc.GateId)
                {
                    _gates[gateId].Value.SetOnline(false);
                }
            }
           
        }

        private void SendPayloadToAll(byte[] payload)
        {
            var armlets = _armletList.GetAll();
            foreach (var armletInfo in armlets)
            {
                armletInfo.SetStatus("Отправляется сообщение");
            }
            OnArmletListUpdated();
            try
            {
                _armletService.DeliverToArmlets(armlets.Select(armlet => armlet.Id).ToArray(), payload);
            }
            catch (GateNotConnectedException exc)
            {
                foreach (var gateId in exc.GateId)
                {
                    _gates[gateId].Value.SetOnline(false);
                }
            }
        }

        public void SavePersistent()
        {
            //TODO coalesce save request
            Task.Factory.StartNew(SaveTask);
        }

        private void SaveTask()
        {
            _armletStorage.SaveData(_armletList.GetDataToStore());
            //_shipStorage.SaveData(_shipList.GetDataToStore());
        }

        private static byte[] CreatePayload(MessageId messageId, IEnumerable<byte> data)
        {
            return new[] {(byte) messageId, (byte) 0x00, (byte) 0x00}.Concat(data).ToArray();
        }

        private static byte[] CreateStringPayload(MessageId messageId, string text)
        {
            return CreatePayload(messageId, Encoding.GetEncoding(1251).GetBytes(text).Concat(new byte[]{0}).ToArray());
        }

        private void SendCommand(IArmletInfo armletId, MessageId messageId, string text)
        {
            if (text.Length > 140)
            {
                throw new ArgumentOutOfRangeException("text");
            }
            SendPayload(armletId, CreateStringPayload(messageId, text));
        }

        public void SendSetName(string text, IArmletInfo armletId)
        {
            text = Armlet.TransformName(text);
            armletId.SetName(text);
            SendCommand(armletId, MessageId.MSG_SET_PLAYER_NAME, text);
        }

        public void SendShowMessage(IArmletInfo armletId, string text)
        {
            SendPayload(armletId, CreateMessagePayload(text));
        }

        public void SetSetPlayerRegen(IArmletInfo armlet, byte regen)
        {
            armlet.SetRegen(regen);
            SendPayload(armlet, CreatePayload(MessageId.MSG_SET_PLAYER_REGENERATION, new[] {regen}));
        }

/*
        public void SendSetLockList(IArmletInfo armlet, byte[] lockList)
        {
            SendPayload(armlet, CreatePayload(MessageId.MSG_UPDATE_LOCK_LIST, lockList));
        }
*/

        public void SendRoomHit(byte roomId, byte hitChance , HitType hitType)
        {
            SendPayloadToAll(CreatePayload(MessageId.MSG_ROOM_HIT, new[] {roomId, hitChance, (byte) hitType}));
        }

        public void UpdateAllNames()
        {
            foreach (var armletInfo in _armletList.GetAll())
            {
                SendCommand(armletInfo, MessageId.MSG_SET_PLAYER_NAME, armletInfo.Name);
                SendPayload(armletInfo, CreatePayload(MessageId.MSG_SET_PLAYER_REGENERATION, new[] { armletInfo.Regen }));
            }
        }

        public IEnumerable<IShip> GetShips()
        {
            return _shipList.GetAll();
        }

        public IEnumerable<byte> RoomsWithPeople
        {
            get
            {
                return _armletList.GetAll().Select(a => a.Room).Distinct();
            }
        }

        public bool IsShipGate(byte gateId)
        {
            return Ships.Any(s => s.PhysicalGateID.Contains(gateId));
        }

        private IEnumerable<ShipBase> Ships
        {
            get { return _shipList.GetAll(); }
        }

        public bool ConnectToSimulator()
        {
            return NetworkDelivery.ConnectToSimulator();
        }

        public IEnumerable<IArmletInfo> PeopleIDsInCertainRooms(IEnumerable<byte> RoomIDs)
        {
            return _armletList.GetAll().Where(a => RoomIDs.Contains(a.Room));
        }

        

        

        

        public string GetRoomName(byte room)
        {
            return Ships.Select(s => s.FindRoomName(room)).SingleOrDefault(s => s != null)
                ??  ShipBase.GetDefaultRoomName(room);
        }

        private void ApplyShipEffects(object state)
        {
            foreach (var ship in Ships)
            {
                ship.ApplyEffects();
            }
        }
    }
}
