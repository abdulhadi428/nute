using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;
using HonorLogic.ShipStatus;

namespace HonorLogic
{
    [Serializable]
    public struct AStoredData
    {
        public byte Id { get; set; }
        public string Name { get; set; }
        public byte Regen { get; set; }
    }

    [Serializable]
    public struct ShipStoredData
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public List<ShipSubsystemStatus> Subsystems { get; set; }
        public List<int> Gates { get; set; }
    }

    internal class Storage<T>
    {    
        private readonly string _infoClass;
        private readonly object _saveRoot = new object();

        private string GetFileInfo(string dir)
        {
            return dir + @"\" + _infoClass + ".json";
        }
        public string ReadJson()
        {
            return ReadFile(DefaultDir()) ?? ReadFile(".");
        }

        private static string DefaultDir()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
        }

        private string ReadFile(string dir)
        {
            try
            {
                return File.ReadAllText(GetFileInfo(dir));
            }
            catch (FileNotFoundException)
            {
                return null;
            }
        }

        public Storage(string infoClass)
        {
            _infoClass = infoClass;
        }

        public void Save(List<T> pairs)
        {
            var serializer = new JsonSerializer();
            lock (_saveRoot)
            {
                using (var writer = new StreamWriter(GetFileInfo(DefaultDir())))
                {
                    serializer.Serialize(writer, pairs);
                }
            }
        }
    }

    internal class ArmletStorage
    {
        private readonly Storage<AStoredData> _storage;
        private readonly Dictionary<byte, AStoredData> _savedData;

        public ArmletStorage()
        {
            _storage = new Storage<AStoredData>("armlet");
            var readJson = _storage.ReadJson();
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
            _storage.Save(pairs);
        }

        public Armlet CreateObject(byte armletId, GlobalModel model)
        {
            return new Armlet(armletId, model, GetName(armletId), GetRegen(armletId));
        }
    }

    internal class ShipStorage
    {
        private readonly Storage<ShipStoredData> _storage;
        private readonly Dictionary<Guid, ShipStoredData> _savedData;

        public ShipStorage()
        {
            _storage = new Storage<ShipStoredData>("ship");
            var readJson = _storage.ReadJson();
            if (readJson != null)
            {
                _savedData =
                    JsonConvert.DeserializeObject<List<ShipStoredData>>(readJson)
                               .ToDictionary(a => a.Id);
            }
            else
            {
                _savedData = new Dictionary<Guid, ShipStoredData>();
            }
        }

        public void SaveData(List<ShipStoredData> pairs)
        {
            _storage.Save(pairs);
        }

        public IShip CreateObject(Guid id, GlobalModel model)
        {
            var obj = _savedData.ContainsKey(id) ? (ShipStoredData?) _savedData[id] : null;
            var gates = obj == null ? new List<int> {71} : obj.Value.Gates;
            var big = gates.Count > 1;
            var ship = big ? (IShip) new BigShip() : new LakShip();
            ship.PhysicalGateID = gates.ToArray();
            if (obj != null)
            {
                foreach (var s in obj.Value.Subsystems)
                {
                    ship.SetSubsystemStatus(s);
                }
            }
            ship.Name = obj == null ? id.ToString() : obj.Value.Name;
            ship.Model = model;
            ship.ShipGuid = id;
            return ship;
        }

        public Guid[] Keys
        {
            get { return _savedData.Keys.ToArray(); }
        }
    }
}