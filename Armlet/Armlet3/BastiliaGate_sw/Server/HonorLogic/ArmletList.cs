using System;
using System.Collections.Generic;
using System.Linq;
using NetworkLevel.WCFServices;
using HonorInterfaces;

namespace HonorLogic
{
    internal class ArmletList : ObjectListBase<byte, Armlet>
    {
        public void UpdateArmlet(PlayerUpdate playerStatusUpdate)
        {
            var armlet = Get(playerStatusUpdate.ArmletID);
            armlet.Update(playerStatusUpdate);
        }

        public List<AStoredData> GetDataToStore()
        {
            return GetAll().Select(a => new AStoredData { Id = a.Id, Name = a.Name.Replace('\n', ' '), Regen = a.Regen }).ToList();
        }
    }
}