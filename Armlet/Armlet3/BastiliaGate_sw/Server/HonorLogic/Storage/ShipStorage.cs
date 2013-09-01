using System;
using System.Collections.Generic;
using System.Linq;
using HonorInterfaces;
using HonorLogic.Annotations;
using HonorLogic.ShipStatus;
using Newtonsoft.Json;

namespace HonorLogic.Storage
{
    [Serializable]
    public struct ShipStoredData
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public List<ShipSubsystemStatus> Subsystems { get; set; }
        public List<byte> Gates { get; set; }
        public List<RoomInfo> Rooms { get; set; }
    }

    [Serializable]
    public struct RoomInfo
    {
        public byte Id { get; [UsedImplicitly] set; }
        public string Name { get; [UsedImplicitly] set; }
    }

    internal class ShipStorage
    {
        private readonly Dictionary<Guid, ShipStoredData> _savedData;

        public ShipStorage()
        {
            var storageBase = new StorageBase<ShipStoredData>("ship");
            var readJson = storageBase.ReadFile(".");
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

        //public void SaveData(List<ShipStoredData> pairs)
        //{
        //    _storageBase.Save(pairs);
        //}

        public ShipBase CreateObject(Guid id, GlobalModel model)
        {
            var obj = _savedData.ContainsKey(id) ? (ShipStoredData?) _savedData[id] : null;
            var gates = obj == null ? new List<byte> {71} : obj.Value.Gates;
            var big = gates.Count > 1;
            var ship = big ? (ShipBase) new BigShip() : new LakShip();
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
            ship.Rooms = (obj == null ? new List<RoomInfo>() : obj.Value.Rooms) ?? new List<RoomInfo>();
            ship.InitializeRanmaPlate();
            return ship;
        }

        public IEnumerable<Guid> Keys
        {
            get { return _savedData.Keys.ToArray(); }
        }
    }
}