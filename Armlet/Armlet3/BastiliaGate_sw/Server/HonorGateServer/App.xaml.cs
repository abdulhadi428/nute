using System.Windows;
using System.Windows.Threading;

namespace HonorGateServer
{
    public partial class App
    {
        private void App_OnDispatcherUnhandledException(object sender, DispatcherUnhandledExceptionEventArgs e)
        {
            MessageBox.Show(e.Exception.StackTrace);
        }
    }
}
