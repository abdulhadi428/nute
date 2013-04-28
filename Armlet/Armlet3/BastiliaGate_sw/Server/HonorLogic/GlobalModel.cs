using System;
using System.Collections.Generic;
using System.Threading;
using NetworkLevel.NetworkDeliveryLevel;
using PillInterfaces;

namespace HonorLogic
{
    public class GlobalModel : IGlobalModel
    {
        public GlobalModel(IArmletDeliveryServece armletService, IGateDeliveryService gateService)
        {
            _armletService = armletService;
            _gateService = gateService;
            
            _armletService.ArmletsStatusUpdate +=ArmletServiceArmletsStatusUpdate;
            _gateService.GateConnected += _gateService_GateConnected;
        }

        void _gateService_GateConnected(byte obj)
        {
            OnNewGateOnline(obj);
        }

        private void ArmletServiceArmletsStatusUpdate(PlayerStatusUpdate[] obj)
        {
            var raiseListUpdated = false;
            lock (_syncRoot)
            {
                foreach (var playerStatusUpdate in obj)
                {
                    byte armletId = playerStatusUpdate.armlet_id;
                    if (!_armlets.ContainsKey(armletId))
                    {
                        raiseListUpdated = true;
                        _armlets.Add(armletId, CreateArmlet(armletId));
                    }
                    var armlet = _armlets[armletId];
                    armlet.Update(playerStatusUpdate);
                }
            }
            if (raiseListUpdated)
            {
                OnArmletListUpdated();
            }
        }

        private Armlet CreateArmlet(byte armletId)
        {
            return new Armlet(armletId, this);
        }

        private readonly object _syncRoot = new object();
        private readonly Dictionary<int, Armlet> _armlets = new Dictionary<int, Armlet>();
        private readonly IArmletDeliveryServece _armletService;
        private readonly IGateDeliveryService _gateService;

        public IEnumerable<IArmletInfo> GetArmlets()
        {
            return _armlets.Values;
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

        private int _lastNonce = 1;

        public void SendPayload(byte id, byte[] payload)
        {
            var nonce = Interlocked.Add(ref _lastNonce, 1);
            _armletService.DeliverToSingleArmlet(id, (short) (nonce & 0xFFFF), payload);
        }
    }
}
