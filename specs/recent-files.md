# Recent File Storage Specification

## Table of Contents

[Overview](#overview)
[Storage](#storage)
[Notification](#notification)

### Overview

Many applications choose to have a menu containing a list of recently used
files, which can be used as an alternative to opening a file selector.
GNOME, and KDE applications have done this for quite some time, and probably
others have as well. This specification aims to do the following things:

  * Provide a standard mechanism for storing a list of recently used files (really, URIs)
  * Allow for notification of changes in the list

Accomplishing this will make using applications that are not native to the user's
desktop a bit nicer, and give a more unified feel to the X desktop in general.
It will be especially useful to applications that don't belong to a specific
desktop at all, such as Mozilla and Open Office, since those applications could
become compatible with all desktops in this area without special case
considerations for each one.

### Storage

For different desktops and applications to have access to the same information,
a protocol for storing the recent file list has to be determined.
An XML document will be used, of the form below:

```xml
<?xml version="1.0"?>
<RecentFiles>
  <RecentItem>
    <URI>file:///home/jwillcox/testfile.txt</URI>
    <Mime-Type>text/plain</Mime-Type>
    <Timestamp>1028181153</Timestamp>
    <Private/>
    <Groups>
      <Group>Recent File Test</Group>
    </Groups>
  </RecentItem>
  <RecentItem>
    <URI>file:///home/jwillcox/recent-file-spec.xml</URI>
    <Mime-Type>text/xml</Mime-Type>
    <Timestamp>1028181158</Timestamp>
    <Private/>
    <Groups>
      <Group>Recent File Test</Group>
    </Groups>
  </RecentItem>
</RecentFiles>
```
    
The URI, Mime-Type, Timestamp tags are required, but the Private, Groups, and
Group tags are not. The Timestamp tag should be the number of seconds sinced
the Epoch when the item was added to the list. The Group tags exist for the
purpose of making groups of items with an arbitrary set of MIME types. A single
item can belong to an unlimited number of groups. If the Private tag is
specified, it means that implementations should only include the item if
requestor has specifically asked for items in a group that the item belongs to.
For instance, maybe a file selector would like to hold a list of directories
that you have recently saved files in, and allow you to choose from one of these
next time you save a file. You wouldn't want those directories showing up in the
"Recent Documents" menu. If the items are put into the "File Selector Save
Directories" group and marked as private, this problem is solved.

All text in the file should be stored in the UTF-8 encoding.
No local paths are allowed in the URI tag.
They should be converted to a valid URI with a "file" scheme.
Items with the same URI should not be allowed.
If a duplicate is found, it should have its timestamp updated, and any new groups added to it.
No other information should be changed.
The document should be stored in "~/.recently-used", and it should contain no more than 500 items.

Before reading or writing the document, it should be locked using the POSIX
lockf() mechanism, and unlocked when finished.

## Notification

Notification should be accomplished by simply monitoring the document for changes.
This can be done by either polling the file every so often, or using a library like FAM.

