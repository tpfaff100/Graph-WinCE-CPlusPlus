# Graph
This application was a rewrite of a custom-GUI DOS application I wrote years ago.
It uses the internal libraries of the PocketPC to generate trig functions using many oscillators.
The output is mixed together on the x-y axis as the user wishes and then displayed on the screen.

This technique is also used in my analog synthesizer system, which uses six oscillators, a plethora of knobs and switches to assign waveforms into a mixing circuit, then outputted (projected) by an argon laser onto the wall using General Scanning GS125D galvonometer scanners.

Not quite as much fun as the laser imaging system but
(a) Uses about 1300 watts less power!  (120v AC)
(b) Won't accidentally scan a 100mw beam across my eyeball (painful)
(c) Much less noisy (noisy fan is always running to cool the argon laser)
(d) Can explore waveforms the galvonemeter scanners can't cope with
(e) Can produce laser imaging plot data using a Compaq IPAQ (2001) well that is a lot of fun!
(f) The analog system works by feel, like playing a synthesizer in real time whereas
    a digital system allows recreation of amazing 2D waveform plots

For better or worse, (f) is pretty important though 'sympathetico' modulcation on the analog systems in real time is way easier!

This thing is worth checking out- one can play with it all day!


I also added some modules for doing unit conversion and loan amortization for no real good reason.
What was I thinking?  I don't know.  I guess I wanted everything I'd done on the iPaq in one application.

The generated artwork is for use on an iPAQ 3650 or so sorry about that.  If you use a horizontal layout WinCE device I'm sure you'll need to modify the images.


TODO:  I wrote this 'quick and dirty' and used the math library for generating sin/cos wave for the 2D plots on screen.  
       This could be a lot faster with lookup tables.
