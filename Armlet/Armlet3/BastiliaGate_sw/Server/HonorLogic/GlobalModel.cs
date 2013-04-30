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
        private readonly Lazy<GateModel>[] _gates;

        
        public GlobalModel(IArmletDeliveryServece armletService, IGateDeliveryService gateService)
        {
            _gates = new Lazy<GateModel>[byte.MaxValue + 1];
            for (var gateIndex = 0; gateIndex <= byte.MaxValue; gateIndex++)
            {
                var index = (byte) gateIndex; //To prevent closure problems. C# 5.0 will fix it
                _gates[gateIndex] = new Lazy<GateModel>(() => new GateModel(gateService, index));
            }


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
                var created = _armletList.CreateIfNeeded(playerStatusUpdate.ArmletID, CreateArmlet);
                if (created)
                {
                    raiseListUpdated = true;
                }
                _armletList.UpdateArmlet(playerStatusUpdate);
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

       

        public IGateModel GetGateModel(byte gateId)
        {
            return _gates[gateId].Value;
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
