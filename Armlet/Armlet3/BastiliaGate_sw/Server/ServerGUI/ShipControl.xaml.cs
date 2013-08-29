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
        private BoardGUI[] _boards;

        private void ShipControl_OnLoaded(object sender, RoutedEventArgs e)
        {
            _boards = new BoardGUI[Ship.BoardCount];

        }
    }
}
