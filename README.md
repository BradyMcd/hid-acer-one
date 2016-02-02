# Overview of the error
Some Acer keyboards report excessively large max usage values. This causes an error at boot which leaves the keyboard inoperable; since the max use values in the report descriptor exceed HID_MAX_USAGES the descriptor isn't parsed.

This module simply changes the report descriptor to have more sensible max usage values during boot before passing the edited version up to the generic hid module for parsing.

##### Known keyboards with this issue
* `06CB:73F4` (This is the only one targeted by this fix at present)
* `06CB:2968`
* `06CB:2991`
    
This module specifically targets keyboards identifying themsleves as ` 06CB:73F4 `. If your device identifies itself as `06CB:2968` or `06CB:2991` they should work OOTB on Linux Kernels version 4.3 and above, for older kernels the fix (which this module was adapted from) can be found [here](https://github.com/SWW13/hid-acer).

#### Symptoms of the error

Symptoms that this might be a problem you are experiencing is a message along the lines of `hid (null): usage index exceeded` followed by hid-generic complaining that `parsing failed` in your dmesg logs.

## Build / Install
First install linux-headers for your version of Linux, some details can be found [here](http://www.cyberciti.biz/tips/build-linux-kernel-module-against-installed-kernel-source-tree.html).

```
git clone https://github.com/BradyMcD/hid-acer-one.git
cd hid-acer-one
make
sudo make install
```

#### Uninstall
```
sudo make uninstall
```
