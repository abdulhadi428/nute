using System;
using System.Collections.Generic;
using System.Linq;
using NetworkLevel.WCFServices;
using PillInterfaces;

namespace HonorLogic
{
    internal class ArmletList
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

        public void UpdateArmlet(PlayerUpdate playerStatusUpdate)
        {
            var armlet = _armlets[playerStatusUpdate.ArmletID];
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
                return _armlets.Select(a => new AStoredData { Id = a.Value.Id, Name = a.Value.Name.Replace('\n', ' '), Regen = a.Value.Regen }).ToList();
            }
        }

/*
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
*/

        public IArmletInfo[] GetAll()
        {
            lock (_syncRoot)
            {
                return _armlets.Select(a => a.Value).Cast<IArmletInfo>().ToArray();
            }
        }

        public IArmletInfo GetById(byte armletId)
        {
            lock (_syncRoot)
            {
                return _armlets[armletId];
            }
        }
    }
}