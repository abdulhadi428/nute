using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace HonorGateServer
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "IwcfService" in both code and config file together.
    public class GateWcfService : IGateWCFService
    {
        public string DoWork(string value)
        {
            return ("welcome" + value);
        }
    }
}
