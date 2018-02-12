
# Desktop Application Autostart Specification

## Table of Contents

  * [Introduction](#introduction)
  * [Autostart Of Applications During Startup](#autostart-of-applications-during-startup)
    - [Autostart Directories](#autostart-directories)
    - [Application .desktop Files](#application-desktop-files)
    - [Implementation Notes](#implementation-notes)
  * [Autostart Of Applications After Mount](#autostart-of-applications-after-mount)
    - [Autostart Files](#autostart-files)
    - [Autoopen Files](#autoopen-files)

## Introduction

This DRAFT document defines a method for automatically starting applications
during the startup of a desktop environment and after mounting a removable medium.

Some of the file locations in this specification are specified based on the
"desktop base directory specification".

The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD",
"SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in this document are to be
interpreted as described in RFC 2119.

## Autostart Of Applications During Startup

By placing an application's .desktop file in one of the Autostart directories
the application will be automatically launched during startup of the user's
desktop environment after the user has logged in.

### Autostart Directories

The Autostart Directories are $XDG_CONFIG_DIRS/autostart as defined in accordance
with the "Referencing this specification" section in the
"desktop base directory specification".

If the same filename is located under multiple Autostart Directories only
the file under the most important directory should be used.

Example: If $XDG_CONFIG_HOME is not set the Autostart Directory
in the user's home directory is ~/.config/autostart/

Example: If $XDG_CONFIG_DIRS is not set the system wide Autostart Directory
is /etc/xdg/autostart/

Example: If $XDG_CONFIG_HOME and $XDG_CONFIG_DIRS are not set and the two files
/etc/xdg/autostart/foo.desktop and ~/.config/autostart/foo.desktop exist then
only the file ~/.config/autostart/foo.desktop will be used because
~/.config/autostart/ is more important than /etc/xdg/autostart/

### Application .desktop Files
An application .desktop file must have the format as defined in the
"Desktop Entry Specification".
All keys should be interpreted as defined with the following exceptions
in order to take into account that the .desktop files in an
autostart directory are not shown in a menu.

**Hidden Key**

When the .desktop file has the Hidden key set to true,
the .desktop file MUST be ignored.
When multiple .desktop files with the same name exists in multiple directories
then only the Hidden key in the most important .desktop file must be considered:
If it is set to true all .desktop files with the same name in the other
directories MUST be ignored as well.

**OnlyShowIn and NotShowIn Keys**

The OnlyShownIn entry may contain a list of strings identifying the desktop
environments that MUST autostart this application, all other desktop
environments MUST NOT autostart this application.

The NotShownIn entry may contain a list of strings identifying the desktop
environments that MUST NOT autostart this application, all other desktop
environments MUST autostart this application.

Only one of these keys, either OnlyShowIn or NotShowIn,
may appear in a single .desktop file.

**TryExec Key**

A .desktop file with a non-empty TryExec field MUST NOT be autostarted
if the value of the TryExec key does NOT match with an installed
executable program.
The value of the TryExec field may either be an absolute path or
the name of an executable without any path components.
If the name of an executable is specified without any path components
then the $PATH environment is searched to find a matching executable program.

### Implementation Notes
If an application autostarts by having a .desktop file
installed in the system wide autostart directory,
an individual user can disable the autotomatic start of this application
by placing a .desktop file of the same name in its personal
autostart directory which contains the key Hidden=true.

## Autostart Of Applications After Mount

When a desktop environment mounts a new medium, the medium may contain
an Autostart file that can suggest to start an application or an Autoopen
file that can suggest to open a specific file located on the medium.

### Autostart Files

When a new medium is mounted the root directory of the medium should
be checked for the following Autostart files in order of precendence:
.autorun, autorun, autorun.sh Only the first file that is present should be considered.

The desktop environment MAY ignore Autostart files altogether based on
policy set by the user, system administrator or vendor.

The desktop environment MUST prompt the user for confirmation before
automatically starting an application.

When an Autostart file has been detected and the user has confirmed
its execution the autostart file MUST be executed with the current
working directory (CWD) set to the root directory of the medium.

###Autoopen Files

When a new medium is mounted and 
a) the medium does not contain an Autostart file or 
b) a policy to ignore Autostart files is in effect
then the root directory of the medium should be checked for
the following Autoopen files in order of precedence:
.autoopen, autoopen .
Only the first file that is present should be considered.

The desktop environment MAY ignore Autoopen files altogether based on
policy set by the user, system administrator or vendor.

An Autoopen file MUST contain a single relative path that points to
a non-executable file contained on the medium.
If the file contains a newline or carriage return character
then the newline or carriage return character itself and all
characters that follow MUST be ignored.

The relative path MUST NOT contain path components that
refer to a parent directory (../)

The relative path MUST NOT point to an executable file.

The desktop environment MUST verify that the relative path points to
a file that is actually located on the medium, taking into account
any symbolic or other links and MUST ignore any relative path that
points to a file location outside the medium itself.

If the relative path points to an executable file then
the desktop environment MUST NOT execute the file.

The desktop environment MUST prompt the user
for confirmation before opening the file.

When an Autoopen file has been detected and the user has confirmed
that the file indicated in the Autoopen file should be opened
then the file indicated in the Autoopen file MUST be opened
in the application normally preferred by the user for files
of its kind UNLESS the user instructed otherwise.
