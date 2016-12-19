using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using WindowsDesktop;
using System.Runtime.InteropServices;

namespace VMwareHelper
{
    class Server
    {
        byte[] Buffer;
        Socket sck;
        private volatile bool _shouldStop;

        [DllImport("user32.dll")]
        private static extern void keybd_event(byte bVk, byte bScan, uint dwFlags, int dwExtraInfo);

        public void DoWork()
        {
            sck = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            sck.Bind(new IPEndPoint(IPAddress.Parse("192.168.163.1"), 28888));
            //sck.Bind(new IPEndPoint(IPAddress.Any, 28888));
            sck.Listen(10);
            Buffer = new byte[10];

            while (!_shouldStop)
            {
                Socket accepted = sck.Accept();
                int bytesRead = accepted.Receive(Buffer);
                switch (Buffer[0])
                {
                    case (byte)'L':
                        VirtualDesktop.Current.GetLeft()?.Switch();
                        //System.Diagnostics.Debug.WriteLine("Left");
                        break;
                    case (byte)'R':
                        VirtualDesktop.Current.GetRight()?.Switch();
                        //System.Diagnostics.Debug.WriteLine("Right");
                        break;
                    case (byte)'U':
                        //System.Diagnostics.Debug.WriteLine("Up");
                        keybd_event(0xA2/*VK_LCONTROL*/, 0, 0x0001/*KEYEVENTF_EXTENDEDKEY*/, 0);
                        keybd_event(0xA4/*VK_LMENU*/, 0, 0x0001/*KEYEVENTF_EXTENDEDKEY*/, 0);
                        keybd_event(0xA2/*VK_LCONTROL*/, 0, 0x0002/*KEYEVENTF_KEYUP*/, 0);
                        keybd_event(0xA4/*VK_LMENU*/, 0, 0x0002/*KEYEVENTF_KEYUP*/, 0);
                        break;
                    default:
                        break;
                }

                accepted.Close();
            }

            sck.Close();
        }

        public void Stop()
        {
            _shouldStop = true;
        }
    }
}
