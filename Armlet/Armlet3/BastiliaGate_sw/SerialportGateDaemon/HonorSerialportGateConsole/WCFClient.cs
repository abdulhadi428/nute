using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HonorSerialportGateConsole.Interfaces;
using HonorSerialportGateConsole.ServerWCFService;

namespace HonorSerialportGateConsole
{
    public static class WCFClient
    {
        public static ServerWCFService.GateWCFServiceClient Client;
        
    }

    public class WCFCallbackHandler: ServerWCFService.IGateWCFServiceCallback
    {
        private HonorSerialportDaemon _daemon;
        public WCFCallbackHandler(HonorSerialportDaemon daemon)
        {
            _daemon = daemon;
        }

        public void SendToArmlet(byte armlet_id, byte[] payload)
        {
            string hexString = Command.ByteArrayToHexString(new byte[] {armlet_id}.Concat(payload).ToArray());
            _daemon.inputMessageQueue.Enqueue(hexString);
        }

        public void CheckIfPillConnected(byte[] pillConnectedData)
        {
            throw new NotImplementedException();
        }

        public void SendPillWrite(byte[] pillWriteData)
        {
            throw new NotImplementedException();
        }

        public void SendPillRead(byte[] pillReadData)
        {
            throw new NotImplementedException();
        }

        public void SendPinSignal(byte[] sendPinSignalData)
        {
            throw new NotImplementedException();
        }
    }
}
