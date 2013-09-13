using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using HonorInterfaces;

namespace ServerGUI
{
    /// <summary>
    /// Interaction logic for ShipControl.xaml
    /// </summary>
    public partial class ShipControl
    {
        public ShipControl()
        {
            InitializeComponent();
        }

        public ArmletGridUI parent;
        public IShip Ship { private get; set; }
        private readonly List<BoardGUI> _boards = new List<BoardGUI>();

        private void ShipControl_OnLoaded(object sender, RoutedEventArgs e)
        {
            Ship.SubsystemUpdated += status => Dispatcher.BeginInvoke(new Action(() => Ship_SubsystemUpdated(status)));
            Ship.OnlineChanged += () => Dispatcher.BeginInvoke(new Action(Ship_OnlineChanged));
            MainGroupBox.Header = Ship.Name;
            foreach (var shipSubsystemStatuse in Ship.GetAllSubsystemsStatus())
            {
                var board = new BoardGUI
                {
                    Status = shipSubsystemStatuse,
                    SubsystemName = Ship.GetSubsystemName(shipSubsystemStatuse.SubSystemNum)
                    
                };
                board.StatusChanged += board_StatusChanged;
                _boards.Add(board);
                BoardStackPanel.Children.Add(board);
                board.Update();
            }
            foreach (var shipRoomsID in Ship.ShipRoomsIDs)
            {
                var button = new Button
                    {
                        Content = Ship.GetRoomName(shipRoomsID),
                    };
                byte id = shipRoomsID; //Access to local closure. C# 5.0 will fix it
                button.Click += (o, args) => ClickRoomHit(id);
                HitStackPanel.Children.Add(button);
            }
            Ship_OnlineChanged();
        }

        public void ClickRoomHit(byte shipRoomsId)
        {
            Ship.DamageRoom(shipRoomsId, (byte) (parent.RoomHit_Percentage.SelectedIndex + 1), (byte) (parent.RoomHit_Type.SelectedIndex + 1));
        } 

        void Ship_OnlineChanged()
        {
            OnlineLabel.Content = Ship.IsOnline ? "Онлайн" : "Оффлайн";
            BoardStackPanel.IsEnabled = Ship.IsOnline;
        }

        void Ship_SubsystemUpdated(ShipSubsystemStatus obj)
        {
            var board = _boards.Single(b => b.Status.SubSystemNum == obj.SubSystemNum);
            board.Status = obj;
            board.Update();
        }

        void board_StatusChanged(object sender, System.EventArgs e)
        {
            var board = (BoardGUI) sender;
            Ship.SetSubsystemStatusFromGUI(board.Status);
        }

        private void HitClick(object sender, RoutedEventArgs e)
        {
            var hitPower = byte.Parse(((Button) sender).Content.ToString());
            Ship.DamageShip(hitPower);
        }
    }
}
