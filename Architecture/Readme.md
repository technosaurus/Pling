## Layout ##

Linux kernel
  - builtin options
     * dax for mapping erofs to pages
     * mount devtmpfs to speedup device init
     * overlayfs and erofs for layered fs
  - initramfs
     * static Xorg, jwm, busy/toybox, ashbox
     * fonts, config files, etc...
  - overlayfs
     * dax mounted erofs lower layers
     * compressed ramdisk upper/working directory
     * mounted filesystems as persistent alternative

## Boot Sequence ##

  1. decompress kernel asynchronously (patches)
  2. use Kernel's devtmpfs auto mount
  3. init begins
     - mount virtual filesystems (sysy, proc, dev/*)
     - draw desktop.png to fb (looks like desk with msg)
     - fork execve static Xorg
     - mount erofs layers in /mnt
     - mount overlayfs
     - wait for :0 and fork execve jwm
       * jwm's startup commands will finish bootup
     - reap zombies and respawn until shutdown

  ## Desktop System ##

jwm will provide
  
   - full access to the entire package manager via menu
  - basic filesystem navigation via generated menus
  - standard wm menus for installed programs
  - standard MIME types get menu entry to select handler
  - interactions using top layer buttons and refresh (jwm tools)
  - pretty much all necessities via ashbox + hacks
