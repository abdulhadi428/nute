using System.Net;
using System.ServiceModel;
using System.ServiceModel.Description;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    public static class  NetworkDelivery
    {
        private static readonly ServiceHost myServiceHost = null; 

        public static readonly IGateDeliveryService GateDeliveryInstance = new GateDeliveryService();
        public static readonly IArmletDeliveryServece ArmletDeliveryInstance = new ArmletDeliveryService();


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
