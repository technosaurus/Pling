# MPRIS D-Bus Interface Specification

## Table of Contents

  * About 


## About

The Media Player Remote Interfacing Specification is a standard D-Bus interface
which aims to provide a common programmatic API for controlling media players.

It provides a mechanism for discovery, querying and basic playback control of
compliant media players, as well as a tracklist interface which is used to add
context to the active media item.

## Bus Name Policy

Each media player must request a unique bus name which begins with
org.mpris.MediaPlayer2. For example:

org.mpris.MediaPlayer2.audacious
org.mpris.MediaPlayer2.vlc
org.mpris.MediaPlayer2.bmp
org.mpris.MediaPlayer2.xmms2

This allows clients to list available media players
(either already running or which can be started via D-Bus activation)

In the case where the media player allows multiple instances running
simultaneously, each additional instance should request a unique bus name,
adding a dot and a unique identifier to its usual bus name, such as one based
on a UNIX process id.
For example, this could be:

org.mpris.MediaPlayer2.vlc.instance7389

Note: According to the D-Bus specification, the unique identifier
"must only contain the ASCII characters '[A-Z][a-z][0-9]_-'"
and "must not begin with a digit".

## Entry point

The media player must expose the `/org/mpris/MediaPlayer2` object path,
which must implement the following interfaces:

org.mpris.MediaPlayer2
org.mpris.MediaPlayer2.Player

The /org/mpris/MediaPlayer2 object may implement the
org.mpris.MediaPlayer2.TrackList interface.

The /org/mpris/MediaPlayer2 object may implement the
org.mpris.MediaPlayer2.Playlists interface.

## The PropertiesChanged signal

The MPRIS uses the org.freedesktop.DBus.Properties.PropertiesChanged signal
to notify clients of changes in the media player state.
If a client implementation uses D-Bus bindings which do not support this signal,
then it should connect to it manually.
If a media player implementation uses D-Bus bindings which do not support this
signal, then it should send it manually.

## Interfaces

### org.mpris.MediaPlayer2

#### Methods

  * Raise()
    - Brings the media player's user interface to the front using any
      appropriate mechanism available.
    - The media player may be unable to control how its user interface is
      displayed, or it may not have a graphical user interface at all.
      In this case, the CanRaise property is false and this method does nothing.
  * Quit()
    - Causes the media player to stop running.
    - The media player may refuse to allow clients to shut it down.
      In this case, the CanQuit property is false and this method does nothing.

Note: Media players which can be D-Bus activated, or for which there is no
sensible easy way to terminate a running instance (via the main interface or a
notification area icon for example) should allow clients to use this method.
Otherwise, it should not be needed.

If the media player does not have a UI, this should be implemented.

#### Properties

