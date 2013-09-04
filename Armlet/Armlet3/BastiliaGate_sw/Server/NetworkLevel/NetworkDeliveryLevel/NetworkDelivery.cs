using System;
using System.Net;
using System.ServiceModel;
using System.ServiceModel.Description;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    public static class  NetworkDelivery
    {
        private static readonly ServiceHost myServiceHost = null;
        private static InstanceContext instanceContext;

        public static readonly IGateDeliveryService GateDeliveryInstance = new GateDeliveryService();
        public static readonly IArmletDeliveryServece ArmletDeliveryInstance = new ArmletDeliveryService();
        public static readonly ShipDamageServiceCallback ShipDamageServiceInstance = new ShipDamageServiceCallback();
        


        static NetworkDelivery()
        {
            if (myServiceHost != null)
            {
                myServiceHost.Close();
            }

            myServiceHost = new ServiceHost(typeof(GateWcfService));
            myServiceHost.Open();
        }
        public static bool ConnectToSimulator()
        {
            try
            {
                instanceContext = new InstanceContext(new ShipDamageServiceCallback());
                ShipDamageServiceCallback.ShipDamageClient = new ShipDamageService.ServerDamageContractClient(instanceContext, "NetTcpBinding_IServerDamageContract");
                ShipDamageServiceCallback.ShipDamageClient.ServerConnect();
            }
            catch (Exception)
            {
                return false;
            }
            return true;
        }
    }
}
