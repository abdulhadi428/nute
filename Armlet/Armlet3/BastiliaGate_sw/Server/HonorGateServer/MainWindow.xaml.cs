using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ServiceModel;
using HonorGateServer.NetworkDeliveryLevel;
using HonorGateServer.WCFServices;


namespace HonorGateServer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        internal static ServiceHost myServiceHost = null; 
        private NetworkDeliveryLevel.ArmletDeliveryService armletDelivery;

        public MainWindow()
        {
            InitializeComponent();

            if (myServiceHost != null)
            {
                myServiceHost.Close();
            }

            myServiceHost = new ServiceHost(typeof(GateWcfService));
            myServiceHost.Open();


            

            //armletDelivery = new ArmletDeliveryService();

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            GateConnectionPool.GateConnections.FirstOrDefault(g => g.Key == 1).Value.CallbackObject.SetGateId(22);
        }
    }
}
