using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using NetworkLevel.NetworkDeliveryLevel;
using Newtonsoft.Json;
using PillInterfaces;

namespace HonorLogic
{
    public class GlobalModel : IGlobalModel
    {
        private readonly Dictionary<byte, AStoredData> _savedData;
        public GlobalModel(IArmletDeliveryServece armletService, IGateDeliveryService gateService)
        {
            _armletService = armletService;
            _gateService = gateService;

            _savedData =
                JsonConvert.DeserializeObject<List<AStoredData>>(GetPersistFileInfo().OpenText().ReadToEnd())
                           .ToDictionary(a => a.Id);
            
            
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
            var armlet = new Armlet(armletId, this);
            if (_savedData.ContainsKey(armletId))
            {
                armlet.SetName(_savedData[armletId].Name);
            }
            return armlet;
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

        public void SendPayload(byte id, byte[] payload)
        {
            _armletService.DeliverToSingleArmlet(id, payload);
        }

        private readonly object _saveRoot = new object();

        public void SavePersistent()
        {
            Task.Factory.StartNew(() =>
                {
                    lock (_saveRoot)
                    {
                        var pairs = _armlets.Select(a => new AStoredData{Id = a.Value.Id, Name =a.Value.Name}).ToList();
                        var serializer = new JsonSerializer();
                        using (var writer = GetPersistFileInfo().CreateText())
                        {
                            serializer.Serialize(writer, pairs);
                        }
                    }
                });
        }

        private static FileInfo GetPersistFileInfo()
        {
            return new FileInfo(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData));
        }
    }

    [Serializable]
    public struct AStoredData
    {
        public byte Id { get; set; }
        public string Name { get; set; }
    }
}
