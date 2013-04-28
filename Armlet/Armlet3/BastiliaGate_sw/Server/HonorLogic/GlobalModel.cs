using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using NetworkLevel.NetworkDeliveryLevel;
using NetworkLevel.WCFServices;
using PillInterfaces;

namespace HonorLogic
{
    public class GlobalModel : IGlobalModel
    {
        private readonly ArmletStorage _armletStorage = new ArmletStorage();
        private readonly ArmletList _armletList = new ArmletList();
        private readonly IArmletDeliveryServece _armletService;
        private readonly IGateDeliveryService _gateService;

        
        public GlobalModel(IArmletDeliveryServece armletService, IGateDeliveryService gateService)
        {
            _armletService = armletService;
            _gateService = gateService;

            _armletService.ArmletsStatusUpdate +=ArmletServiceArmletsStatusUpdate;
            _gateService.GateConnected += OnNewGateOnline;
        }

        private void ArmletServiceArmletsStatusUpdate(PlayerUpdate[] obj)
        {
            var raiseListUpdated = false;

            foreach (var playerStatusUpdate in obj)
            {
                byte armletId = playerStatusUpdate.ArmletID;
                raiseListUpdated = _armletList.CreateIfNeeded(armletId, CreateArmlet);
                _armletList.UpdateArmlet(armletId, playerStatusUpdate);
            }

            if (raiseListUpdated)
            {
                OnArmletListUpdated();
            }
        }

        private Armlet CreateArmlet(byte armletId)
        {
            return new Armlet(armletId, this, _armletStorage.GetName(armletId));
        }
        
        public IEnumerable<IArmletInfo> GetArmlets()
        {
            return _armletList.Get();
        }

        public event Action ArmletListUpdated;
        public event Action<byte> NewGateOnline;

        private void OnNewGateOnline(byte obj)
        {
            var handler = NewGateOnline;
            if (handler != null) handler(obj);
        }

        private readonly object _gateRoot = new object();
        private readonly Dictionary<byte, GateModel>  _gates = new Dictionary<byte, GateModel>();

        public IGateModel GetGateModel(byte gateId)
        {
            lock (_gateRoot)
            {
                if (!_gates.ContainsKey(gateId))
                {
                    _gates.Add(gateId, new GateModel(_gateService, gateId));
                }
                return _gates[gateId];
            }
        }

        private void OnArmletListUpdated()
        {
            var handler = ArmletListUpdated;
            if (handler != null) handler();
        }

        public void SendPayload(byte id, byte[] payload)
        {
            var task = _armletService.DeliverToSingleArmlet(id, payload);
            task.ContinueWith(result =>
                {
                    var value = result.Result;
                    if (value != ArmletDeliveryStatus.SuccessDeliver)
                    {
                        _armletList.Remove(id);
                        OnArmletListUpdated();
                    }
                });
        }

        public void SavePersistent()
        {
            Task.Factory.StartNew(() => _armletStorage.SaveData(_armletList.GetDataToStore()));
        }
    }
}
