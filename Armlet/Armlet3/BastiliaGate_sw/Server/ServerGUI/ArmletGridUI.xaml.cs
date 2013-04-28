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
    /// Interaction logic for ArmletGridUI.xaml
    /// </summary>
    public partial class ArmletGridUI : UserControl
    {
        public IGlobalModel Model { get; set; }

        public ArmletGridUI()
        {
            InitializeComponent();
        }

        private void ArmletGridUI_OnLoaded(object sender, RoutedEventArgs e)
        {   
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
    }
}
