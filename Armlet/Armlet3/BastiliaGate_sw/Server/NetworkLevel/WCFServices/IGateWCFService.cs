using System.ServiceModel;

namespace NetworkLevel.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IGateWCFService" in both code and config file together.
    [ServiceContract(SessionMode = SessionMode.Required,
                 CallbackContract = typeof(IGateWCFServiceCallback))]
    public interface IGateWCFService
    {
        [OperationContract]
        int RegisterGate (int preferedGateId);
    }


    public interface IGateWCFServiceCallback
    {
        [OperationContract]
        void SetGateId(int newGateId);
    }
}
