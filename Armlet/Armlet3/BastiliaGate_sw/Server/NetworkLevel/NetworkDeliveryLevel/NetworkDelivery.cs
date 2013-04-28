using System.ServiceModel;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
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
