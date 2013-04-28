using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;
using HonorGateServer.WCFServices;

namespace HonorGateServer.NetworkDeliveryLevel
{
    public static class  NetworkDelivery
    {
        internal static ServiceHost myServiceHost = null; 

        public static IGateDeliveryService GateDeliveryInstance;
        public static IArmletDeliveryServece ArmletDeliveryInstance;
        
        static NetworkDelivery()
        {
            if (myServiceHost != null)
            {
                myServiceHost.Close();
            }

            myServiceHost = new ServiceHost(typeof(GateWcfService));
            myServiceHost.Open();
        }
    }
}
