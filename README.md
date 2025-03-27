# shuffle

The shuffle command prints an ASCII art picture with a cool shuffle effect.

You can also shuffle any text file, even if it contains Unicode characters.

Now you can also use it with a pipe:  

cat file.ascii | shuffle -s 100 -c random

## Usage

shuffle SOURCE -s speed -c color -e effect

-s speed: 1 fast to 500 very slow.

-c color: Must be in the format "r;g;b" or one of the standard colors: red, green, yellow, blue, magenta, cyan, orange, white, black, grey, or just "random".

-e effect: Random(standard), spiral, wipe, snake, blocks

-h show this help.

-v show version.

## Example

shuffle file.ascii -s 10 -c "50;255;50"  

prints file.ascii in bright green with random shuffle effect.

shuffle file.ascii -s 50 -c orange -e wipe

prints it with the standard color (orange) with "wipe" effect.

## Build with Meson or Make

Meson:

```bash
mkdir builddir && meson setup builddir --buildtype=release --prefix=/usr/bin && meson compile -C builddir
```

Make:

```bash
make  
```

Install with Meson or Make:

Meson:

```bash
sudo meson install -C builddir  
```

Make:

```bash
sudo make install
```

For Arch Linux users: It is now available in the AUR as 'shuffle-ascii'.

![Shuffle animation](shuffle.gif)
![By the way animation](bytheway.gif)
