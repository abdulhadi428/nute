using System.ServiceModel;

namespace NetworkLevel.WCFServices
{

    public class PlayerUpdate
    {
        public byte ArmletID { get; set; }
        public byte NewRoom { get; set; }
        public byte NewBlood { get; set; }
    }


    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IGateWCFService" in both code and config file together.
    [ServiceContract(SessionMode = SessionMode.Required,
                 CallbackContract = typeof(IGateWCFServiceCallback))]
    public interface IGateWCFService
    {
        [OperationContract]
        byte RegisterGate (byte preferedGateId);

        [OperationContract(IsOneWay = true)]
        void TXCompleted(byte armlet_id);

        [OperationContract(IsOneWay = true)]
        void ArmlteStatusUpdate(PlayerUpdate[] updates);

        [OperationContract(IsOneWay = true)]
        void ArmletSendsData(byte armlet_id, byte[] payload);

        [OperationContract(IsOneWay = true)]
        void PillConnectionStatus(byte[] isConnectionResultData);

        [OperationContract(IsOneWay = true)]
        void PillWriteCompleted(byte[] writingResultData);

        [OperationContract(IsOneWay = true)]
        void PillDataRead(byte[] readingResultData);

        [OperationContract(IsOneWay = true)]
        void PinSet(byte[] pinSetResultData);

    }


    public interface IGateWCFServiceCallback
    {
       
        [OperationContract(IsOneWay = true)]
        void SendToArmlet(byte armlet_id, byte[] payload);

        [OperationContract(IsOneWay = true)]
        void CheckIfPillConnected(byte[] pillConnectedData);

        [OperationContract(IsOneWay = true)]
        void SendPillWrite(byte[] pillWriteData);

        [OperationContract(IsOneWay = true)]
        void SendPillRead(byte[] pillReadData);

        [OperationContract(IsOneWay = true)]
        void SendPinSignal(byte[] sendPinSignalData);

    }
}
