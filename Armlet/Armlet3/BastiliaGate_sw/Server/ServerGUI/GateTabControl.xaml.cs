using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using HonorInterfaces;

namespace ServerGUI
{
    /// <summary>
    /// Interaction logic for GateTabControl.xaml
    /// </summary>
    public partial class GateTabControl
    {
        public IGlobalModel Model { private get; set; }

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
            Model.NewGateOnline += obj => Dispatcher.BeginInvoke(new Action(() => Model_NewGateOnline(obj)));
        }

        private readonly object _syncRoot = new object();

        void Model_NewGateOnline(byte gateId)
        {
            lock (_syncRoot)
            {
                if (_gates.Contains(gateId)) return;
                _gates.Add(gateId);
            }
            var gateUi = new GateUI {Model = Model.GetGateModel(gateId)};
            GatesTabControl.Items.Add(new TabItem {Content = gateUi, Header = "Gate " + gateId, IsSelected = true});
            
        }
    }
}
