# OSESLayer
Layer for the Embedded Linux assignment of the Operating Systems for Embedded Systems course

To make it work properly, you need to:
- Clone the layer (the whole git project) inside /poky/layer_name (layer_name is the name you want to assign for this layer);
- Add the layer by using the following command: bitbake-layers add-layer path_of_the_layer
- Inside your build, go to conf/local.conf and add the following strings: <br/>
        IMAGE_INSTALL_append = " ppgsensor-driver" <br/>
  If there are no other kernel modules to load at boot time, write <br/>
        KERNEL_MODULE_AUTOLOAD += "ppgsensor-driver" <br/>
  Otherwise, if you have already defined a command like <br/>
        KERNEL_MODULE_AUTOLOAD += "kernelmodule1 kernelmodule2 ... kernelmoduleN" <br/>
  Then you need to append the kernel module name in this way <br/>
        KERNEL_MODULE_AUTOLOAD += " ... kernelmoduleN ppgsensor-driver" <br/>
- Finally, execute bitbake core-image-minimal for your qemuarm machine and run the machine to test the driver. <br/>
