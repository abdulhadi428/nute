using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HonorInterfaces;

namespace HonorLogic.ShipStatus.RanmaPlates
{
    public class RanmaPlate
    {
        private static byte[] repairedTable = new byte[] {0,0};
        public List<ShipSubsystemStatus> plateStatusList = new List<ShipSubsystemStatus>();
        private int physicalGateId;  //id гейта, соответсвующего плате. Задается в конструкторе

        public RanmaPlate(int gateId, int subsystemsCount)
        {
            physicalGateId = gateId;
            for (int i = 0; i < subsystemsCount; i++)
            {
                plateStatusList.Add(new ShipSubsystemStatus(){Severity = RanmaRepairSeverity.Ready, SubSystemNum = i});
            }

            NetworkLevel.NetworkDeliveryLevel.NetworkDelivery.GateDeliveryInstance.PillDataRead +=
                        (id, data) => IfMe(id, () => PlateDataRead(data));
        }
            
        public void SetPlateSubsystemInfo(int subsystemNum, RanmaRepairSeverity severity, RanmaSubsystemTable table)
        {
            plateStatusList.First(a => a.SubSystemNum == subsystemNum).Severity = severity;
            NetworkLevel.NetworkDeliveryLevel.NetworkDelivery.GateDeliveryInstance.SendPillWhite((byte) physicalGateId,
                                                                     new byte[] { 0, (byte)subsystemNum }.Concat(table.Bytes).ToArray());
        }

        public void InitiateUpdatePlateInfo()  // Дергает вызов чтения 24-байтов с платы
        {
            NetworkLevel.NetworkDeliveryLevel.NetworkDelivery.GateDeliveryInstance.SendPillRead((byte)physicalGateId, new byte[]{0, 24});
        }

        private void PlateDataRead(byte[] data) // Вызывается в момент, когда произведено чтение байт с платы
        {
            for (int i = 0; i <= 7; i++)
            {
                byte[] subsystemData = data.Skip(i*3).Take(3).ToArray();
                if (subsystemData.Skip(1).ToArray().Equals(repairedTable))
                {
                    plateStatusList.First(a => a.SubSystemNum == subsystemData[0]).Severity = RanmaRepairSeverity.Ready;
                }
            }
        }

        private void IfMe(byte id, Action action)
        {
            if (physicalGateId == id)
            {
                action();
            }
        }
    }
}

