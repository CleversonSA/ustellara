# uStellara SDR Receiver (by Cleverson S A - PU5TOS)

A radio user frontend for the greate osmosdr rtl_fm utility. This app uses ncurses and was developed and projected to run on Raspberry PI with 3.5 LCD Small screens.

In fact, this app was developed using an Raspberry PI 3 B (2018's model), using vim and GCC.

IMPORTANT NOTICE: This app doesn't modifies the rtl_fm repository, this uses it as a child process in a "smart" way, killing and reestarting it after user request (link change frequency).

I love the linux console mode, so this project was not intented to be a graphical FFT or waterfall, it could be used, in future, by SSH access to tune a "remote" radio connected to external speakers.

The aestic of the app was inspired on my Kenwood TS-140s ham radio rig, the colors, digits, the way of turning on/off the display components, etc. :) 

Enjoy! Visit my QRZ.com

# Features

- Frequency tunner with arrow keys usage
- RX mode change with arrow keys usage (like a joystick)
- Frequency skip with direct keyboard input (ex: I'm on 98.1 Mhz FM and want to skip to the UVB76 radio (4.625). So I press F and type the freq)
- Volume level control (thanks alsamixer)
- Squelch level control (direct input)
- Auto freq step or by each scale (like GQRX)
- Low memory usage (May be run this on a RPi Zero? Why not?)


# Known issues
- Like I said, this frontend uses rtl_fm as a child process (System C command), so, sometimes after a frequency search, the rtl_fm app could stop running. It's normal and the only way to correct it is modifing the source code, that's not my target. 
  So. just press 'R' and evething works normal again. 
- Works only on stand gr-osmosdr default installation (TODO thing to correct later)
- Big screens was not fully supported (TODO later)


# Compiling and install
  
  - It uses ncurses, so make sure you have ncurses installed first
  - Make sure you have the gr-osmosdr installed and properly. It needs the rtl_fm utility.

  - make
  - sudo make install
 
  (If you have a small screen, you could use ./build.sh command to show the compiling result)

# Future plans

 - Make work on bigger screens (for ssh access)
 - Add ETM (Easy Tunnig Mode) using rtl_power
 - Add Frequency scan
 - Add VFO A and VFO B
 - Choice dongle? (If you have more than one dongle installed)

