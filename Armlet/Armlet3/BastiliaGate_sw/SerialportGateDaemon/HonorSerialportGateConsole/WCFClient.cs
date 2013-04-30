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
            var sendCommand = new ServerToArmletCommand(ServerToArmletCommands.SendDataToArmlet,
                                                         new byte[] {armlet_id}.Concat(payload).ToArray());
            _daemon.inputMessageQueue.Enqueue(sendCommand);
        }

        public IAsyncResult BeginSendToArmlet(byte armlet_id, byte[] payload, AsyncCallback callback, object asyncState)
        {
            throw new NotImplementedException();
        }

        public void EndSendToArmlet(IAsyncResult result)
        {
            throw new NotImplementedException();
        }

        public void CheckIfPillConnected(byte[] pillConnectedData)
        {
            var ifPillCommand = new ServerToGateCommand(ServerToGateCommands.GetPillStatus, pillConnectedData);
            _daemon.inputMessageQueue.Enqueue(ifPillCommand);
        }

        public IAsyncResult BeginCheckIfPillConnected(byte[] pillConnectedData, AsyncCallback callback, object asyncState)
        {
            throw new NotImplementedException();
        }

        public void EndCheckIfPillConnected(IAsyncResult result)
        {
            throw new NotImplementedException();
        }

        public void SendPillWrite(byte[] pillWriteData)
        {
            var pillWriteCommand = new ServerToGateCommand(ServerToGateCommands.WriteDataToPill, pillWriteData);
            _daemon.inputMessageQueue.Enqueue(pillWriteCommand);
        }

        public IAsyncResult BeginSendPillWrite(byte[] pillWriteData, AsyncCallback callback, object asyncState)
        {
            throw new NotImplementedException();
        }

        public void EndSendPillWrite(IAsyncResult result)
        {
            throw new NotImplementedException();
        }

        public void SendPillRead(byte[] pillReadData)
        {
            var pillReadCommand = new ServerToGateCommand(ServerToGateCommands.ReadDataFromPill, pillReadData);
            _daemon.inputMessageQueue.Enqueue(pillReadCommand);
        }

        public IAsyncResult BeginSendPillRead(byte[] pillReadData, AsyncCallback callback, object asyncState)
        {
            throw new NotImplementedException();
        }

        public void EndSendPillRead(IAsyncResult result)
        {
            throw new NotImplementedException();
        }

        public void SendPinSignal(byte[] sendPinSignalData)
        {
            var pinCommand = new ServerToGateCommand(ServerToGateCommands.InitiatePinSignal, sendPinSignalData);
            _daemon.inputMessageQueue.Enqueue(pinCommand);
        }

        public IAsyncResult BeginSendPinSignal(byte[] sendPinSignalData, AsyncCallback callback, object asyncState)
        {
            throw new NotImplementedException();
        }

        public void EndSendPinSignal(IAsyncResult result)
        {
            throw new NotImplementedException();
        }
    }
}
