using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;

namespace HonorLogic
{
    [Serializable]
    public struct AStoredData
    {
        public byte Id { get; set; }
        public string Name { get; set; }
    }
    internal class ArmletStorage
    {
        private readonly Dictionary<byte, AStoredData> _savedData;
        private readonly object _saveRoot = new object();

        private static FileInfo GetPersistFileInfo()
        {
            return new FileInfo(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\armlet.json");
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

        public ArmletStorage()
        {
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
        }

        public string GetName(byte armletId)
        {
            return _savedData.ContainsKey(armletId) ? _savedData[armletId].Name : null;
        }

        public void SaveData(List<AStoredData> pairs)
        {
            var serializer = new JsonSerializer();
            lock (_saveRoot)
            {
                using (var writer = GetPersistFileInfo().CreateText())
                {
                    serializer.Serialize(writer, pairs);
                }
            }
        }
    }
}