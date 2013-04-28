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
            var globalModel = new GlobalModel(NetworkDelivery.ArmletDeliveryInstance, NetworkDelivery.GateDeliveryInstance);
            ArmletUI.Model = globalModel;
            GateUI.Model = globalModel;
        }
    }
}
