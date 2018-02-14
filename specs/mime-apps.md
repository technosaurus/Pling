# Association between MIME types and applications

## Table of Contents

  * [Introduction](#introduction)
  * [File name and location](#file-name-and-location)
  * [Adding/removing associations](#addingremoving-associations)
  * [Default Application](#default-application)

## Introduction

The freedesktop.org Shared MIME database provides a single way to store static
information about MIME types and rules for determining a type.

The freedesktop.org Desktop Entry specification allows applications to announce
which MIME types they support.

This specification solves the remaining issues: which application should open a
file by default, how to let the user change the default application, and how to
let the user add or remove associations between applications and mimetypes.

## File name and location

Users, system administrators, application vendors and distributions can change
associations between applications and mimetypes by writing into a file called
mimeapps.list.

The lookup order for this file is as follows:

|mimeapps.list path|purpose
|------------------|-------
|`$XDG_CONFIG_HOME/$desktop-mimeapps.list`|user overrides, desktop-specific (for advanced users)
|`$XDG_CONFIG_HOME/mimeapps.list`|user overrides (recommended location for user configuration GUIs)
|`$XDG_CONFIG_DIRS/$desktop-mimeapps.list`|sysadmin and ISV overrides, desktop-specific
|`$XDG_CONFIG_DIRS/mimeapps.list`|sysadmin and ISV overrides
|`$XDG_DATA_HOME/applications/$desktop-mimeapps.list`|for completeness, deprecated, desktop-specific
|`$XDG_DATA_HOME/applications/mimeapps.list`|for compatibility, deprecated
|`$XDG_DATA_DIRS/applications/$desktop-mimeapps.list`|distribution-provided defaults, desktop-specific
|`$XDG_DATA_DIRS/applications/mimeapps.list`|distribution-provided defaults

In this table, `$desktop` is one of the names of the current desktop, lowercase
(for instance, kde, gnome, xfce, etc.)

This is determined from taking the ascii-lowercase form of a component the
environment variable `$XDG_CURRENT_DESKTOP`, which is a colon-separated list of
names that the current desktop is known as.

The `$desktop` variable should be each of these values in turn.

All of the above files are referred to as "`mimeapps.list`" in the rest of this
specification, for simplicity.

Note that the desktop-specific files can only be used for specifying the default
application for a given type.
It is not possible to add or remove associations from these files.

The algorithms for determining the list of all applications associated with a
mime type and for determining the default application are (almost) completely
unrelated, and so they are presented separately.

## Adding/removing associations

Adding and removing associations between mimetypes and applications is done
with the following syntax in the mimeapps.list file:

```
[Added Associations]
mimetype1=foo1.desktop;foo2.desktop;foo3.desktop;
mimetype2=foo4.desktop;
[Removed Associations]
mimetype1=foo5.desktop;
```

[Added Associations] and [Removed Associations] groups may only appear in the
non-desktop-specific files (ie: in files that are actually named "mimeapps.list").

The [Added Associations] group defines additional associations of applications
with mimetypes, as if the .desktop file was listing this mimetype in the first
place.

The [Removed Associations] group removes associations of applications with
mimetypes, as if the .desktop file was NOT listing this mimetype in the first
place.

Listing the same application for the same type in both the Added and Removed
sections is invalid and may produce implementation-defined behaviour.

The order of the entries in the [Added Associations] entry for a given type
should be in the "most preferred order", according to the implementation.
As such, implementations should take care to preserve the order except in
situations where they are explicitly intending to change it.

The adding and removal of associations only applies to desktop files in the
current directory, or a later one (in precedence order).
This means that additions and removals applied from the mimeapps.list file in
`$XDG_CONFIG_HOME`, `$XDG_CONFIG_DIRS` and `$XDG_DATA_HOME` will override all
existing desktop files, but that an addition or removal of an association using
`/usr/share/applications/mimeapps.list`, for example, would be ignored if the
desktop file of the named application was present in 
`$XDG_DATA_HOME/applications`.
Put another way: added and removed associations can be thought of as equivalent
to copying the desktop file from the next-in-precedence directory with respect
to the directory containing the mimeapps.list (which may be the same directory)
and tweaking its MimeType= line.
If the same .desktop file appears again in a directory with higher precedence
order, then this modified .desktop file with lower precedence would be ignored.
If the addition or removal refers to a desktop file that doesn't exist at this
precedence level, or a lower one, then the addition or removal is ignored, even
if the desktop file exists in a high-precedence directory.

The suggested algorithm for listing (in preference order) the applications
associated to a given mimetype is:

  * create an empty list for the results, and a temporary empty "blacklist"
  * visit each "`mimeapps.list`" file, in turn; a missing file is equivalent
    to an empty file
  * add to the results list any "Added Associations" in the `mimeapps.list`,
    excluding items on the blacklist
  * add to the blacklist any "Removed Associations" in `mimeapps.list`
  * add to the results list any .desktop file found in the same directory as the
    `mimeapps.list` which lists the given type in its MimeType= line, excluding
    any desktop files already in the blacklist.
    For directories based on `XDG_CONFIG_HOME` and `XDG_CONFIG_DIRS`, there are
    (by definition) no desktop files in the same directory.
  * add to the blacklist the names of any desktop files found in the same
    directory as the mimeapps.list file (which for directories based on 
    `XDG_CONFIG_HOME` and `XDG_CONFIG_DIRS`, is none)
  * repeat the last four steps for each subsequent directory

## Default Application

Indicating the default application for a given mimetype is done by writing into
the group [Default Applications] in the `file mimeapps.list`.

The [Default Applications] group indicates the default application to be used
for a given mimetype.
This is for instance the one that will be started when double-clicking on a
file in a file manager.
If the application is no longer installed, the next application in the list is
attempted, and so on.

This example ensures that the application default1.desktop will be used for
mimetype1, if it's installed, and otherwise default2.desktop if it's installed:

```
[Default Applications]
mimetype1=default1.desktop;default2.desktop;
```

The value is a semicolon-separated list of desktop file IDs
(as defined in the desktop entry spec).

In the absence of such an entry, the next mimeapps.list is checked.
Once all levels have been checked, if no entry could be found, the
implementations should pick the most-preferred .desktop files associated with
the mimetype, taking into account added and removed associations as per the
previous section.

The suggested algorithm for determining the default application for a given
mimetype is:

  * get the list of desktop ids for the given mimetype under the
    "Default Applications" group in the first mimeapps.list
  * for each desktop ID in the list, attempt to load the named desktop file,
    using the normal rules
  * if a valid desktop file is found, we have found the default application
  * if after all list items are handled, we have not yet found a default
    application, proceed to the next mimeapps.list file in the search order
    and repeat
  * if after all files are handled, we have not yet found a default application,
    select the most-preferred application (according to associations) that
    supports the type

Note that, unlike adding and removing associations, a desktop ID set as the
default for an application can refer to a desktop file of the same name found
in a directory of higher precedence.

Note as well that the default application for a given type can be an application
that is not associated with this type (ie: neither by MimeType= or an added
association, or even in the case that the association was specifically removed).
Such configurations should be regarded as unusual, however, and implementations
should not write mimeapps.list files that create this circumstance.
