# Patches

I plan on upstreaming some of these patches, but for now you'll have to start a git repo from the OldUnreal UTSDK 469d zip, and apply these patches manually. Build them in release mode, and you should be good! I generated these on a fresh SDK install, so they should work.

## Fix OpenGL

This is used to avoid a crash on creating multiple viewports (iirc). I'm really unsure if this is a bug or just something I'm doing wrong with my bootstrap code.

## Allow per viewport input for SDLDrv

This patch checks for inputs on each client tick, and buffers them per viewport it was done in. It also adds some special functionality like clicking on things for MultiEd. This code should only run if we're in editor mode.