using Microsoft.Win32;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace StereoscopicMoviePlayer
{
    class Settings
    {
        private static Point LocationValue;
        private static Size SizeValue;
        private static bool VisibleValue = true;
        private static bool AlwaysOnTopValue = true;
        private static bool RunAtStartupValue = false;
        private static string FilePathValue = string.Empty;
        private static string ComPortValue = "COM1";
        private static int GlassesTimeOffsetValue = 0;
        private static bool SoundOnValue = true;
        private static int VolumeValue = 32768;
        private static bool SwapLRValue = false;
        private static bool VerticalLRValue = false;
        private static int LRBothValue = 0;

        //Location Property.
        public static Point Location
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                LocationValue = new Point((int)key.GetValue("Left", 0), (int)key.GetValue("Top", 0));
                return (LocationValue);
            }
            set
            {
                LocationValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("Left", LocationValue.X);
                key.SetValue("Top", LocationValue.Y);
            }
        }

        //Size Property.
        public static Size Size
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                SizeValue = new Size((int)key.GetValue("Width", 352), (int)key.GetValue("Height", 370));
                return (SizeValue);
            }
            set
            {
                SizeValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("Width", SizeValue.Width);
                key.SetValue("Height", SizeValue.Height);
            }
        }

        //Visible Property.
        public static bool Visible
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                VisibleValue = Boolean.Parse((string)key.GetValue("Visible", "True"));
                return (VisibleValue);
            }
            set
            {
                VisibleValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("Visible", VisibleValue.ToString());
            }
        }

        //Always On Top Property.
        public static bool AlwaysOnTop
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                AlwaysOnTopValue = Boolean.Parse((string)key.GetValue("AlwaysOnTop", "True"));
                return (AlwaysOnTopValue);
            }
            set
            {
                AlwaysOnTopValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("AlwaysOnTop", AlwaysOnTopValue.ToString());
            }
        }

        //Run At Startup Property.
        public static bool RunAtStartup
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                RunAtStartupValue = Boolean.Parse((string)key.GetValue("RunAtStartup", "False"));
                //If application executable file location has been changed.
                RegistryKey runKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                if (RunAtStartupValue) runKey.SetValue(GetAssemblyInfo.AssemblyProduct, Application.ExecutablePath.ToString());
                else runKey.DeleteValue(GetAssemblyInfo.AssemblyProduct, false);
                return (RunAtStartupValue);
            }
            set
            {
                RunAtStartupValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("RunAtStartup", RunAtStartupValue.ToString());
                RegistryKey runKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                if (RunAtStartupValue) runKey.SetValue(GetAssemblyInfo.AssemblyProduct, Application.ExecutablePath.ToString());
                else runKey.DeleteValue(GetAssemblyInfo.AssemblyProduct, false);
            }
        }

        // Left Image Path
        public static string FilePath
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                FilePathValue = (string)key.GetValue("FilePath", string.Empty);
                return (FilePathValue);
            }
            set
            {
                FilePathValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("FilePath", FilePathValue);
            }
        }

        // Com Port
        public static string ComPort
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                ComPortValue = (string)key.GetValue("ComPort", "COM1");
                return (ComPortValue);
            }
            set
            {
                ComPortValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("ComPort", ComPortValue);
            }
        }

        //Glasses Time Offset
        public static int GlassesTimeOffset
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                GlassesTimeOffsetValue = (int)key.GetValue("GlassesTimeOffset", 0);
                return (GlassesTimeOffsetValue);
            }
            set
            {
                GlassesTimeOffsetValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("GlassesTimeOffset", GlassesTimeOffsetValue);
            }
        }

        //SoundOn Property.
        public static bool SoundOn
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                SoundOnValue = Boolean.Parse((string)key.GetValue("SoundOn", "True"));
                return (SoundOnValue);
            }
            set
            {
                SoundOnValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("SoundOn", SoundOnValue.ToString());
            }
        }

        //Volume Property.
        public static int Volume
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                VolumeValue = (int)key.GetValue("Volume", 32768);
                return (VolumeValue);
            }
            set
            {
                VolumeValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("Volume", VolumeValue);
            }
        }

        //SwapLR Property.
        public static bool SwapLR
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                SwapLRValue = Boolean.Parse((string)key.GetValue("SwapLR", "False"));
                return (SwapLRValue);
            }
            set
            {
                SwapLRValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("SwapLR", SwapLRValue.ToString());
            }
        }

        //VerticalLR Property.
        public static bool VerticalLR
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                VerticalLRValue = Boolean.Parse((string)key.GetValue("VerticalLR", "False"));
                return (VerticalLRValue);
            }
            set
            {
                VerticalLRValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("VerticalLR", VerticalLRValue.ToString());
            }
        }

        //LRBoth Property.
        public static int LRBoth
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                LRBothValue = (int)key.GetValue("LRBoth", 0);
                return (LRBothValue);
            }
            set
            {
                LRBothValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("LRBoth", LRBothValue);
            }
        }
    }
}
