# jack-midi-velocity-scale
Basic utility for scaling or clamping velocity of midi input events.

I wrote this because my midi keyboard is too quiet and I just needed a simple way to raise the floor of velocity values.

The way I use the program is.
Start a2jmidi and this program. Use qjackctl to connect the midi device made available from a2jmidi to this program's input, connect output to midi through.
Then use this midi-through port in a DAW or other program.
