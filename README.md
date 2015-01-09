# Dephase

Dephase is a VST2 audio effect working at least on windows. To describe it, one could say that it makes some kind of "watery" or "blubby" sound. It modifies the sample heavily, especially on the harmony, so I mostly used it to get more harmony out of a sample.

The core idea of this effect is to first make a (very inaccurate) approximation of the frequency of the sound at each sample using the discrete first and second derivatives, then make some transformations on the pseudo-frequency and then turn it back to the wave using an oscillator (a mix of sine and saw) with the modified frequency.

One has to take care that this effect will create some loud infrabass that have to be removed afterwards (the return parameter will be helpful for that or any equalizer).
