# Idle Inhibition Service Draft

  * API Documentation
    - Introduction
    - Consequences of idleness
    - API overview
    - Design notes
  * D-Bus API Reference
    - Interfaces
    - org.freedesktop.ScreenSaver — The Idle Inhibition Service manages the inhibition requests.
  * References

## API Documentation

### Introduction

The Idle Inhibition Service API allows client applications to inhibit a user's
login session from becoming idle.

The service is usually implemented by the session manager, or another daemon
running inside the user's session, and communicating with the platform software
to avoid becoming idle, and the consequences of becoming idle.

This API was designed by GNOME and KDE developers with the goal of having a
common way to inhibit idleness.

### Consequences of idleness

Depending on the user session software running, idleness can have multiple
consequences, including, but not limited to:

  * Screen backlight dimming

  * Screensaver animation replacing the desktop content

  * Instant messenging client going to "auto-away"

  * Computer suspending or hibernating

Inhibiting idleness is supposed to stop all those actions from taking place,
specifically in response to idleness.
A user action asking for any of those states would obviously be acted upon
without delay.

### API overview

Idle inhibition is achieved by the application calling an 
[Inhibit](screensaver.md#inhibit) method on a well-known D-Bus name.

Inhibition will stop when the [UnInhibit](screensaver.md#uninhibit) method is
called, or the application disconnects from the D-Bus session bus
(which usually happens upon exit).

Implementations of this well-known bus name must have an object
`/org/freedesktop/ScreenSaver` which implements the 
[org.freedesktop.ScreenSaver](screensaver.md) interface.

### Design notes

The D-Bus service name, org.freedesktop.ScreenSaver, intentionally contains the
word screensaver, as KDE implements the idle inhibition API in a screensaver
sub-module.
It was necessary for the specificed Idle Inhibition Service API to be
compatible with this existing software.

KDE's implementation until kde-workspace 4.11.8 lived on the /ScreenSaver
object, not on /org/freedesktop/ScreenSaver as described in this specification.
This is fixed in later releases.

For the same reasons, inhibition of suspend, hibernation, or user-switching
(amongst others) is not supported in the API.

## D-Bus API Reference

### Interfaces

org.freedesktop.ScreenSaver — The Idle Inhibition Service manages the
inhibition requests.

### Synopsis

## Methods

```
Inhibit (
	IN String application_name,
 	IN String reason_for_inhibit,
 	OUT UInt32 cookie
);
 
UnInhibit (IN UInt32 cookie);
```
 
## Methods

`org.freedesktop.ScreenSaver.Inhibit`

```
Inhibit (
	IN String application_name,
 	IN String reason_for_inhibit,
 	OUT UInt32 cookie
);
```
 
Inhibit idleness for the caller application.

`application_name`
	A unique identifier for the application, usually a reverse domain
	(such as 'org.freedesktop.example').

`reason_for_inhibit`
	A human-readable and possibly translated string explaining the reason why
	idleness is inhibited (such as 'Playing a movie').

`cookie`
	A cookie uniquely representing the inhibition request, to be passed to
	`UnInhibit` when done.

`org.freedesktop.ScreenSaver.UnInhibit`

```
UnInhibit (	IN UInt32 cookie);
```
 
Disable inhibit idleness for the caller application.

`cookie`
	A cookie representing the inhibition request, as returned by the
	`Inhibit` function.

## References

*kde-api* [KDE Screenlocker D-Bus API](https://projects.kde.org/projects/kde/kde-workspace/repository/revisions/master/entry/ksmserver/screenlocker/dbus/org.freedesktop.ScreenSaver.xml)

*gnome-impl* [GNOME org.freedesktop.ScreenSaver proxy implementation](https://web.archive.org/web/20180212195104/http://git.gnome.org/browse/gnome-settings-daemon/tree/plugins/screensaver-proxy)
