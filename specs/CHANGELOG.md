**TODO:** move Changes sections here use version control for changes thereafter.

## Table of Contents

[Desktop Entry Specification](#desktop-entry-specification)

## Desktop Entry Specification

### Changes from version 1.1 to 1.2

  * New section "File naming", to formalize file names, with relation to D-Bus naming.
  * Add Implements key.
  * Specify XDG_CURRENT_DESKTOP.

### Changes from version 1.0 to 1.1

  * Add DBusActivatable key.
  * Add support for Actions.
  * Add Keyword key.
  * Make Exec key mandatory.
  * Make Version key a string.
  * Make UnmountIcon key a localestring.

## Icon Naming Specification

### Version 0.8, 1 August 2006, Rodney Dawes. 

  * Add "view-restore" and "utilities-system-monitor"
  * Add "folder-new" and "mail-send"
  * Add "object-flip-horizontal" and "object-flip-vertical"
  * Add "object-rotate-left" and "object-rotate-right"
  * Add "applications-engineering" and "applications-science"
  * Remove duplicate "preferences-desktop-accessibility"
  * Rename "drive-cdrom" to "drive-optical"
  * Rename "media-cdrom" to "media-optical"
  * Add "media-flash", "media-tape", and "modem"
  * Add "security-high", "security-medium", and "security-low"

### Version 0.7, 15 February 2006, Rodney Dawes. 

  * Remove the wallpaper-* icon names
  * Align text blocks to the left
  * Add "weather-few-clouds-night" and "weather-clear-night" to the status context
  * Remove "network-online" from the status context
  * Add "network-idle", "network-receive", "network-transmit", and
    "network-transmit-receive" to the status context
  * Update the printer status icon definitions

### Version 0.6, 07 December 2005, Rodney Dawes. 

  * Add the "Animations" and "Places" contexts
  * Add "media-eject" to the actions context
  * Move "media-playlist-shuffle" to the status context
  * Add the "process-working" animation to the animations context
  * Add "battery", "camera-photo", "media-floppy", "multimedia-player",
    "network-wired", and "network-wireless" to the devices context
  * Remove the printer-remote and video-card icons from devices
  * Rename "emblem-favorites" to "emblem-favorite"
  * Add "application-x-executable", "package-x-generic", "text-x-generic-template",
    and "text-x-script" to the mimetypes context
  * Rename "x-directory-normal" to "folder" and move it to the places context
  * Rename "x-directory-remote" to "folder-remote" and move it to the places context
  * Rename "x-directory-normal-remote-server" to "network-server" and move it to
    the places context
  * Rename "x-directory-normal-remote-workgroup" to "network-workgroup" and move it
    to the places context
  * Rename "x-directory-trash" to "user-trash" and move it to the places context
  * Rename "x-directory-trash-full" to "user-trash-full" and move it to the status
    context
  * Add "start-here", "user-desktop", and "user-home" to the places context
  * Add "folder-drag-accept", "folder-open", "folder-visiting", and
    "media-playlist-repeat" to the status context

### Version 0.4, 10 October 2005, Rodney Dawes. 

  * Use "The icon" instead of "This icon" for "contact-new" and "battery-low"
  * Add icons for "preferences-desktop-accessibility" and "preferences-desktop-font"
    in the apps context
  * Fix typo in "drive-cdrom" name
  * Rename "x-font-generic" to "font-x-generic" to match other MIME type icons
    such as for audio, images, and video
  * Add icons for "image-loading" and "image-missing" to status

### Version 0.3, 28 June 2005, Rodney Dawes. 

  * Replace "printer-local" with "printer" to be more generic.
  * Import into freedesktop.org cvs alongside icon-theme-spec.xml
  * Add missing icon descriptions.

### Version 0.2, 01 April 2005, Jakub Steiner. 

  * Fixed two small typographical errors.

### Version 0.1, 29 March 2005, Rodney Dawes. 

  * Created initial draft.
