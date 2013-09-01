using System;
using System.Collections.Generic;
using System.Linq;
using HonorInterfaces;
using NetworkLevel.NetworkDeliveryLevel;

namespace HonorLogic.ShipStatus.RanmaPlates
{
    public class RanmaPlate
    {
        private static readonly byte[] RepairedTable = {0,0};
        private readonly List<ShipSubsystemStatus> _plateStatusList = new List<ShipSubsystemStatus>();
        private readonly int _physicalGateId;  //id гейта, соответсвующего плате. Задается в конструкторе

        public RanmaPlate(int gateId, int subsystemsCount)
        {
            _physicalGateId = gateId;
            for (int i = 0; i < subsystemsCount; i++)
            {
                _plateStatusList.Add(new ShipSubsystemStatus {Severity = RanmaRepairSeverity.Ready, SubSystemNum = i});
            }

            NetworkDelivery.GateDeliveryInstance.PillDataRead +=
                        (id, data) => IfMe(id, () => PlateDataRead(data));
        }

        public void InitiateUpdatePlateInfo()  // Дергает вызов чтения 24-байтов с платы
        {
            try
            {
                NetworkDelivery.GateDeliveryInstance.SendPillRead((byte)_physicalGateId, new byte[] { 0, 24 });
            }
            catch
            {
            }
            
        }

        private void PlateDataRead(byte[] data) // Вызывается в момент, когда произведено чтение байт с платы
        {
            for (int i = 0; i <= 7; i++)
            {
                byte[] subsystemData = data.Skip(i*3).Take(3).ToArray();
                if (subsystemData.Skip(1).ToArray().Equals(RepairedTable))
                {
                    _plateStatusList.First(a => a.SubSystemNum == subsystemData[0]).Severity = RanmaRepairSeverity.Ready;
                }
            }
        }

        private void IfMe(byte id, Action action)
        {
            if (_physicalGateId == id)
            {
                action();
            }
        }

        public void SetSubsystemSeverity(int subSystemNum, RanmaRepairSeverity ranmaRepairSeverity)
        {
            _plateStatusList.First(a => a.SubSystemNum == subSystemNum).Severity = ranmaRepairSeverity;
            NetworkDelivery.GateDeliveryInstance.SendPillWhite((byte) _physicalGateId,
                new byte[] {0, (byte) subSystemNum}.Concat(
                    RanmaSubsystemStatusFactory.GenerateRanmaSubsystemStatus(ranmaRepairSeverity).Bytes).ToArray());
        }

        public RanmaRepairSeverity GetSubsystemSeverity(int i)
        {
            return _plateStatusList.First(a => a.SubSystemNum == i).Severity;
        }

        public ShipSubsystemStatus GetSubsystem(int i)
        {
            return _plateStatusList.First(a => a.SubSystemNum == i);
        }
    }
}

