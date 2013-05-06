using System.Windows;
using HonorLogic;
using NetworkLevel.NetworkDeliveryLevel;

namespace HonorGateServer
{
    public partial class MainWindow
    {        
        public MainWindow()
        {
            InitializeComponent();
        }

        private void MainWindow_OnLoaded(object sender, RoutedEventArgs e)
        {
            Application.Current.DispatcherUnhandledException  += Current_DispatcherUnhandledException;
            var globalModel = new GlobalModel(NetworkDelivery.ArmletDeliveryInstance, NetworkDelivery.GateDeliveryInstance);
            ArmletUI.Model = globalModel;
            ArmletUI.GateUI.Model = globalModel;
        }

        void Current_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            MessageBox.Show(e.Exception.Message);
        }
    }
}
