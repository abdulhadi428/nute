using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using PillInterfaces;

namespace ServerGUI
{
    public partial class ArmletGridUI
    {
        public IGlobalModel Model { get; set; }

        public ArmletGridUI()
        {
            InitializeComponent();
        }

        private void ArmletGridUI_OnLoaded(object sender, RoutedEventArgs e)
        {   
            if (Model == null)
            {
                return;
            }
            Model.ArmletListUpdated += () => Dispatcher.BeginInvoke(new Action(UpdateArmlets));
            UpdateArmlets();
        }

        private void UpdateArmlets()
        {
            ArmletGrid.ItemsSource = Model.GetArmlets();
        }

        private void UpdateName_OnClick(object sender, RoutedEventArgs e)
        {
            CurrentArmlet.SetName(NameTextBox.Text);
        }

        private IArmletInfo CurrentArmlet
        {
            get { return ArmletGrid.CurrentItem as IArmletInfo; }
        }

        private void SendMessage_Click(object sender, RoutedEventArgs e)
        {
            CurrentArmlet.SendMessage(MessageTextBox.Text);
        }

        private void UpdateRegeneration_Click(object sender, RoutedEventArgs e)
        {
            CurrentArmlet.SetRegeneration((byte)RegenLevelComboBox.SelectedValue);
        }

        private void UpdateLockList_OnClick(object sender, RoutedEventArgs e)
        {
            var list = (from object child in LockGrid.Children
                        select child as CheckBox
                        into checkBox 
                        where checkBox != null && checkBox.IsChecked == true 
                        select (byte) checkBox.Tag)
                .ToArray();

            CurrentArmlet.SetLockList(list);
        }
    }
}
