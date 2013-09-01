using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using HonorSerialportGateConsole.Interfaces;
using HonorSerialportGateConsole.ServerWCFService;

namespace HonorSerialportGateConsole
{
    public static class WCFClient
    {
        public static ServerWCFService.GateWCFServiceClient Client;


    }

    public class WCFCallbackHandler: IGateWCFServiceCallback, IDisposable
    {
        private readonly HonorSerialportDaemon _daemon;
        private DateTime _lastHeartBeatTime = DateTime.Now;
        private readonly Timer _timer;
        private readonly int _heartBeatTimeoutSeconds;

        private void WatchDog(object state)
        {
            if (DateTime.Now.Subtract(_lastHeartBeatTime).TotalSeconds > _heartBeatTimeoutSeconds)
            {
                Environment.Exit(1);
            }
        }


        public WCFCallbackHandler(HonorSerialportDaemon daemon, int heartBeatTimeoutSeconds)
        {
            _daemon = daemon;
            _heartBeatTimeoutSeconds = heartBeatTimeoutSeconds;
            _timer = new Timer(WatchDog, null, 15 *1000, 2*1000);
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

        public void SendHeartbeat()
        {
            _lastHeartBeatTime = DateTime.Now;
        }

        public IAsyncResult BeginSendHeartbeat(AsyncCallback callback, object asyncState)
        {
            throw new NotImplementedException();
        }

        public void EndSendHeartbeat(IAsyncResult result)
        {
            throw new NotImplementedException();
        }

        public void Dispose()
        {
            if (_timer != null)
            {
                _timer.Dispose();
            }
        }
    }
}
