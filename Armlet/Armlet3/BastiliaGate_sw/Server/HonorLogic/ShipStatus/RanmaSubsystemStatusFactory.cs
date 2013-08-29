using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public static class RanmaSubsystemStatusFactory
    {
        private static Random random = new Random();
        private static List<ushort> easyTables = new List<ushort>() {0x0303, 0x8228, 0x1111, 0xA0A0, 0x0AA0, 0xA6A6, 0x8800, 
                                                   0x8808, 0x0161, 0x6116, 0x1310, 0x1F10, 0x1810, 0x4941, 0x2332,
                                                   0x0F99, 0x909F, 0x4F7F, 0x0B0B, 0x7077};
        private static List<ushort> mediumTables = new List<ushort>() { 0x0C0D };
        private static List<ushort> hardTables = new List<ushort>() { 0xDDFF };
        public static RanmaSubsystemStatus GenerateRanmaSubsystemStatus(RanmaRepairSeverity severity)
        {
            RanmaSubsystemStatus result;
            switch (severity)
            {
                    case RanmaRepairSeverity.Easy:
                        result = new RanmaSubsystemStatus(easyTables[random.Next(easyTables.Count - 1)]);
                        break;
                    case RanmaRepairSeverity.Medium:
                        result = new RanmaSubsystemStatus(mediumTables[random.Next(mediumTables.Count - 1)]);
                        break;
                    case RanmaRepairSeverity.Hard:
                        result = new RanmaSubsystemStatus(hardTables[random.Next(hardTables.Count - 1)]);
                        break;
                default:
                    return null;
            }
            return result;
        }
    }
}
