using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;

namespace HonorLogic.Storage
{
    internal class StorageBase<T>
    {    
        private readonly string _infoClass;
        private readonly object _saveRoot = new object();

        private string GetFileInfo(string dir)
        {
            return dir + @"\" + _infoClass + ".json";
        }

        public static string DefaultDir()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
        }

        public string ReadFile(string dir)
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

        public StorageBase(string infoClass)
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
}