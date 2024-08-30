# shuffle
Shuffle command prints out an ASCII art picture with a cool shuffle effect. You can also shuffle every kind of text file. (>=v1.1.0)

Usage:

shuffle SOURCE -s speed -c color

-s  speed: 1 is fast and >500 very slow, -1 = immediately.

-s  color: Must be in format: "r;g;b" or one of the standard colors:
           red, green, yellow, blue, magenta, cyan, orange, white, black, grey or just random.

-h  show this help.

-v  show version

Example: shuffle file.ascii -s 10 -c "50;255;50"  prints file.ascii in bright green.
         shuffle file.ascii -s 50 -c orange  prints it with standard color (orange).


## Build with meson or make:

meson:

```bash
mkdir builddir && meson setup builddir --buildtype=release --prefix=/usr/bin && meson compile -C builddir
```

make:

```bash
make
```

## Install with meson or make:

meson:

```bash
sudo meson install -C builddir
```

make:

```bash
sudo make install
```

## For Archlinux users: It is in the AUR now 'shuffle-ascii'.

<img src="shuffle.gif" alt="shuffle.gif"></img>
<img src="bytheway.gif" alt="bytheway.gif"></img>
<img src="FischerYatesShuffle.png" alt="shuffle algorith" width="1184" height="708"></img>
<img src="shuffle-main-algo.png" alt="flowchart" width="1618" height="3840"></img>
