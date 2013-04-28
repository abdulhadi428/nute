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
            service.ArmletsStatusUpdate +=service_ArmletsStatusUpdate;
        }

        private void service_ArmletsStatusUpdate(PlayerStatusUpdate[] obj)
        {
            lock (_syncRoot)
            {
                foreach (var playerStatusUpdate in obj)
                {
                    
                }
            }
        }

        private readonly object _syncRoot = new object();
        private readonly List<IArmletInfo> _armlets = new List<IArmletInfo>();
        public IEnumerable<IArmletInfo> GetArmlets()
        {
            return _armlets;
        }

        public event Action ArmletListUpdated;

        private void OnArmletListUpdated()
        {
            var handler = ArmletListUpdated;
            if (handler != null) handler();
        }
    }
}
