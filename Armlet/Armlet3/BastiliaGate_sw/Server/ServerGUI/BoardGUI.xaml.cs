using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using HonorInterfaces;

namespace ServerGUI
{
    public partial class BoardGUI
    {
        public ShipSubsystemStatus Status
        {
            get; set;
        }
        public void Update()
        {
            StatusListBox.SelectedIndex = (int) Status.Severity;
            UpdateBackground();
        }

        private void UpdateBackground()
        {
            StatusListBox.Background = new SolidColorBrush(GetSeverityColor(Status.Severity));
        }

        private Color GetSeverityColor(RanmaRepairSeverity severity)
        {
            switch (severity)
            {
                case RanmaRepairSeverity.Ready:
                    return Colors.GreenYellow;
                case RanmaRepairSeverity.Easy:
                    return Colors.Yellow;
                case RanmaRepairSeverity.Medium:
                    return Colors.HotPink;
                case RanmaRepairSeverity.Hard:
                    return Colors.OrangeRed;
                default:
                    throw new ArgumentException("severity");
            }
        }

        private void StatusListBoxOnSelectionChanged(object sender, SelectionChangedEventArgs selectionChangedEventArgs)
        {
            Status.Severity = (RanmaRepairSeverity) StatusListBox.SelectedIndex;
            if (StatusChanged != null)
            {
                StatusChanged(this, EventArgs.Empty);
            }
            UpdateBackground();
        }

        public event EventHandler StatusChanged;

        public BoardGUI()
        {
            InitializeComponent();
        }

        private void BoardGUI_OnLoaded(object sender, RoutedEventArgs e)
        {
            StatusListBox.SelectionChanged += StatusListBoxOnSelectionChanged;
        }
    }
}
