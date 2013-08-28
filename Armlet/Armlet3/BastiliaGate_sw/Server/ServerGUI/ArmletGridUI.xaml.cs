using System.Collections.ObjectModel;
 using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
 using System.Windows.Input;
 using PillInterfaces;

namespace ServerGUI
{
    public partial class ArmletGridUI
    {
        public IGlobalModel Model { private get; set; }

        public ObservableCollection<ArmletWatcher> Source11 { get; set; }

        public ArmletGridUI()
        {
            InitializeComponent();
            Source11 = new ObservableCollection<ArmletWatcher>();
        }

        private void ArmletGridUI_OnLoaded(object sender, RoutedEventArgs e)
        {   
            if (Model == null)
            {
                return;
            }
            foreach (var armletInfo in Model.GetArmlets())
            {
                Source11.Add(new ArmletWatcher(armletInfo));
            }
            ArmletGrid.ItemsSource = Source11;
            Task.Factory.StartNew(() =>
                {
                    Thread.Sleep(5000);
              //      Model.UpdateAllNames();
                });

        }

        private void UpdateName_OnClick(object sender, RoutedEventArgs e)
        {
            if (CurrentArmlet == null) //FIXME Disable button when nothing selected
            {
                MessageBox.Show("Выберите браслет!");
                return;
            }
            Model.SendSetName(NameTextBox.Text, CurrentArmlet);
        }

        private IArmletInfo CurrentArmlet
        {
            get { 
                var selectemArmlet = ArmletGrid.SelectedItem;
                if (selectemArmlet == null)
                {
                    return null;
                }
                return ((ArmletWatcher)selectemArmlet).Value; }
        }

        private void SendMessage_Click(object sender, RoutedEventArgs e)
        {
        if (CurrentArmlet == null) //FIXME Disable button when nothing selected
              {
                MessageBox.Show("Выберите браслет!");
                return;
              }

            Model.SendShowMessage(CurrentArmlet, MessageTextBox.Text);
        }

        private void UpdateRegeneration_Click(object sender, RoutedEventArgs e)
        {
            if (CurrentArmlet == null) //FIXME Disable button when nothing selected
            {
                MessageBox.Show("Выберите браслет!");
                return;
            }
            Model.SetSetPlayerRegen(CurrentArmlet, (byte) RegenLevelComboBox.SelectedValue);
        }

        private void UpdateLockList_OnClick(object sender, RoutedEventArgs e)
        {
                if (CurrentArmlet == null) //FIXME Disable button when nothing selected
              {
                MessageBox.Show("Выберите браслет!");
                return;
              }
              
            var list = (from object child in LockGrid.Children
                        select child as CheckBox
                        into checkBox 
                        where checkBox != null && checkBox.IsChecked == true 
                        select (byte) checkBox.Tag)
                .ToArray();

            Model.SendSetLockList(CurrentArmlet, list);
        }

        private void SendAll_Click(object sender, RoutedEventArgs e)
        {
            var saveCursor = Cursor;
            try
            {
                Cursor = Cursors.Wait;
                Model.SendMessageToAll(MessageTextBox.Text);
            }
            finally
            {
                Cursor = saveCursor;
            }
        }

        private void HitRoom_Click(object sender, RoutedEventArgs e)
        {
            var selectedRoom = RoomComboBox.SelectedItem;
            if (selectedRoom == null)
            {
                MessageBox.Show("Выберите отсек!");
                return;
            }
            Model.SendRoomHit(byte.Parse(selectedRoom.ToString()));
            
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            Model.UpdateAllNames();
        }
    }
}
