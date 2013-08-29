using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;using HonorInterfaces;

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
        }
    }
}
