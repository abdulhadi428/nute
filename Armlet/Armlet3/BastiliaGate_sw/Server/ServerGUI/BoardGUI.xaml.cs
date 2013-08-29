using System;
using System.Windows.Controls;
using HonorLogic.ShipStatus;
using HonorInterfaces;

namespace ServerGUI
{
    public partial class BoardGUI : UserControl
    {
        public ShipSubsystemStatus Status
        {
            get; set;
        }
        public void Update()
        {
            StatusListBox.SelectedIndex = (int) Status.Severity;
            StatusListBox.SelectionChanged += StatusListBoxOnSelectionChanged;
        }

        private void StatusListBoxOnSelectionChanged(object sender, SelectionChangedEventArgs selectionChangedEventArgs)
        {
            Status.Severity = (RanmaRepairSeverity) StatusListBox.SelectedIndex;
            if (StatusChanged != null)
            {
                StatusChanged(this, EventArgs.Empty);
            }
        }

        public event EventHandler StatusChanged;

        public BoardGUI()
        {
            InitializeComponent();
        }
    }
}
