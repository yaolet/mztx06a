mztx06a
=======

Raspberry Pi mztx06a LCD library

```
When I wrote this monstrosity years ago and put it on github, I had no idea it would be picked up by other rpi hobbists.  It was purely personal play-around and I am sorry for any painstaking attempts to make it work on your pi and lacking of updates thereafter.

The program modified bcm2835 library a bit to gain some extra SPI performance. Apparently 3 generations of pi later, it will not work on recent models.

I will attempt to make an update soon possibly with some reorganisation of the code and performance improvement.
```

I know, most parts of the code are messy and un-organised.  Global variables are flying around; quick and dirty hacks everywhere.  But should you still have interest poking around, here is a brief step-by-step guide

First make sure your Pi is running in a low-resolution (currently support 640x480, 320x240).  If you are really new to this Raspberry pi thingy and don't know what to do, I'll give you a pointer, just type 

> sudo pico /boot/config.txt

try fix your eyeballs on something look like:

> \# uncomment to force a console size. By default it will be display's size minus
> \# overscan.
> \# framebuffer_width=xxxx
> \# framebuffer_height=xxx

now uncomment the last 2 lines and change it to either

> framebuffer_width=320
> framebuffer_height=240

If you prefer a blurry console nightmare, try 

> framebuffer_width=640
> framebuffer_height=480

Beware this won't make your LCD bigger, nor it has some retina-like hidden pixels like your iPhone.  The program will just squeeze 4 pixels into 1.

Now Ctrl-X and yes to save the changes.  Don't get panic if nothing shows up on the screen.  Try 

> sudo reboot

and start live your life in good-old 80s fashion screen (You might wonder what's the whole point of getting a HD screen at this point).

To compile, go to whatever the directory you store my precious git, and 'cd src'.  type 'make', no news is good news.

To run, root priviledge is required: 

> sudo ./mztx06a

then let there be light.

If the light is incorrect, check the main(), for 320x240 resolution,  ues loadFrameBuffer_diff_320(), drop _320 if the resolution is 640x480.

Well... this is it.  No kernel re-compilation is required.  No raspberry pi was harmed during the making of this program.

Feel free to spit, twit, modify or legendise.
