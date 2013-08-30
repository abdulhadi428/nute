    using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HonorLogic
{
    class ObjectListBase<TKey, TObj>
    {
        private readonly Dictionary<TKey, TObj> _armlets = new Dictionary<TKey, TObj>();
        private readonly object _syncRoot = new object();

        private bool IsRegistered(TKey armletId)
        {
            return _armlets.ContainsKey(armletId);
        }


        public bool CreateIfNeeded(TKey armletId, Func<TKey, TObj> creator)
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

        public TObj Get(TKey armletId)
        {
            lock (_syncRoot)
            {
                return _armlets[armletId];
            }
        }

        public TObj[] GetAll()
        {
            lock (_syncRoot)
            {
                return _armlets.Values.ToArray();
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
    }
}