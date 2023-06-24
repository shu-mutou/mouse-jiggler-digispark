# mouse-jiggler-digispark

Create mouse jiggler with Digispark ATTINY85 

## Develop environment

* Ubuntu Desktop 22.04 (not VM)
* Install library for usb
  `sudo apt install libusb-0.1-4 libserialport0 patchelf`
* Add user to `dialout` group
  `sudo adduser $USER dialout`
* Create file `/etc/udev/rules.d/49-micronucleus.rules` with following:
  ```
  # UDEV Rules for Micronucleus boards including the Digispark.
  # This file must be placed at:
  #
  # /etc/udev/rules.d/49-micronucleus.rules    (preferred location)
  #   or
  # /lib/udev/rules.d/49-micronucleus.rules    (req'd on some broken systems)
  #
  # After this file is copied, physically unplug and reconnect the board.
  #
  SUBSYSTEMS=="usb", ATTRS{idVendor}=="16d0", ATTRS{idProduct}=="0753", MODE:="0666"
  KERNEL=="ttyACM*", ATTRS{idVendor}=="16d0", ATTRS{idProduct}=="0753", MODE:="0666", ENV{ID_MM_DEVICE_IGNORE}="1"
  #
  # If you share your linux system with other users, or just don't like the
  # idea of write permission for everybody, you can replace MODE:="0666" with
  # OWNER:="yourusername" to create the device owned by you, or with
  # GROUP:="somegroupname" and mange access using standard unix groups.
  ```
  + Then, reload it `sudo udevadm control --reload-rules`
* Restart machine

## Arduino IDE

* Visit [Arduino IDE site](https://www.arduino.cc/en/software)
* Download `Linux ZIP file 64 bits (X86-64)` and extract it
* Add settings for digispark board
  + Open `File` -> `Preferences` menu on Arduino IDE (sketch)
  + Input `http://digistump.com/package_digistump_index.json` into `Additional Boards Manager URLs`
  + Open `Tools` -> `Board` -> `Boards Manager` menu
  + Filter with `Digistump AVR Boards` and `INSTALL` it
  + Select `Tools` -> `Board` -> `Digistump AV Boards` -> `Digispark (Default - 16.5mhz)`
  + Reference: [digistump wiki](https://digistump.com/wiki/digispark/tutorials/connecting)

## Download Source Code

* See download [source code](./mouse-jiggler-digispark.ino)
* Open the source code with Arduino IDE
* Unplug Digispark device
* Click `→` (Upload) button on tool bar
* Then plugin Digispark device
