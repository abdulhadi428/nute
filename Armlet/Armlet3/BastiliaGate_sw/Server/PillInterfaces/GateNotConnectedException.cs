using System;

namespace HonorInterfaces
{
    public class GateNotConnectedException : Exception
    {
        public byte[] GateId { get; set; }

        public GateNotConnectedException(byte[] gateId)
            : base("Gate with id " + gateId + " is not connected")
        {
            GateId = gateId;
        }

        public GateNotConnectedException(byte gateId, Exception exception)
            : base("Gate with id " + gateId + " is not connected",  exception)
        {
            GateId = new[] {gateId};
        }

        public GateNotConnectedException(byte gateId) : this (new[] {gateId}) {
        }
    }
}
