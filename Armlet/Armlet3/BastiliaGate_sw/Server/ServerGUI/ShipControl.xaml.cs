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
                    
                };
                board.StatusChanged += board_StatusChanged;
                _boards.Add(board);
                BoardStackPanel.Children.Add(board);
                board.Update();
            }
            foreach (var roomsID in Ship.ShipRoomsIDs)
            {
                CreateButton(roomsID);
            }
            Ship_OnlineChanged();
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
            Ship.SetSubsystemStatus(board.Status);
        }

        private void CreateButton(byte roomsID)
        {
            var button = new Button
            {
                Content = Ship.GetRoomName(roomsID)
            };
            button.Click += (o, args) => Ship.Model.SendRoomHit(roomsID, 50);
            HitStackPanel.Children.Add(button);
        }

        private void RandomHitClick(object sender, RoutedEventArgs e)
        {
            Ship.HitRandomRoom(RanmaRepairSeverity.Medium);
        }
    }
}
