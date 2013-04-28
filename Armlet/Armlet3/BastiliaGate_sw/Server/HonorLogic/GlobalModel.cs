using System;
using System.Collections.Generic;
using NetworkLevel.NetworkDeliveryLevel;
using PillInterfaces;

namespace HonorLogic
{
    class GlobalModel : IGlobalModel
    {
        public GlobalModel(IArmletDeliveryServece service)
        {
            _service = service;
            _service.ArmletsStatusUpdate +=service_ArmletsStatusUpdate;
        }

        private void service_ArmletsStatusUpdate(PlayerStatusUpdate[] obj)
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
                    armlet.BloodLevel = playerStatusUpdate.new_blood;
                    armlet.Room = playerStatusUpdate.new_room;
                }
            }
            if (raiseListUpdated)
            {
                OnArmletListUpdated();
            }
        }

        private Armlet CreateArmlet(byte armletId)
        {
            return new Armlet(armletId, _service);
        }

        private readonly object _syncRoot = new object();
        private readonly Dictionary<int, Armlet> _armlets = new Dictionary<int, Armlet>();
        private readonly IArmletDeliveryServece _service;

        public IEnumerable<IArmletInfo> GetArmlets()
        {
            return _armlets.Values;
        }

        public event Action ArmletListUpdated;
        public event Action<byte> NewGateOnline;
        public IGateModel GetGateModel(byte gateId)
        {
            throw new NotImplementedException();
        }

        private void OnArmletListUpdated()
        {
            var handler = ArmletListUpdated;
            if (handler != null) handler();
        }
    }
}
