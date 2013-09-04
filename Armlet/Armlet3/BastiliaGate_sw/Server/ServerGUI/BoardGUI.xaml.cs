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

        public string SubsystemName { get; set; }

        public void Update()
        {
            DamageListBox.SelectedIndex = (int) Status.Severity;
            StatusLabel.Content = Status.RepairedStatus.ToString("X4");
            UpdateBackground();
        }

        private void UpdateBackground()
        {
            DamageListBox.Background = new SolidColorBrush(GetSeverityColor(Status.Severity));
            StatusLabel.Background = new SolidColorBrush(Status.Repaired ? Colors.Green : Colors.Red);
        }

        private Color GetSeverityColor(RanmaRepairSeverity severity)
        {
            switch (severity)
            {
                case RanmaRepairSeverity.NotDamaged:
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

        private void DamageListBoxOnSelectionChanged(object sender, SelectionChangedEventArgs selectionChangedEventArgs)
        {
            Status.Severity = (RanmaRepairSeverity) DamageListBox.SelectedIndex;
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
            DamageListBox.SelectionChanged += DamageListBoxOnSelectionChanged;
            SubsystemLabel.Content = SubsystemName;
        }
    }
}