Accessed using the `org.freedesktop.DBus.Properties` interface.

  * CanQuit
    - Read only (boolean)
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - If false, calling Quit will have no effect, and may raise a NotSupported
      error.
      If true, calling Quit will cause the media application to attempt to quit
      (although it may still be prevented from quitting by the user, for example).
  * Fullscreen
    - Read/Write (boolean)
    - Added in 2.2.
    - This property is optional. Clients should handle its absence gracefully.
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - Whether the media player is occupying the fullscreen.
    - This is typically used for videos. A value of true indicates that the
      media player is taking up the full screen.
    - Media centre software may well have this value fixed to true
    - If [CanSetFullscreen](#cansetfullscreen) is true, clients may set this
      property to true to tell the media player to enter fullscreen mode,
      or to false to return to windowed mode.
    - If [CanSetFullscreen](#cansetfullscreen) is false, then attempting to set
      this property should have no effect, and may raise an error.
      However, even if it is true, the media player may still be unable to
      fulfil the request, in which case attempting to set this property will
      have no effect (but should not raise an error).
  * CanSetFullscreen
    - Read only (boolean)
    - Added in 2.2.
    - This property is optional.
      Clients should handle its absence gracefully.
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - If false, attempting to set Fullscreen will have no effect,
      and may raise an error.
      If true, attempting to set Fullscreen will not raise an error, and
      (if it is different from the current value) will cause the media player
      to attempt to enter or exit fullscreen mode.
    - Note that the media player may be unable to fulfil the request.
      In this case, the value will not change.
      If the media player knows in advance that it will not be able to fulfil
      the request, however, this property should be false.
  * CanRaise
    - Read only (boolean)
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - If false, calling Raise will have no effect, and may raise a NotSupported error.
      If true, calling Raise will cause the media application to attempt to
      bring its user interface to the front, although it may be prevented from
      doing so (by the window manager, for example).
  * HasTrackList
    - Read only (boolean)
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - Indicates whether the `/org/mpris/MediaPlayer2` object implements the
      `org.mpris.MediaPlayer2.TrackList` interface.
  * Identity
    - Read only (string)
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - A friendly name to identify the media player to users.
    - This should usually match the name found in .desktop files
      (eg: "VLC media player").
  * DesktopEntry
    - Read only (string)
    - This property is optional. Clients should handle its absence gracefully.
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - The basename of an installed .desktop file which complies with the
      Desktop entry specification, with the ".desktop" extension stripped.
    - Example: The desktop entry file is "/usr/share/applications/vlc.desktop",
      and this property contains "vlc"
  * SupportedUriSchemes
    - Read only (strings)
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - The URI schemes supported by the media player.
    - This can be viewed as protocols supported by the player in almost all cases.
      Almost every media player will include support for the "file" scheme.
      Other common schemes are "http" and "rtsp".
    - Note that URI schemes should be lower-case.
  * SupportedMimeTypes
    - Read only (strings)
    - When this property changes, the
      `org.freedesktop.DBus.Properties.PropertiesChanged`
      signal is emitted with the new value.
    - The mime-types supported by the media player.
    - Mime-types should be in the standard format
      (eg: audio/mpeg or application/ogg).

### org.mpris.MediaPlayer2.Player

*Description* - This interface implements the methods for querying and
providing basic control over what is currently playing.

#### Methods

  * Next()
    - Skips to the next track in the tracklist.
    - If there is no next track (and endless playback and track repeat are
      both off), stop playback.
    - If playback is paused or stopped, it remains that way.
    - If [CanGoNext](#cangonext) is false, attempting to call this method
      should have no effect.
  * Previous()
    - Skips to the previous track in the tracklist.
    - If there is no previous track (and endless playback and track repeat are
      both off), stop playback.
    - If playback is paused or stopped, it remains that way.
    - If [CanGoPrevious](#cangoprevious) is false, attempting to call this
      method should have no effect.
  * Pause()
    - Pauses playback.
    - If playback is already paused, this has no effect.
    - Calling Play after this should cause playback to start again from the
      same position.
    - If [CanPause](#canpause) is false, attempting to call this method should
      have no effect.
  * PlayPause()
    - Pauses playback.
    - If playback is already paused, resumes playback.
    - If playback is stopped, starts playback.
    - If [CanPause](#canpause) is false, attempting to call this method should
      have no effect and raise an error.
  * Stop()
    - Stops playback.
    - If playback is already stopped, this has no effect.
    - Calling Play after this should cause playback to start again from the
      beginning of the track.
    - If [CanControl](#cancontrol) is false, attempting to call this method
      should have no effect and raise an error.
  * Play()
    - Starts or resumes playback.
    - If already playing, this has no effect.
    - If paused, playback resumes from the current position.
    - If there is no track to play, this has no effect.
    - If [CanPlay](#canplay) is false, attempting to call this method should
      have no effect.
  * Seek(x: Offset)
    - Parameters:
      Offset — x (Time_In_Us) Number of microseconds to seek forward.
    - Seeks forward in the current track by the specified number of microseconds.
    - A negative value seeks back.
      If this would mean seeking back further than the start of the track,
      the position is set to 0.
    - If the value passed in would mean seeking beyond the end of the track,
      acts like a call to Next.
    - If the [CanSeek](#canseek) property is false, this has no effect.
  * SetPosition(o: TrackId, x: Position)
    - Parameters:
    - TrackId — o (Track_Id) The currently playing track's identifier.
      - If this does not match the id of the currently-playing track,
      the call is ignored as "stale".
      - /org/mpris/MediaPlayer2/TrackList/NoTrack is not valid for this argument.
    - Position — x (Time_In_Us) Track position in microseconds.
      - This must be between 0 and <track_length>.
      - Sets the current track position in microseconds.
      - If the Position argument is less than 0, do nothing.
      - If the Position argument is greater than the track length, do nothing.
    - If the CanSeek property is false, this has no effect.
  * OpenUri(s: Uri)
    - Parameters
    - Uri — s (Uri) Uri of the track to load. Its uri scheme should be an
      element of the org.mpris.MediaPlayer2.SupportedUriSchemes property and the
      mime-type should match one of the elements of the
      org.mpris.MediaPlayer2.SupportedMimeTypes.
    - Opens the Uri given as an argument
    - If the playback is stopped, starts playing
    - If the uri scheme or the mime-type of the uri to open is not supported,
      this method does nothing and may raise an error. In particular, if the
      list of available uri schemes is empty, this method may not be implemented.
    - Clients should not assume that the Uri has been opened as soon as this
      method returns. They should wait until the mpris:trackid field in the
      Metadata property changes.
    - If the media player implements the TrackList interface, then the opened
      track should be made part of the tracklist, the
      org.mpris.MediaPlayer2.TrackList.TrackAdded or
      org.mpris.MediaPlayer2.TrackList.TrackListReplaced signal should be fired,
      as well as the org.freedesktop.DBus.Properties.PropertiesChanged
      signal on the tracklist interface.

#### Signals

  * Seeked(x: Position)
    - *Parameters*
    - Position — x (Time_In_Us) The new position, in microseconds.
    - Indicates that the track position has changed in a way that is
      inconsistant with the current playing state.
    - When this signal is not received, clients should assume that:
    - When playing, the position progresses according to the rate property.
    - When paused, it remains constant.
    - This signal does not need to be emitted when playback starts or when the
      track changes, unless the track is starting at an unexpected position.
      An expected position would be the last known one when going from Paused
      to Playing, and 0 when going from Stopped to Playing.

#### Properties

  * Accessed using the org.freedesktop.DBus.Properties interface.
  * PlaybackStatus -> (enum Playback_Status)
    - Read only (string)
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - The current playback status.
    - May be "Playing", "Paused" or "Stopped".
  * LoopStatus -> (enum Loop_Status)
    - Read/Write string
    - This property is optional. Clients should handle its absence gracefully.
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - The current loop / repeat status
    - *May be*:
    - "None" if the playback will stop when there are no more tracks to play
    - "Track" if the current track will start again from the begining once
      it has finished playing
    - "Playlist" if the playback loops through a list of tracks
    - If [CanControl](#cancontrol) is false, attempting to set this property
      should have no effect and raise an error.
  * Rate — d (Playback_Rate)
    - Read/Write
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - The current playback rate.
    - The value must fall in the range described by MinimumRate and MaximumRate,
      and must not be 0.0.
      If playback is paused, the PlaybackStatus property should be used to
      indicate this. A value of 0.0 should not be set by the client.
      If it is, the media player should act as though Pause was called.
    - If the media player has no ability to play at speeds other than the normal
      playback rate, this must still be implemented, and must return 1.0.
      The MinimumRate and MaximumRate properties must also be set to 1.0.
    - Not all values may be accepted by the media player.
      It is left to media player implementations to decide how to deal with values
      they cannot use; they may either ignore them or pick a "best fit" value.
      Clients are recommended to only use sensible fractions or multiples of 1
      (eg: 0.5, 0.25, 1.5, 2.0, etc).
  * Shuffle
    - Read/Write (boolean)
    - This property is optional. Clients should handle its absence gracefully.
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - A value of false indicates that playback is progressing linearly through a
      playlist, while true means playback is progressing through a playlist in
      some other order.
    - If [CanControl](#cancontrol) is false, attempting to set this property
      should have no effect and raise an error.
  * Metadata — a{sv} (Metadata_Map)
    - Read only
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - The metadata of the current element.
    - If there is a current track, this must have a "mpris:trackid" entry
      (of D-Bus type "o") at the very least, which contains a D-Bus path that
      uniquely identifies this track.
    - See the type documentation for more details.
  * Volume — d (Volume)
    - Read/Write
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - The volume level.
    - When setting, if a negative value is passed, the volume should be set to 0.0.
    - If CanControl is false, attempting to set this property should have no
      effect and raise an error.
  * Position — x (Time_In_Us)
    - Read only
    - The org.freedesktop.DBus.Properties.PropertiesChanged signal is not
      emitted when this property changes.
    - The current track position in microseconds, between 0 and the
      'mpris:length' metadata entry (see Metadata).
    - Note: If the media player allows it, the current playback position can be
      changed either the SetPosition method or the Seek method on this interface.
      If this is not the case, the CanSeek property is false, and setting this
      property has no effect and can raise an error.
    - If the playback progresses in a way that is inconstistant with the Rate
      property, the Seeked signal is emited.
  * MinimumRate — d (Playback_Rate)
    - Read only
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - The minimum value which the Rate property can take.
     Clients should not attempt to set the Rate property below this value.
    - Note that even if this value is 0.0 or negative, clients should not
      attempt to set the Rate property to 0.0.
    - This value should always be 1.0 or less.
  * MaximumRate — d (Playback_Rate)
    - Read only
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - The maximum value which the Rate property can take.
      Clients should not attempt to set the Rate property above this value.
    - This value should always be 1.0 or greater.
  * CanGoNext — b
    - Read only
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - Whether the client can call the Next method on this interface and expect
      the current track to change.
    - If it is unknown whether a call to Next will be successful (for example,
      when streaming tracks), this property should be set to true.
    - If CanControl is false, this property should also be false.
  * CanGoPrevious — b
    - Read only
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - Whether the client can call the Previous method on this interface and
      expect the current track to change.
    - If it is unknown whether a call to Previous will be successful (for
      example, when streaming tracks), this property should be set to true.
    - If CanControl is false, this property should also be false.
  * CanPlay — b
    - Read only
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - Whether playback can be started using Play or PlayPause.
    - Note that this is related to whether there is a "current track": the value
      should not depend on whether the track is currently paused or playing.
      In fact, if a track is currently playing (and CanControl is true), this
      should be true.
    - If CanControl is false, this property should also be false.
  * CanPause — b
    - Read only
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - Whether playback can be paused using Pause or PlayPause.
    - Note that this is an intrinsic property of the current track: its value
      should not depend on whether the track is currently paused or playing.
      In fact, if playback is currently paused (and CanControl is true), this
      should be true.
    - If CanControl is false, this property should also be false.
  * CanSeek — b
    - Read only
    - When this property changes, the
      org.freedesktop.DBus.Properties.PropertiesChanged
      signal is emitted with the new value.
    - Whether the client can control the playback position using Seek and
      SetPosition. This may be different for different tracks.
    - If CanControl is false, this property should also be false.
  * CanControl — b
    - Read only
    - The org.freedesktop.DBus.Properties.PropertiesChanged signal is not
      emitted when this property changes.
    - Whether the media player may be controlled over this interface.
    - This property is not expected to change, as it describes an intrinsic
      capability of the implementation.
    - If this is false, clients should assume that all properties on this
      interface are read-only (and will raise errors if writing to them is
      attempted), no methods are implemented and all other properties starting
      with "Can" are also false.

#### Types

  * Track_Id — o: Simple Type
    - Unique track identifier.
    - If the media player implements the TrackList interface and allows the
      same track to appear multiple times in the tracklist, this must be
      unique within the scope of the tracklist.
    - Note that this should be a valid D-Bus object id, although clients
      should not assume that any object is actually exported with any
      interfaces at that path.
    - Media players may not use any paths starting with /org/mpris unless
      explicitly allowed by this specification.
      Such paths are intended to have special meaning, such as
      /org/mpris/MediaPlayer2/TrackList/NoTrack to indicate "no track".
  * Playback_Rate — d: Simple Type
    - A playback rate
    - This is a multiplier, so a value of 0.5 indicates that playback is
      happening at half speed, while 1.5 means that 1.5 seconds of "track time"
      is consumed every second.
  * Volume — d: Simple Type
    - Audio volume level
    - 0.0 means mute.
    - 1.0 is a sensible maximum volume level (ex: 0dB).
    - Note that the volume may be higher than 1.0, although generally clients
      should not attempt to set it above 1.0.
  * Time_In_Us — x: Simple Type
    - Time in microseconds.
  * Playback_Status — s: Enum
    - A playback state.
    - Playing (Playing): A track is currently playing.
    - Paused (Paused): A track is currently paused.
    - Stopped (Stopped): There is no track currently playing.
  * Loop_Status — s: Enum
    - A repeat / loop status
    - None (None): The playback will stop when there are no more tracks to play
    - Track (Track): The current track will start again from the begining once
      it has finished playing
    - Playlist (Playlist): The playback loops through a list of tracks

### org.mpris.MediaPlayer2.TrackList  TODO

### org.mpris.MediaPlayer2.Playlists  TODO
