# vmware-helper(linux)
This program is for linux guest-pc

This program catch key-combination. And if caught key-combination is ctrl+win+(← or →). Then send command to host-pc to switch virtual desktop.

compile with this command
> gcc -lX11 vmware-helper.c -o vmware-helper

And execute program like this
> ./vmware-helper &lt;host VMnet1-IP&gt; &lt;PORT&gt;

If you want the program to run automatically on boot-time.

Check this page : [linux autostarting wiki](https://wiki.archlinux.org/index.php/Autostarting)

In kaliLnux - '/etc/xdg/autostart/' directory has boot-time run program description files.