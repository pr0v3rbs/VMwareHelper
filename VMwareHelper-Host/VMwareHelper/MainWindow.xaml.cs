using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WindowsDesktop;

namespace VMwareHelper
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Server server;
        Thread thread;

        public MainWindow()
        {
            this.InitializeComponent();
            this.Closing += MainWindow_Closing;

            foreach (var id in VirtualDesktop.GetDesktops().Select(x => x.Id))
            {
                System.Diagnostics.Debug.WriteLine(id);
            }

            server = new Server();
            thread = new Thread(server.DoWork);
            thread.Start();

            //this.Hide();
        }

        void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            server.Stop();

            IPEndPoint ipep = new IPEndPoint(IPAddress.Parse("192.168.163.1"), 28888);
            Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            client.Connect(ipep);
            Byte[] data = new Byte[1];
            data[0] = (byte)'Z';
            client.Send(data);

            thread.Join();
        }
    }
}
