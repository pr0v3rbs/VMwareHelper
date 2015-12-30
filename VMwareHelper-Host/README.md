# VMwareHelper-Host
This is VMwareHelper-Host program.

This program create server with 'VMnet1-IP'. And receive guest-pc command from 'VMwareHelper-guest'

And execute program like this
> ./VMwareHelper &lt;VMnet1-IP&gt; &lt;PORT&gt;

If you want the program to run automatically on boot-time.

Check this registry key 'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run'