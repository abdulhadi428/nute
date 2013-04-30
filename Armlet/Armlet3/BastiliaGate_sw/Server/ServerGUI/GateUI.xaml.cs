using System;
using System.Linq;
using System.Windows;
using PillInterfaces;

namespace ServerGUI
{
    /// <summary>
    /// Interaction logic for GateUI.xaml
    /// </summary>
    public partial class GateUI
    {
        public IGateModel Model { get; set; }


        public GateUI()
        {
            InitializeComponent();
        }

        private void ActivatePin_OnClick(object sender, RoutedEventArgs e)
        {
            Model.ActivatePin();
        }

        private void WritePillButton_Click(object sender, RoutedEventArgs e)
        {
            int charges;
            if (!int.TryParse(ChargesTextBox.Text, out charges))
            {
                MessageBox.Show("Заряды должны быть числом!");
                return;
            }
            if (charges < 0 || charges > 1000)
            {
                MessageBox.Show("Заряды должны быть от 0 до 1000");
                return;
            }
            Model.WritePill(((IPillType) PillTypeBox.SelectedItem).Id, charges);
        }

        private void GateUI_OnLoaded(object sender, RoutedEventArgs e)
        {
            Model.PillOnlineChanged += () => Dispatcher.BeginInvoke(new Action(UpdatePillOnline));
            Model.PillDataArrived += obj => Dispatcher.BeginInvoke(new Action<byte[]>(UpdatePillDataArrived), obj);
            Model.GateOnlineChanged += () => Dispatcher.BeginInvoke(new Action(UpdateGateOnline));

            UpdatePillOnline();
            UpdateGateOnline();
            PillTypeBox.ItemsSource = Model.PillTypes;
        }

        private void UpdateGateOnline()
        {
            IsEnabled = Model.Online;
        }

        private void UpdatePillOnline()
        {
            PillGroupBox.IsEnabled = Model.PillOnline;
        }

        private void UpdatePillDataArrived(byte[] obj)
        {
            PillContentBox.Text = string.Join(" ", obj.Select(b => b.ToString("X2")));
        }

        private void UpdateButton_Click(object sender, RoutedEventArgs e)
        {
            Model.RefreshPillStatus();
        }
    }
}
