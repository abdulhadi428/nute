using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Media3D;
using HonorInterfaces;

namespace ServerGUI
{
    /// <summary>
    /// Interaction logic for ShipControl.xaml
    /// </summary>
    public partial class ShipControl : UserControl
    {
        public ShipControl()
        {
            InitializeComponent();
        }

        public IShip Ship { get; set; }
        private readonly List<BoardGUI> _boards = new List<BoardGUI>();

        private void ShipControl_OnLoaded(object sender, RoutedEventArgs e)
        {
            MainGroupBox.Header = Ship.Name;
            foreach (var shipSubsystemStatuse in Ship.GetAllSubsystemsStatus())
            {
                var board = new BoardGUI();
                _boards.Add(board);
                BoardStackPanel.Children.Add(board);
                board.Update(shipSubsystemStatuse);
            }
            foreach (var roomsID in Ship.ShipRoomsIDs)
            {
                CreateButton(roomsID);
            }
        }

        private void CreateButton(byte roomsID)
        {
            var button = new Button()
            {
                Content = "Отсек " + roomsID
            };
            button.Click += (o, args) => Ship.Model.SendRoomHit(roomsID, 50);
            HitStackPanel.Children.Add(button);
        }
    }
}
