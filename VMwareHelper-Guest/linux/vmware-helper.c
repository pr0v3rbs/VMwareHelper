#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>

char IP[40];
unsigned short PORT;

Bool ParseArgument(int argc, char** argv)
{
    Bool result = False;

    if (argc == 3)
    {
        strncpy(IP, argv[1], strlen(argv[1]));
        PORT = atoi(argv[2]);
        result = True;
    }

    return result;
}

Bool SendCommand(char cmd)
{
    Bool result = False;
    int serverSocket;
    struct sockaddr_in serverAddr;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket >= 0)
    {
        bzero(&serverAddr, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        inet_pton(AF_INET, IP, &serverAddr.sin_addr);
        serverAddr.sin_port = htons(PORT);

        if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) >= 0)
        {
            if (write(serverSocket, &cmd, 1) == 1)
            {
                result = True;
            }
        }

        close(serverSocket);
    }

    return result;
}

int main(int argc, char** argv)
{
    if (ParseArgument(argc, argv))
    {
        Display *d = XOpenDisplay(0);
        Window rootWindow = DefaultRootWindow(d);
        XEvent ev;

        Bool            isMoved         = False;
        unsigned int    modifiers       = ControlMask | Mod4Mask;
        int             keycode         = XKeysymToKeycode(d, XK_Right);
        Window          grab_window     = rootWindow;
        Bool            owner_events    = False;
        int             pointer_mode    = GrabModeAsync;
        int             keyboard_mode   = GrabModeAsync;
        KeySym          sym;

        XGrabKey(d, keycode, modifiers, grab_window, owner_events, pointer_mode, keyboard_mode);

        keycode = XKeysymToKeycode(d, XK_Left);
        XGrabKey(d, keycode, modifiers, grab_window, owner_events, pointer_mode, keyboard_mode);
        XSelectInput(d, rootWindow, KeyPressMask | KeyReleaseMask);

        modifiers = ControlMask;
        keycode = XKeysymToKeycode(d, XK_Super_L);
        XGrabKey(d, keycode, modifiers, grab_window, owner_events, pointer_mode, keyboard_mode);
        XSelectInput(d, rootWindow, KeyReleaseMask);

        while(1)
        {
            XNextEvent(d, &ev);
            switch(ev.type)
            {
                case KeyPress:
                    sym = XKeycodeToKeysym(d, ev.xkey.keycode, 0);
                    if (sym == (XK_Right))
                    {
                        isMoved = True;
                        SendCommand('R');
                    }
                    else if (sym == XK_Left)
                    {
                        isMoved = True;
                        SendCommand('L');
                    }
                    break;

                case KeyRelease:
                    sym = XKeycodeToKeysym(d, ev.xkey.keycode, 0);
                    if (isMoved && sym == (XK_Control_L | XK_Super_L))
                    {
                        isMoved = False;
                        SendCommand('U');
                    }
                    break;

                default:
                    break;
            }
        }

        XCloseDisplay(d);
    }

    return 0;
}
