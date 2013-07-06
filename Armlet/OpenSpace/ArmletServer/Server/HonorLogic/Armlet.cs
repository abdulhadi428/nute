using System;
using System.Linq;
using NetworkLevel.WCFServices;
using PillInterfaces;

namespace HonorLogic
{
    class Armlet : IArmletInfo
    {
        private readonly GlobalModel _model;

        public event Action ArmletUpdated;
        public void SetRegen(byte regen)
        {
            var old = Regen;
            Regen = regen;
            if (regen != old)
            {
                _model.SavePersistent();
                OnArmletUpdated();
            }
        }

        public void SetToxic(byte toxic)
        {
            Toxic = toxic;
        }

        private void OnArmletUpdated()
        {
            Action handler = ArmletUpdated;
            if (handler != null) handler();
        }

        public Armlet(byte armletId, GlobalModel model, string name, byte getRegen)
        {
            Name = name ?? ("Браслет " + armletId);
            _model = model;
            Id = armletId;
            Regen = getRegen;
        }

        public void SetName(string text)
        {
            var oldName = Name;
            Name = text;
            if (oldName != text)
            {
                _model.SavePersistent();
                OnArmletUpdated();
            }
        }

        public void SetStatus(string status)
        {
            Status = status;
            OnArmletUpdated();
        }

        public byte Id { get; private set; }
        public byte Room { get; private set; }
        public byte BloodLevel { get; private set; }
        public string Name { get; private set; }
        public string Status { get; private set; }
        public byte Regen { get; private set; }
        public byte Toxic { get; private set; }

        public void Update(PlayerUpdate playerStatusUpdate)
        {
            BloodLevel = playerStatusUpdate.NewBlood;
            Room = playerStatusUpdate.NewRoom;
            OnArmletUpdated();
        }

        public static string TransformName(string text)
        {
            text = text.Trim();
            if (text.Count(ch => ch == ' ') < 3)
            {
                text = text.Replace(' ', '\n');
            }
            return text;
        }
    }
}
