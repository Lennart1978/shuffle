## shuffle
Shuffle command prints out an ASCII art picture with a cool shuffle effect.

Usage:

shuffle ascii_picture speed "color"

speed: 1 is fast and >500 very slow, -1 = immediately.

color: Must be in quotation marks of format: "r;g;b"
or one of the standard colors "red, green, yellow, blue, magenta, cyan, orange, white, grey" or just "random".

Example: shuffle file.ascii 10 "50;255;50" - prints "file.ascii" in bright green.

Build:
```bash
mkdir builddir && meson setup builddir --buildtype=release --prefix=/usr/bin && meson compile -C builddir
```

Run:
```bash
cd builddir && ./shuffle ../test.ascii 10 "50;255;50"
```

Install (when in builddir):
```bash
sudo meson install
```
For Archlinux users: It is in the AUR now 'shuffle-ascii'.

<img src="shuffle.gif" alt="shuffle.gif"></img>
<img src="bytheway.gif" alt="bytheway.gif"></img>
