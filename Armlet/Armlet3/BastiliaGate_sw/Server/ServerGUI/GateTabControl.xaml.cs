using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using PillInterfaces;

namespace ServerGUI
{
    /// <summary>
    /// Interaction logic for GateTabControl.xaml
    /// </summary>
    public partial class GateTabControl : UserControl
    {
        public IGlobalModel Model { get; set; }

        private readonly List<byte> _gates = new List<byte>();
        public GateTabControl()
        {
            InitializeComponent();
        }

        private void GateTabControl_OnLoaded(object sender, RoutedEventArgs e)
        {
            if (Model == null)
            {
                return;
            }
            Model.NewGateOnline += (obj) => Dispatcher.BeginInvoke(new Action<byte>(Model_NewGateOnline));
        }

        private readonly object _syncRoot = new object();

        void Model_NewGateOnline(byte gateId)
        {
            lock (_syncRoot)
            {
                if (!_gates.Contains(gateId))
                {
                    _gates.Add(gateId);
                    var gateUi = new GateUI {Model = Model.GetGateModel(gateId)};
                    GatesTabControl.Items.Add(new TabItem {Content = gateUi});
                }
            }
        }
    }
}
