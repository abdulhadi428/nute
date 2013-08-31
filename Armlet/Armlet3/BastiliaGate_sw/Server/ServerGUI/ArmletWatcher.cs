using System;
using System.Threading;
using System.Windows;
using HonorInterfaces;

namespace ServerGUI
{
    public class ArmletWatcher : DependencyObject
    {

        public static readonly DependencyProperty RegenProperty =
            DependencyProperty.Register("Regen", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));

        public string Regen
        {
            get { return (string) GetValue(RegenProperty); }
            private set { SetValue(RegenProperty, value); }
        }

        public static readonly DependencyProperty PoxinProperty =
            DependencyProperty.Register("Poxin", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));

        public string Poxin
        {
            get { return (string) GetValue(PoxinProperty); }
            set { SetValue(PoxinProperty, value); }
        }

        public static readonly DependencyProperty PulseProperty =
            DependencyProperty.Register("Pulse", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));


        public string Pulse
        {
            get { return (string) GetValue(PulseProperty); }
            private set { SetValue(PulseProperty, value); }
        }

        public static readonly DependencyProperty TempProperty =
            DependencyProperty.Register("Temp", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));

        public string Temp
        {
            get { return (string) GetValue(TempProperty); }
            private set { SetValue(TempProperty, value); }
        }

        public static readonly DependencyProperty IdProperty =
            DependencyProperty.Register("Id", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));


        public string Id
        {
            get { return (string) GetValue(IdProperty); }
            private set { SetValue(IdProperty, value); }
        }

        public static readonly DependencyProperty RoomProperty =
            DependencyProperty.Register("Room", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));

        public string Room
        {
            get { return (string) GetValue(RoomProperty); }
            private set { SetValue(RoomProperty, value); }
        }

        public static readonly DependencyProperty BloodLevelProperty =
            DependencyProperty.Register("BloodLevel", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));

        public string BloodLevel
        {
            get { return (string) GetValue(BloodLevelProperty); }
            private set { SetValue(BloodLevelProperty, value); }
        }

        public static readonly DependencyProperty ToxicLevelProperty =
            DependencyProperty.Register("ToxicLevel", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));

        public string ToxicLevel
        {
            get { return (string) GetValue(ToxicLevelProperty); }
            private set { SetValue(ToxicLevelProperty, value); }
        }

        public static readonly DependencyProperty NameProperty =
            DependencyProperty.Register("Name", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));

        public string Name
        {
            get { return (string) GetValue(NameProperty); }
            private set { SetValue(NameProperty, value); }
        }

        public static readonly DependencyProperty StatusProperty =
            DependencyProperty.Register("Status", typeof (string), typeof (ArmletWatcher),
                new PropertyMetadata(default(string)));

        public IArmletInfo Value { get; private set; }

        public ArmletWatcher(IArmletInfo armletInfo)
        {
            Update(armletInfo);
            armletInfo.ArmletUpdated += () => Dispatcher.BeginInvoke(new Action(() => Update(armletInfo)));
        }

        private void Update(IArmletInfo armletInfo)
        {
            Value = armletInfo;
            Id = string.Format("{0}\n0x{1:X2}", armletInfo.Id, armletInfo.Id);
            BloodLevel = armletInfo.BloodLevel.ToString(Thread.CurrentThread.CurrentCulture);
            Name = armletInfo.Name;
            Status = armletInfo.Status;
            Room = armletInfo.RoomName;
            Regen = armletInfo.Regen.ToString(Thread.CurrentThread.CurrentCulture);
            ToxicLevel = armletInfo.Toxic.ToString(Thread.CurrentThread.CurrentCulture);
            Pulse = armletInfo.Pulse.ToString(Thread.CurrentThread.CurrentCulture);
            Temp = armletInfo.Temp.ToString(Thread.CurrentThread.CurrentCulture);
        }

        public string Status
        {
            get { return (string) GetValue(StatusProperty); }
            private set { SetValue(StatusProperty, value); }
        }
    }
}