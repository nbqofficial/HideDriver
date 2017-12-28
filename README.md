# HideDriver ( Direct kernel object manipulation )
Using DKOM to hide kernel mode drivers.

HideDriver() function changes flink and blink pointers of previous
and next process in the process list to point around our Driver process.
This makes our Driver vanish in programs like DriverView because those
programs enumerate process list in which our Driver process has been snipped out.

This could be incredibly  useful for someone trying to bypass certain anticheat software
to circumvent security when programming a rootkit-like software.

Everything you need is provided in the project. For driver development you need 
Visual Studio 2015 Professional with WDK and SDK installed.


