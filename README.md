LeapFrog
========

An open-source Linux driver for the Leap Motion device.

##TODO
 * Implement basic I/O ( **started** )
 * Implement basic tracking and input data ( **todo** )
 * Implement login and session management interface ( **todo** )
 
##Build Instructions
There should be a precompiled kernel module in `/bin`, but if not, then simply running:
>make && sudo insmod leapmotion.ko
Should be enough to build the driver and then dynamically load it into the kernel. Once loaded, it may be found with the command `lsmod | grep leapmotion`.

###ChangeLog 3.23.13
 * Moved from software prototype to initial hardware USB interface.

###ChangeLog 3.20.13
 * Started work on driver prototype - a null character software driver.
