using System.ServiceModel;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    public static class  NetworkDelivery
    {
        internal static ServiceHost myServiceHost = null; 

        public static IGateDeliveryService GateDeliveryInstance = new GateDeliveryService();
        public static IArmletDeliveryServece ArmletDeliveryInstance = new ArmletDeliveryService();
        
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
