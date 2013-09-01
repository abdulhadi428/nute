using System;
using System.Collections.Generic;
using System.Linq;
using Newtonsoft.Json;

namespace HonorLogic.Storage
{
    [Serializable]
    public struct AStoredData
    {
        public byte Id { get; set; }
        public string Name { get; set; }
        public byte Regen { get; set; }
    }

    internal class ArmletStorage
    {
        private readonly StorageBase<AStoredData> _storageBase;
        private readonly Dictionary<byte, AStoredData> _savedData;

        public ArmletStorage()
        {
            _storageBase = new StorageBase<AStoredData>("armlet");
            var readJson = _storageBase.ReadFile(StorageBase<AStoredData>.DefaultDir());
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
        }

        private string GetName(byte armletId)
        {
            return _savedData.ContainsKey(armletId) ? Armlet.TransformName(_savedData[armletId].Name) : null;
        }

        private byte GetRegen(byte armletId)
        {
            return (byte) (_savedData.ContainsKey(armletId) ? _savedData[armletId].Regen : 2);
        }

        public void SaveData(List<AStoredData> pairs)
        {
            _storageBase.Save(pairs);
        }

        public Armlet CreateObject(byte armletId, GlobalModel model)
        {
            return new Armlet(armletId, model, GetName(armletId), GetRegen(armletId));
        }
    }
}