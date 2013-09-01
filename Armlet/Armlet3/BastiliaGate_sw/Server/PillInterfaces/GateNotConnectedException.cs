using System;

namespace HonorInterfaces
{
    public class GateNotConnectedException : Exception
    {
        public GateNotConnectedException(byte gateId)
            : base("Gate with id " + gateId + " is not connected")
        {
        }
    }
}
