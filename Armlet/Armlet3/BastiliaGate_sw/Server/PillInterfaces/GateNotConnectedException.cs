using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

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
