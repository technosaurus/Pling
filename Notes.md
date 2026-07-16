# Ideal OS #

## Memory Subsystem ##

All compression is the same as the underlying filesystem such that decompression is only necessary into RAM and compression is only needed from RAM.
This also allows for the underlying filesystem utilities to recompress data at a higher level as needed or as resources permit.

  * RAM
  * Compressed RAM
  * Compressed SWAP
  * Compressed Filesystem with automatic SWAP facilities including "Lazy" mmap

Currently, Linux supports so many file and memory systems that it is difficult to integrate them vertically.
If something is supposed to be moved from compressed RAM into compressed SWAP, the data is de/re-compressed during the copy.
Ideally this would just be a copy since the compression method is controlled.
As part of this, any file that is mmap'ed, could be forcibly switched to a "Lazy" mmap to reduce RAM pressure in an OOM event.

  * If this happens with Linux, it will probably be a fork - It would take me 100 times the effort to deal with the kernel devs as it would to code it.

## Filesystem ##

The visible filesystem must be capable of:
  * layering multiple underlying compressed local filesystems
  * layering multiple underlying compressed remote filesystems
  * optionally caching slower (remote/hdd) filesytems to faster (local/SSD) ones
  * using remote filesystems without a local cache
  * ability to turn off caching on a per process basis (using env var?)
  * unioning multiple writeable mounts into one (unlike overlayfs)
  * allow key based remote filesystems for "payed" apps, private resources, etc...
Possible usage would be to handle the layered file systems in a similar way to the Memory Subsystem by caching remote filesystems locally, then purging the cache as it gets full.
Purging a cached file would require that the file is unmodified since caching and still exist in a lower layer.
If caching is enabled but full, the least recently used cached files get purged to make room.
This requires a capable server for the remote filesystems, fortunately http-1.1 has 99% of the requirements (see httpfs)

  * Again this will probably be outside the kernel tree, so might as well use FUSE.

## Package Manager? We don't need no stinkin' package manager! ##

Rather than have a package manager, all packages will be installed on a remote server which can be invisibly mounted into the filesytem.
Because this is an immense number of files, the remote filesystem is initially "invisible" or in typically unioning file system terminology "whited out".
This means that a directory listing will only list files on the local filesystem and files that have already been cached;
however, for file operations like open*, access* and stat* the remote filesytem will be probed as well.
This allows for "installation" by simply un-whiting out a corresponding desktop file and running a script that gives options for shortcuts, handling Mime-Types, etc...
Note that this in an of itself doesn't "install" (cache) any other files.
This should happen when the filesystem first tries to exec the executable, but this could be handled by the server if necessary.
As the executable tries to access/open/stat files, they are automatically cached or loaded from the remote filesystem.

  * The server can run a modified fileserver with modifications to handle FUSE operations

## Mime-Type handling ##
If the user tries to xdg-open an unsupported file (no default handler) it can be difficult for them to figure out what to install.
Since we don't have to bother with a traditional package manager, we can simply grok the *mimeapps.list or .desktop files and offer the user multiple options.
As a big bonus, most .desktop files have localized descriptions and user recognizable icons, so the user will barely notice the process.
It will just look similar to any first run dialog.
For example, The first time a user tries to open a pdf, they will get a dialog that shows details for mupdf, evince, xpdf, epdfview,...
This could set the default handler and open the pdf or even let them try and compare each one.
Note: XDG specs don't specify the difference between opening and editing - just handling (Needs correction)

## System Configuration ##
All basic configuration will have a command line utility or wrapper in the form of a multicall shell script (similar to busybox)
These will be designed to interface with multiple frontends including:
  * voice controlled, spoken interface for vision impaired
  * web interface that is useable without javascript (may be enhanced via javascript to sort tables, verify options etc...)
    - many web browsers can be configured for accessibility including using larger fonts or a specific browser like edbrowse
  * dialog/Xdialog version that selects the appropriate version based on $DISPLAY
  * Desktop environment specific versions like gtkdialog for gtk

## Fault tolerant base system ## 

All essential programs/scripts will be included in the local filesytem and statically compiled.
Initially, this will be xfbdev, jwm, st, netsurf-framebuffer and busybox compiled against musl-libc.
Additional command line binaries will be patched into busybox as needed and the X apps will be in a single separate multicall binary.


## Roadblocks ##

# Historical Baggage

The Linux kernel has baggage deriving from its desire for backwards compatibility and originating on a register starved 386 that used a stack based calling convention, historical bolted on struct definitions and the basic structure of Unix. Each newly supportef architecture after 386 has been a bolt on with recognized problems of prior architectures omitted, which is great for that new architecture but doesn't help the old one bound by backwards compatibility requirements. This has led to every C library to hack in some inline assembly for every supported architecture just to _start a program and to bloat the source with a spaghetti maze if ifdefs just to handle struct definitions and inexplicably varying basic syscall numbers

## Reality - Everything sucks, but it's fine. ##

The Linux kernel has too much inertia to change. No size ordered structs, no shared syscall numbers, no using the same registers for syscalls as function calls. This means we can't use tools like llvm intermediate representation or its corresponding bitcode to compile a single mybinary.bc that can be 99% optimized inside the package using `clang -march=generic64 ... ` and then seamlessly converted to the exact architecture to take advantage of the full machine capacity using `llc -march=native` to reduce server space by as much as 10 fold while providing the user a better product. That's fine, we can get around those problems on an architecture by architecture basis. Sure it leads to some amount of tribalism, but in some cases that's beneficial - the X86 distro manager doesn't need to know or care about the latest arm or ppc vulnerability. That's all fine, we can have multiple build systems for each architecture.

Then you get to the underlying systems where the defacto libc is practically useless for static builds (yes we have musl for that), large and built on code that even to an intermediate C programmer looks like PFM. Fine, we can have 2 separate build systems for static vs. shared.

Since we are no longer in the command line era, we need to look at graphical environments, which has devolved to X11 vs Wayland. For X11 you get a code legacy built on the legacy of stack based calling convention (no xcb didn't learn and do better, they just made it asynchronously bad) where functions take argument in struct order so that old cdecl calling conventions would have the X request data already lined up, but when the first n parameters are passed on the stack, the compiler gets to play shuffle the musical chairs for several cycles just to format the request. And then you have Wayland which got some of the failures of X right but decided to find new ways to suck to the point where after longer than the time between X11 and the start of Wayland we barely see anything developed exclusively for it or even as an option except for a couple GUI toolkits that bring apps along for free. That's fine, we can use Xwayland (or realistically just keep Xorg until something better replaces Wayland)

Realistically the 1 area where we have bike sheds in every shape, size and color is desktop environment and window managers to the point where one could get analysis paralysis. That's fine, I still like jwm because it reminds me of my first Packard Bell and its easy to morph into just about anything... good enough is good enough.

One area of optimism is the browser. While actively maintained browser engines have decreased, the quality and speed has drastically improved for the remaining competition, but everyone has their preferences. That's fine, just let the user choose their poison based on their own tastes.

I could get into all the other application categories and suites, but the reality is that most people use online apps for their day to day tasks. That's fine, we can just let the user choose their tools when they start the task, that's why we have MIME types and package managers.

## Executive Summary ##

People just want a system that starts fast and does what they want but nothing more. If an included application is useful or may be, they let it slide or ignore it until it gets in the way. We have managed to cobble together 100s of distros that cater to every kind of computer nerd but only a few that reach beyond that.

## Conclusion ##

I'm not building a distro. I'm assembling a set of tools that can make it simple for anyone to intuitively allow their own perfect distro to magically grow inside their computers with minimal thought or interaction.
