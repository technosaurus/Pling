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
