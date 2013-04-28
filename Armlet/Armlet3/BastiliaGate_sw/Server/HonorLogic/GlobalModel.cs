using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using NetworkLevel.NetworkDeliveryLevel;
using NetworkLevel.WCFServices;
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

            var readJson = ReadJson();
            if (readJson != null)
            {
                _savedData =
                    JsonConvert.DeserializeObject<List<AStoredData>>(readJson)
                               .ToDictionary(a => a.Id);
            }
            else
            {
                _savedData = new Dictionary<byte, AStoredData>();
            }


            _armletService.ArmletsStatusUpdate +=ArmletServiceArmletsStatusUpdate;
            _gateService.GateConnected += _gateService_GateConnected;


        }

        private static string ReadJson()
        {
            try
            {
                return GetPersistFileInfo().OpenText().ReadToEnd();
            }
            catch
            {
                return null;
            }
            
        }

        void _gateService_GateConnected(byte obj)
        {
            OnNewGateOnline(obj);
        }

        private readonly ArmletList _armletList = new ArmletList();

        private class ArmletList
        {
            private readonly Dictionary<int, Armlet> _armlets = new Dictionary<int, Armlet>();
            private readonly object _syncRoot = new object();

            private bool IsRegistered(byte armletId)
            {
                return _armlets.ContainsKey(armletId);
            }

            public bool CreateIfNeeded(byte armletId, Func<byte, Armlet> creator)
            {
                lock (_syncRoot)
                {
                    if (IsRegistered(armletId))
                    {
                        return false;
                    }
                    _armlets.Add(armletId, creator(armletId));
                    return true;
                }
            }

            public void UpdateArmlet(byte armletId, PlayerUpdate playerStatusUpdate)
            {
                var armlet = _armlets[armletId];
                armlet.Update(playerStatusUpdate);
            }

            public IEnumerable<IArmletInfo> Get()
            {
                lock (_syncRoot)
                {
                    return _armlets.Values;
                }
            }

            public List<AStoredData> GetDataToStore()
            {
                lock (_syncRoot)
                {
                    return _armlets.Select(a => new AStoredData { Id = a.Value.Id, Name = a.Value.Name }).ToList();
                }
            }

            public void Remove(byte id)
            {
                lock (_syncRoot)
                {
                    if (_armlets.ContainsKey(id))
                    {
                        _armlets.Remove(id);
                    }
                }
            }
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
            string name = _savedData.ContainsKey(armletId) ? _savedData[armletId].Name : null;
            var armlet = new Armlet(armletId, this, name);
            return armlet;
        }
        
        private readonly IArmletDeliveryServece _armletService;
        private readonly IGateDeliveryService _gateService;

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

        private readonly object _saveRoot = new object();

        public void SavePersistent()
        {
            Task.Factory.StartNew(() =>
                {
                    var pairs = _armletList.GetDataToStore();
                    var serializer = new JsonSerializer();
                    lock (_saveRoot)
                    {
                        using (var writer = GetPersistFileInfo().CreateText())
                        {
                            serializer.Serialize(writer, pairs);
                        }
                    }
                });
        }

        private static FileInfo GetPersistFileInfo()
        {
            return new FileInfo(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\armlet.json");
        }
    }

    [Serializable]
    public struct AStoredData
    {
        public byte Id { get; set; }
        public string Name { get; set; }
    }
}
