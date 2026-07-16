## Nanosaurus ##

Back in the old Murga forum days of Puppy Linux, goingnuts and I rebooted the Xwoaf project. Even then we felt that software was getting bloated and overwhelming complex. Nanosaurus grew out of pure curiosity of what exactly was the bare minimum requirement for a standard Linux desktop environment. All unnecessary kernel configs were disabled and only the bare minimum tools were included, built-in to the kernel's embedded initramfs.

_This is what the underlying filesystem looked like_

```
/
├── bin/
│   ├── busybox           # Monolithic binary providing core CLI tools (sh, ls, cat, etc.)
│   └── fixmenu           # Minimal script to regenerate JWM configuration menus
├── etc/
│   ├── jwm/
│   │   └── jwmrc         # Main window manager configuration layout
│   └── profile           # Basic system environment variables
├── sbin/
│   └── (Minimal links)   # Radically trimmed down; standard network/hardware helpers removed
├── usr/
│   └── bin/
│       ├── jwm           # Custom patched window manager (stb_image/nanosvg backend)
│       └── st / rxvt     # Ultra-lightweight terminal emulator (Suckless ST or RXVT)
├── dev/                  # Static or devtmpfs populated core nodes (null, console, tty, fb0)
├── proc/                 # Virtual process filesystem mount point
├── sys/                  # Virtual system filesystem mount point
└── init                  # The compiled 10-line C binary (handles mounting proc/sys and launching X)
```

## pupngo ##

Several advancements
  * gtkdialog ported to gtk1
  * netsurf patches to run on xcb/linux framebuffer
  * various new tools made
  * various old tools restored

## Puppy, Quirky and EasyOS $$
