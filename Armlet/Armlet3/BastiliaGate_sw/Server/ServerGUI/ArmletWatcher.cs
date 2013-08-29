using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using HonorInterfaces;

namespace ServerGUI
{
    public class ArmletWatcher : DependencyObject
    {

        public static readonly DependencyProperty RegenProperty =
            DependencyProperty.Register("Regen", typeof(string), typeof(ArmletWatcher), new PropertyMetadata(default(string)));

        public string Regen
        {
            get { return (string)GetValue(RegenProperty); }
            set { SetValue(RegenProperty, value); }
        }
        
        public static readonly DependencyProperty IdProperty =
            DependencyProperty.Register("Id", typeof (string), typeof (ArmletWatcher), new PropertyMetadata(default(string)));

        public string Id
        {
            get { return (string) GetValue(IdProperty); }
            set { SetValue(IdProperty, value); }
        }

        public static readonly DependencyProperty RoomProperty =
            DependencyProperty.Register("Room", typeof (string), typeof (ArmletWatcher), new PropertyMetadata(default(string)));

        public string Room
        {
            get { return (string) GetValue(RoomProperty); }
            set { SetValue(RoomProperty, value); }
        }

        public static readonly DependencyProperty BloodLevelProperty =
            DependencyProperty.Register("BloodLevel", typeof (string), typeof (ArmletWatcher), new PropertyMetadata(default(string)));

        public string BloodLevel
        {
            get { return (string) GetValue(BloodLevelProperty); }
            set { SetValue(BloodLevelProperty, value); }
        }

        public static readonly DependencyProperty ToxicLevelProperty =
            DependencyProperty.Register("ToxicLevel", typeof(string), typeof(ArmletWatcher), new PropertyMetadata(default(string)));

        public string ToxicLevel
        {
            get { return (string)GetValue(ToxicLevelProperty); }
            set { SetValue(ToxicLevelProperty, value); }
        }

        public static readonly DependencyProperty NameProperty =
            DependencyProperty.Register("Name", typeof (string), typeof (ArmletWatcher), new PropertyMetadata(default(string)));

        public string Name
        {
            get { return (string) GetValue(NameProperty); }
            set { SetValue(NameProperty, value); }
        }

        public static readonly DependencyProperty StatusProperty =
            DependencyProperty.Register("Status", typeof (string), typeof (ArmletWatcher), new PropertyMetadata(default(string)));

        private IArmletInfo _armletInfo;

        public IArmletInfo Value {get { return _armletInfo; }}

        public ArmletWatcher(IArmletInfo armletInfo)
        {
            Update(armletInfo);
            armletInfo.ArmletUpdated += () => Dispatcher.BeginInvoke(new Action(() => Update(armletInfo)));
        }

        private void Update(IArmletInfo armletInfo)
        {
            _armletInfo = armletInfo;
            Id = armletInfo.Id.ToString() + " / " + armletInfo.Id.ToString("X2");
            BloodLevel = armletInfo.BloodLevel.ToString();
            Name = armletInfo.Name;
            Status = armletInfo.Status;
            Room = armletInfo.RoomName;
            Regen = armletInfo.Regen.ToString();
            ToxicLevel = armletInfo.Toxic.ToString();
        }

        public string Status
        {
            get { return (string) GetValue(StatusProperty); }
            set { SetValue(StatusProperty, value); }
        }
    }
}
