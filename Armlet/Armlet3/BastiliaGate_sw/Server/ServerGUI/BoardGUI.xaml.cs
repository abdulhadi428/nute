using System.Windows.Controls;
using HonorLogic.ShipStatus;
using HonorInterfaces;

namespace ServerGUI
{
    public partial class BoardGUI : UserControl
    {
        public void Update(ShipSubsystemStatus status)
        {
            StatusListBox.SelectedIndex = (int) status.Severity;
        }

        public BoardGUI()
        {
            InitializeComponent();
        }
    }
}
