Attribute VB_Name = "modBass"
' BASS 2.4 Visual Basic module
' Copyright (c) 1999-2019 Un4seen Developments Ltd.
'
' See the BASS.CHM file for more detailed documentation

' NOTE: VB does not support 64-bit integers, so VB users only have access
'       to the low 32-bits of 64-bit return values. 64-bit parameters can
'       be specified though, using the "64" version of the function.

' NOTE: Use "StrPtr(filename)" to pass a filename to the BASS_MusicLoad,
'       BASS_SampleLoad and BASS_StreamCreateFile functions.

' NOTE: Use the VBStrFromAnsiPtr function to convert "char *" to VB "String".

Global Const BASSVERSION = &H204    'API version
Global Const BASSVERSIONTEXT = "2.4"

Global Const BASSTRUE As Long = 1   'Use this instead of VB Booleans
Global Const BASSFALSE As Long = 0  'Use this instead of VB Booleans

' Error codes returned by BASS_ErrorGetCode
Global Const BASS_OK = 0               'all is OK
Global Const BASS_ERROR_MEM = 1        'memory error
Global Const BASS_ERROR_FILEOPEN = 2   'can't open the file
Global Const BASS_ERROR_DRIVER = 3     'can't find a free sound driver
Global Const BASS_ERROR_BUFLOST = 4    'the sample buffer was lost
Global Const BASS_ERROR_HANDLE = 5     'invalid handle
Global Const BASS_ERROR_FORMAT = 6     'unsupported sample format
Global Const BASS_ERROR_POSITION = 7   'invalid position
Global Const BASS_ERROR_INIT = 8       'BASS_Init has not been successfully called
Global Const BASS_ERROR_START = 9      'BASS_Start has not been successfully called
Global Const BASS_ERROR_ALREADY = 14   'already initialized/paused/whatever
Global Const BASS_ERROR_NOCHAN = 18    'can't get a free channel
Global Const BASS_ERROR_ILLTYPE = 19   'an illegal type was specified
Global Const BASS_ERROR_ILLPARAM = 20  'an illegal parameter was specified
Global Const BASS_ERROR_NO3D = 21      'no 3D support
Global Const BASS_ERROR_NOEAX = 22     'no EAX support
Global Const BASS_ERROR_DEVICE = 23    'illegal device number
Global Const BASS_ERROR_NOPLAY = 24    'not playing
Global Const BASS_ERROR_FREQ = 25      'illegal sample rate
Global Const BASS_ERROR_NOTFILE = 27   'the stream is not a file stream
Global Const BASS_ERROR_NOHW = 29      'no hardware voices available
Global Const BASS_ERROR_EMPTY = 31     'the MOD music has no sequence data
Global Const BASS_ERROR_NONET = 32     'no internet connection could be opened
Global Const BASS_ERROR_CREATE = 33    'couldn't create the file
Global Const BASS_ERROR_NOFX = 34      'effects are not available
Global Const BASS_ERROR_NOTAVAIL = 37  'requested data/action is not available
Global Const BASS_ERROR_DECODE = 38    'the channel is/isn't a "decoding channel"
Global Const BASS_ERROR_DX = 39        'a sufficient DirectX version is not installed
Global Const BASS_ERROR_TIMEOUT = 40   'connection timedout
Global Const BASS_ERROR_FILEFORM = 41  'unsupported file format
Global Const BASS_ERROR_SPEAKER = 42   'unavailable speaker
Global Const BASS_ERROR_VERSION = 43   'invalid BASS version (used by add-ons)
Global Const BASS_ERROR_CODEC = 44     'codec is not available/supported
Global Const BASS_ERROR_ENDED = 45     'the channel/file has ended
Global Const BASS_ERROR_BUSY = 46      'the device is busy
Global Const BASS_ERROR_UNKNOWN = -1   'some other mystery problem

' BASS_SetConfig options
Global Const BASS_CONFIG_BUFFER = 0
Global Const BASS_CONFIG_UPDATEPERIOD = 1
Global Const BASS_CONFIG_GVOL_SAMPLE = 4
Global Const BASS_CONFIG_GVOL_STREAM = 5
Global Const BASS_CONFIG_GVOL_MUSIC = 6
Global Const BASS_CONFIG_CURVE_VOL = 7
Global Const BASS_CONFIG_CURVE_PAN = 8
Global Const BASS_CONFIG_FLOATDSP = 9
Global Const BASS_CONFIG_3DALGORITHM = 10
Global Const BASS_CONFIG_NET_TIMEOUT = 11
Global Const BASS_CONFIG_NET_BUFFER = 12
Global Const BASS_CONFIG_PAUSE_NOPLAY = 13
Global Const BASS_CONFIG_NET_PREBUF = 15
Global Const BASS_CONFIG_NET_PASSIVE = 18
Global Const BASS_CONFIG_REC_BUFFER = 19
Global Const BASS_CONFIG_NET_PLAYLIST = 21
Global Const BASS_CONFIG_MUSIC_VIRTUAL = 22
Global Const BASS_CONFIG_VERIFY = 23
Global Const BASS_CONFIG_UPDATETHREADS = 24
Global Const BASS_CONFIG_DEV_BUFFER = 27
Global Const BASS_CONFIG_REC_LOOPBACK = 28
Global Const BASS_CONFIG_VISTA_TRUEPOS = 30
Global Const BASS_CONFIG_DEV_DEFAULT = 36
Global Const BASS_CONFIG_NET_READTIMEOUT = 37
Global Const BASS_CONFIG_VISTA_SPEAKERS = 38
Global Const BASS_CONFIG_MF_DISABLE = 40
Global Const BASS_CONFIG_HANDLES = 41
Global Const BASS_CONFIG_UNICODE = 42
Global Const BASS_CONFIG_SRC = 43
Global Const BASS_CONFIG_SRC_SAMPLE = 44
Global Const BASS_CONFIG_ASYNCFILE_BUFFER = 45
Global Const BASS_CONFIG_OGG_PRESCAN = 47
Global Const BASS_CONFIG_MF_VIDEO = 48
Global Const BASS_CONFIG_VERIFY_NET = 52
Global Const BASS_CONFIG_DEV_PERIOD = 53
Global Const BASS_CONFIG_FLOAT = 54
Global Const BASS_CONFIG_NET_SEEK = 56
Global Const BASS_CONFIG_NET_PLAYLIST_DEPTH = 59
Global Const BASS_CONFIG_NET_PREBUF_WAIT = 60
Global Const BASS_CONFIG_WASAPI_PERSIST = 65
Global Const BASS_CONFIG_REC_WASAPI = 66

' BASS_SetConfigPtr options
Global Const BASS_CONFIG_NET_AGENT = 16
Global Const BASS_CONFIG_NET_PROXY = 17
Global Const BASS_CONFIG_LIBSSL = 64

' BASS_ASIO_Init flags
Global Const BASS_DEVICE_8BITS = 1     '8 bit
Global Const BASS_DEVICE_MONO = 2      'mono
Global Const BASS_DEVICE_3D = 4        'enable 3D functionality
Global Const BASS_DEVICE_LATENCY = &H100 'calculate device latency (BASS_INFO struct)
Global Const BASS_DEVICE_CPSPEAKERS = &H400 'detect speakers via Windows control panel
Global Const BASS_DEVICE_SPEAKERS = &H800 'force enabling of speaker assignment
Global Const BASS_DEVICE_NOSPEAKER = &H1000 'ignore speaker arrangement
Global Const BASS_DEVICE_DSOUND = &H40000 ' use DirectSound output

' DirectSound interfaces (for use with BASS_GetDSoundObject)
Global Const BASS_OBJECT_DS = 1                     ' DirectSound
Global Const BASS_OBJECT_DS3DL = 2                  'IDirectSound3DListener

' Device info structure
Type BASS_DEVICEINFO
    name As Long          ' description
    driver As Long        ' driver
    flags As Long
End Type

' BASS_DEVICEINFO flags
Global Const BASS_DEVICE_ENABLED = 1
Global Const BASS_DEVICE_DEFAULT = 2
Global Const BASS_DEVICE_INIT = 4

Global Const BASS_DEVICE_TYPE_MASK = &Hff000000
Global Const BASS_DEVICE_TYPE_NETWORK = &H01000000
Global Const BASS_DEVICE_TYPE_SPEAKERS = &H02000000
Global Const BASS_DEVICE_TYPE_LINE = &H03000000
Global Const BASS_DEVICE_TYPE_HEADPHONES = &H04000000
Global Const BASS_DEVICE_TYPE_MICROPHONE = &H05000000
Global Const BASS_DEVICE_TYPE_HEADSET = &H06000000
Global Const BASS_DEVICE_TYPE_HANDSET = &H07000000
Global Const BASS_DEVICE_TYPE_DIGITAL = &H08000000
Global Const BASS_DEVICE_TYPE_SPDIF = &H09000000
Global Const BASS_DEVICE_TYPE_HDMI = &H0a000000
Global Const BASS_DEVICE_TYPE_DISPLAYPORT = &H40000000

Type BASS_INFO
    flags As Long         ' device capabilities (DSCAPS_xxx flags)
    hwsize As Long        ' size of total device hardware memory
    hwfree As Long        ' size of free device hardware memory
    freesam As Long       ' number of free sample slots in the hardware
    free3d As Long        ' number of free 3D sample slots in the hardware
    minrate As Long       ' min sample rate supported by the hardware
    maxrate As Long       ' max sample rate supported by the hardware
    eax As Long           ' device supports EAX? (always BASSFALSE if BASS_DEVICE_3D was not used)
    minbuf As Long        ' recommended minimum buffer length in ms (requires BASS_DEVICE_LATENCY)
    dsver As Long         ' DirectSound version
    latency As Long       ' delay (in ms) before start of playback (requires BASS_DEVICE_LATENCY)
    initflags As Long     ' BASS_Init "flags" parameter
    speakers As Long      ' number of speakers available
    freq As Long          ' current output rate
End Type

' BASS_INFO flags (from DSOUND.H)
Global Const DSCAPS_CONTINUOUSRATE = 16    ' supports all sample rates between min/maxrate
Global Const DSCAPS_EMULDRIVER = 32        ' device does NOT have hardware DirectSound support
Global Const DSCAPS_CERTIFIED = 64         ' device driver has been certified by Microsoft
Global Const DSCAPS_SECONDARYMONO = 256    ' mono
Global Const DSCAPS_SECONDARYSTEREO = 512  ' stereo
Global Const DSCAPS_SECONDARY8BIT = 1024   ' 8 bit
Global Const DSCAPS_SECONDARY16BIT = 2048  ' 16 bit

' Recording device info structure
Type BASS_RECORDINFO
    flags As Long         ' device capabilities (DSCCAPS_xxx flags)
    formats As Long       ' supported standard formats (WAVE_FORMAT_xxx flags)
    inputs As Long        ' number of inputs
    singlein As Long      ' BASSTRUE = only 1 input can be set at a time
    freq As Long          ' current input rate
End Type

' BASS_RECORDINFO flags (from DSOUND.H)
Global Const DSCCAPS_EMULDRIVER = DSCAPS_EMULDRIVER ' device does NOT have hardware DirectSound recording support
Global Const DSCCAPS_CERTIFIED = DSCAPS_CERTIFIED   ' device driver has been certified by Microsoft

' defines for formats field of BASS_RECORDINFO (from MMSYSTEM.H)
Global Const WAVE_FORMAT_1M08 = &H1          ' 11.025 kHz, Mono,   8-bit
Global Const WAVE_FORMAT_1S08 = &H2          ' 11.025 kHz, Stereo, 8-bit
Global Const WAVE_FORMAT_1M16 = &H4          ' 11.025 kHz, Mono,   16-bit
Global Const WAVE_FORMAT_1S16 = &H8          ' 11.025 kHz, Stereo, 16-bit
Global Const WAVE_FORMAT_2M08 = &H10         ' 22.05  kHz, Mono,   8-bit
Global Const WAVE_FORMAT_2S08 = &H20         ' 22.05  kHz, Stereo, 8-bit
Global Const WAVE_FORMAT_2M16 = &H40         ' 22.05  kHz, Mono,   16-bit
Global Const WAVE_FORMAT_2S16 = &H80         ' 22.05  kHz, Stereo, 16-bit
Global Const WAVE_FORMAT_4M08 = &H100        ' 44.1   kHz, Mono,   8-bit
Global Const WAVE_FORMAT_4S08 = &H200        ' 44.1   kHz, Stereo, 8-bit
Global Const WAVE_FORMAT_4M16 = &H400        ' 44.1   kHz, Mono,   16-bit
Global Const WAVE_FORMAT_4S16 = &H800        ' 44.1   kHz, Stereo, 16-bit

' Sample info structure
Type BASS_SAMPLE
    freq As Long          ' default playback rate
    volume As Single      ' default volume (0-100)
    pan As Single         ' default pan (-100=left, 0=middle, 100=right)
    flags As Long         ' BASS_SAMPLE_xxx flags
    length As Long        ' length (in samples, not bytes)
    max As Long           ' maximum simultaneous playbacks
    origres As Long       ' original resolution
    chans As Long         ' number of channels
    mingap As Long        ' minimum gap (ms) between creating channels
    mode3d As Long        ' BASS_3DMODE_xxx mode
    mindist As Single     ' minimum distance
    MAXDIST As Single     ' maximum distance
    iangle As Long        ' angle of inside projection cone
    oangle As Long        ' angle of outside projection cone
    outvol As Single      ' delta-volume outside the projection cone
    vam As Long           ' voice allocation/management flags (BASS_VAM_xxx)
    priority As Long      ' priority (0=lowest, &Hffffffff=highest)
End Type

Global Const BASS_SAMPLE_8BITS = 1          ' 8 bit
Global Const BASS_SAMPLE_FLOAT = 256        ' 32 bit floating-point
Global Const BASS_SAMPLE_MONO = 2           ' mono
Global Const BASS_SAMPLE_LOOP = 4           ' looped
Global Const BASS_SAMPLE_3D = 8             ' 3D functionality
Global Const BASS_SAMPLE_SOFTWARE = 16      ' not using hardware mixing
Global Const BASS_SAMPLE_MUTEMAX = 32       ' mute at max distance (3D only)
Global Const BASS_SAMPLE_VAM = 64           ' DX7 voice allocation & management
Global Const BASS_SAMPLE_FX = 128           ' old implementation of DX8 effects
Global Const BASS_SAMPLE_OVER_VOL = &H10000 ' override lowest volume
Global Const BASS_SAMPLE_OVER_POS = &H20000 ' override longest playing
Global Const BASS_SAMPLE_OVER_DIST = &H30000 ' override furthest from listener (3D only)

Global Const BASS_STREAM_PRESCAN = &H20000  ' enable pin-point seeking/length (MP3/MP2/MP1)
Global Const BASS_STREAM_AUTOFREE = &H40000 ' automatically free the stream when it stop/ends
Global Const BASS_STREAM_RESTRATE = &H80000 ' restrict the download rate of internet file streams
Global Const BASS_STREAM_BLOCK = &H100000   ' download/play internet file stream in small blocks
Global Const BASS_STREAM_DECODE = &H200000  ' don't play the stream, only decode (BASS_ChannelGetData)
Global Const BASS_STREAM_STATUS = &H800000  ' give server status info (HTTP/ICY tags) in DOWNLOADPROC

Global Const BASS_MP3_IGNOREDELAY = &H200   ' ignore LAME/Xing/VBRI/iTunes delay & padding info
Global Const BASS_MP3_SETPOS = BASS_STREAM_PRESCAN

Global Const BASS_MUSIC_FLOAT = BASS_SAMPLE_FLOAT
Global Const BASS_MUSIC_MONO = BASS_SAMPLE_MONO
Global Const BASS_MUSIC_LOOP = BASS_SAMPLE_LOOP
Global Const BASS_MUSIC_3D = BASS_SAMPLE_3D
Global Const BASS_MUSIC_FX = BASS_SAMPLE_FX
Global Const BASS_MUSIC_AUTOFREE = BASS_STREAM_AUTOFREE
Global Const BASS_MUSIC_DECODE = BASS_STREAM_DECODE
Global Const BASS_MUSIC_PRESCAN = BASS_STREAM_PRESCAN ' calculate playback length
Global Const BASS_MUSIC_CALCLEN = BASS_MUSIC_PRESCAN
Global Const BASS_MUSIC_RAMP = &H200        ' normal ramping
Global Const BASS_MUSIC_RAMPS = &H400       ' sensitive ramping
Global Const BASS_MUSIC_SURROUND = &H800    ' surround sound
Global Const BASS_MUSIC_SURROUND2 = &H1000  ' surround sound (mode 2)
Global Const BASS_MUSIC_FT2PAN = &H2000     ' apply FastTracker 2 panning to XM files
Global Const BASS_MUSIC_FT2MOD = &H2000     ' play .MOD as FastTracker 2 does
Global Const BASS_MUSIC_PT1MOD = &H4000     ' play .MOD as ProTracker 1 does
Global Const BASS_MUSIC_NONINTER = &H10000  ' non-interpolated sample mixing
Global Const BASS_MUSIC_SINCINTER = &H800000 ' sinc interpolated sample mixing
Global Const BASS_MUSIC_POSRESET = 32768  ' stop all notes when moving position
Global Const BASS_MUSIC_POSRESETEX = &H400000 ' stop all notes and reset bmp/etc when moving position
Global Const BASS_MUSIC_STOPBACK = &H80000  ' stop the music on a backwards jump effect
Global Const BASS_MUSIC_NOSAMPLE = &H100000 ' don't load the samples

' Speaker assignment flags
Global Const BASS_SPEAKER_FRONT = &H1000000 ' front speakers
Global Const BASS_SPEAKER_REAR = &H2000000  ' rear/side speakers
Global Const BASS_SPEAKER_CENLFE = &H3000000 ' center & LFE speakers (5.1)
Global Const BASS_SPEAKER_REAR2 = &H4000000 ' rear center speakers (7.1)
Global Const BASS_SPEAKER_LEFT = &H10000000 ' modifier: left
Global Const BASS_SPEAKER_RIGHT = &H20000000 ' modifier: right
Global Const BASS_SPEAKER_FRONTLEFT = BASS_SPEAKER_FRONT Or BASS_SPEAKER_LEFT
Global Const BASS_SPEAKER_FRONTRIGHT = BASS_SPEAKER_FRONT Or BASS_SPEAKER_RIGHT
Global Const BASS_SPEAKER_REARLEFT = BASS_SPEAKER_REAR Or BASS_SPEAKER_LEFT
Global Const BASS_SPEAKER_REARRIGHT = BASS_SPEAKER_REAR Or BASS_SPEAKER_RIGHT
Global Const BASS_SPEAKER_CENTER = BASS_SPEAKER_CENLFE Or BASS_SPEAKER_LEFT
Global Const BASS_SPEAKER_LFE = BASS_SPEAKER_CENLFE Or BASS_SPEAKER_RIGHT
Global Const BASS_SPEAKER_REAR2LEFT = BASS_SPEAKER_REAR2 Or BASS_SPEAKER_LEFT
Global Const BASS_SPEAKER_REAR2RIGHT = BASS_SPEAKER_REAR2 Or BASS_SPEAKER_RIGHT

Global Const BASS_ASYNCFILE = &H40000000
Global Const BASS_UNICODE = &H80000000

Global Const BASS_RECORD_PAUSE = 32768 ' start recording paused

' DX7 voice allocation flags
Global Const BASS_VAM_HARDWARE = 1
Global Const BASS_VAM_SOFTWARE = 2
Global Const BASS_VAM_TERM_TIME = 4
Global Const BASS_VAM_TERM_DIST = 8
Global Const BASS_VAM_TERM_PRIO = 16

' Channel info structure
Type BASS_CHANNELINFO
    freq As Long          ' default playback rate
    chans As Long         ' channels
    flags As Long         ' BASS_SAMPLE/STREAM/MUSIC/SPEAKER flags
    ctype As Long         ' type of channel
    origres As Long       ' original resolution
    plugin As Long        ' plugin
    sample As Long        ' sample
    filename As Long      ' filename
End Type

Global Const BASS_ORIGRES_FLOAT = &H10000

' BASS_CHANNELINFO types
Global Const BASS_CTYPE_SAMPLE = 1
Global Const BASS_CTYPE_RECORD = 2
Global Const BASS_CTYPE_STREAM = &H10000
Global Const BASS_CTYPE_STREAM_OGG = &H10002
Global Const BASS_CTYPE_STREAM_MP1 = &H10003
Global Const BASS_CTYPE_STREAM_MP2 = &H10004
Global Const BASS_CTYPE_STREAM_MP3 = &H10005
Global Const BASS_CTYPE_STREAM_AIFF = &H10006
Global Const BASS_CTYPE_STREAM_MF = &H10008
Global Const BASS_CTYPE_STREAM_DUMMY = &H18000
Global Const BASS_CTYPE_STREAM_DEVICE = &H18001
Global Const BASS_CTYPE_STREAM_WAV = &H40000 ' WAVE flag, LOWORD=codec
Global Const BASS_CTYPE_STREAM_WAV_PCM = &H50001
Global Const BASS_CTYPE_STREAM_WAV_FLOAT = &H50003
Global Const BASS_CTYPE_MUSIC_MOD = &H20000
Global Const BASS_CTYPE_MUSIC_MTM = &H20001
Global Const BASS_CTYPE_MUSIC_S3M = &H20002
Global Const BASS_CTYPE_MUSIC_XM = &H20003
Global Const BASS_CTYPE_MUSIC_IT = &H20004
Global Const BASS_CTYPE_MUSIC_MO3 = &H100    ' MO3 flag

Type BASS_PLUGINFORM
    ctype As Long         ' channel type
    name As Long          ' format description
    exts As Long          ' file extension filter (*.ext1;*.ext2;etc...)
End Type

Type BASS_PLUGININFO
    Version As Long       ' version (same form as BASS_GetVersion)
    formatc As Long       ' number of formats
    formats As Long       ' the array of formats
End Type

' 3D vector (for 3D positions/velocities/orientations)
Type BASS_3DVECTOR
    X As Single           ' +=right, -=left
    Y As Single           ' +=up, -=down
    z As Single           ' +=front, -=behind
End Type

' 3D channel modes
Global Const BASS_3DMODE_NORMAL = 0     ' normal 3D processing
Global Const BASS_3DMODE_RELATIVE = 1   ' position is relative to the listener
Global Const BASS_3DMODE_OFF = 2        ' no 3D processing

' software 3D mixing algorithms (used with BASS_CONFIG_3DALGORITHM)
Global Const BASS_3DALG_DEFAULT = 0
Global Const BASS_3DALG_OFF = 1
Global Const BASS_3DALG_FULL = 2
Global Const BASS_3DALG_LIGHT = 3

' EAX environments, use with BASS_SetEAXParameters
Global Const EAX_ENVIRONMENT_GENERIC = 0
Global Const EAX_ENVIRONMENT_PADDEDCELL = 1
Global Const EAX_ENVIRONMENT_ROOM = 2
Global Const EAX_ENVIRONMENT_BATHROOM = 3
Global Const EAX_ENVIRONMENT_LIVINGROOM = 4
Global Const EAX_ENVIRONMENT_STONEROOM = 5
Global Const EAX_ENVIRONMENT_AUDITORIUM = 6
Global Const EAX_ENVIRONMENT_CONCERTHALL = 7
Global Const EAX_ENVIRONMENT_CAVE = 8
Global Const EAX_ENVIRONMENT_ARENA = 9
Global Const EAX_ENVIRONMENT_HANGAR = 10
Global Const EAX_ENVIRONMENT_CARPETEDHALLWAY = 11
Global Const EAX_ENVIRONMENT_HALLWAY = 12
Global Const EAX_ENVIRONMENT_STONECORRIDOR = 13
Global Const EAX_ENVIRONMENT_ALLEY = 14
Global Const EAX_ENVIRONMENT_FOREST = 15
Global Const EAX_ENVIRONMENT_CITY = 16
Global Const EAX_ENVIRONMENT_MOUNTAINS = 17
Global Const EAX_ENVIRONMENT_QUARRY = 18
Global Const EAX_ENVIRONMENT_PLAIN = 19
Global Const EAX_ENVIRONMENT_PARKINGLOT = 20
Global Const EAX_ENVIRONMENT_SEWERPIPE = 21
Global Const EAX_ENVIRONMENT_UNDERWATER = 22
Global Const EAX_ENVIRONMENT_DRUGGED = 23
Global Const EAX_ENVIRONMENT_DIZZY = 24
Global Const EAX_ENVIRONMENT_PSYCHOTIC = 25
Global Const EAX_ENVIRONMENT_COUNT = 26 ' total number of environments

Global Const BASS_STREAMPROC_END = &H80000000 ' end of user stream flag

' special STREAMPROCs
Global Const STREAMPROC_DUMMY = 0 ' "dummy" stream
Global Const STREAMPROC_PUSH = -1 ' push stream
Global Const STREAMPROC_DEVICE = -2 ' device mix stream
Global Const STREAMPROC_DEVICE_3D = -3 ' device 3D mix stream

' BASS_StreamCreateFileUser file systems
Global Const STREAMFILE_NOBUFFER = 0
Global Const STREAMFILE_BUFFER = 1
Global Const STREAMFILE_BUFFERPUSH = 2

Type BASS_FILEPROCS
    close As Long
    length As Long
    read As Long
    seek As Long
End Type

' BASS_StreamPutFileData options
Global Const BASS_FILEDATA_END = 0 ' end & close the file

' BASS_StreamGetFilePosition modes
Global Const BASS_FILEPOS_CURRENT = 0
Global Const BASS_FILEPOS_DECODE = BASS_FILEPOS_CURRENT
Global Const BASS_FILEPOS_DOWNLOAD = 1
Global Const BASS_FILEPOS_END = 2
Global Const BASS_FILEPOS_START = 3
Global Const BASS_FILEPOS_CONNECTED = 4
Global Const BASS_FILEPOS_BUFFER = 5
Global Const BASS_FILEPOS_SOCKET = 6
Global Const BASS_FILEPOS_ASYNCBUF = 7
Global Const BASS_FILEPOS_SIZE = 8
Global Const BASS_FILEPOS_BUFFERING = 9

' BASS_ChannelSetSync types
Global Const BASS_SYNC_POS = 0
Global Const BASS_SYNC_END = 2
Global Const BASS_SYNC_META = 4
Global Const BASS_SYNC_SLIDE = 5
Global Const BASS_SYNC_STALL = 6
Global Const BASS_SYNC_DOWNLOAD = 7
Global Const BASS_SYNC_FREE = 8
Global Const BASS_SYNC_SETPOS = 11
Global Const BASS_SYNC_MUSICPOS = 10
Global Const BASS_SYNC_MUSICINST = 1
Global Const BASS_SYNC_MUSICFX = 3
Global Const BASS_SYNC_OGG_CHANGE = 12
Global Const BASS_SYNC_DEV_FAIL = 14
Global Const BASS_SYNC_DEV_FORMAT = 15
Global Const BASS_SYNC_MIXTIME = &H40000000 ' flag: sync at mixtime, else at playtime
Global Const BASS_SYNC_ONETIME = &H80000000 ' flag: sync only once, else continuously

' BASS_ChannelIsActive return values
Global Const BASS_ACTIVE_STOPPED = 0
Global Const BASS_ACTIVE_PLAYING = 1
Global Const BASS_ACTIVE_STALLED = 2
Global Const BASS_ACTIVE_PAUSED = 3
Global Const BASS_ACTIVE_PAUSED_DEVICE = 4

' Channel attributes
Global Const BASS_ATTRIB_FREQ = 1
Global Const BASS_ATTRIB_VOL = 2
Global Const BASS_ATTRIB_PAN = 3
Global Const BASS_ATTRIB_EAXMIX = 4
Global Const BASS_ATTRIB_NOBUFFER = 5
Global Const BASS_ATTRIB_VBR = 6
Global Const BASS_ATTRIB_CPU = 7
Global Const BASS_ATTRIB_SRC = 8
Global Const BASS_ATTRIB_NET_RESUME = 9
Global Const BASS_ATTRIB_SCANINFO = 10
Global Const BASS_ATTRIB_NORAMP = 11
Global Const BASS_ATTRIB_BITRATE = 12
Global Const BASS_ATTRIB_BUFFER = 13
Global Const BASS_ATTRIB_MUSIC_AMPLIFY = &H100
Global Const BASS_ATTRIB_MUSIC_PANSEP = &H101
Global Const BASS_ATTRIB_MUSIC_PSCALER = &H102
Global Const BASS_ATTRIB_MUSIC_BPM = &H103
Global Const BASS_ATTRIB_MUSIC_SPEED = &H104
Global Const BASS_ATTRIB_MUSIC_VOL_GLOBAL = &H105
Global Const BASS_ATTRIB_MUSIC_ACTIVE = &H106
Global Const BASS_ATTRIB_MUSIC_VOL_CHAN = &H200 ' + channel #
Global Const BASS_ATTRIB_MUSIC_VOL_INST = &H300 ' + instrument #

' BASS_ChannelSlideAttribute flags
Global Const BASS_SLIDE_LOG = &H1000000

' BASS_ChannelGetData flags
Global Const BASS_DATA_AVAILABLE = 0         ' query how much data is buffered
Global Const BASS_DATA_FIXED = &H20000000    ' flag: return 8.24 fixed-point data
Global Const BASS_DATA_FLOAT = &H40000000    ' flag: return floating-point sample data
Global Const BASS_DATA_FFT256 = &H80000000   ' 256 sample FFT
Global Const BASS_DATA_FFT512 = &H80000001   ' 512 FFT
Global Const BASS_DATA_FFT1024 = &H80000002  ' 1024 FFT
Global Const BASS_DATA_FFT2048 = &H80000003  ' 2048 FFT
Global Const BASS_DATA_FFT4096 = &H80000004  ' 4096 FFT
Global Const BASS_DATA_FFT8192 = &H80000005  ' 8192 FFT
Global Const BASS_DATA_FFT16384 = &H80000006 ' 16384 FFT
Global Const BASS_DATA_FFT32768 = &H80000007 ' 32768 FFT
Global Const BASS_DATA_FFT_INDIVIDUAL = &H10 ' FFT flag: FFT for each channel, else all combined
Global Const BASS_DATA_FFT_NOWINDOW = &H20   ' FFT flag: no Hanning window
Global Const BASS_DATA_FFT_REMOVEDC = &H40   ' FFT flag: pre-remove DC bias
Global Const BASS_DATA_FFT_COMPLEX = &H80    ' FFT flag: return complex data
Global Const BASS_DATA_FFT_NYQUIST = &H100   ' FFT flag: return extra Nyquist value

' BASS_ChannelGetLevelEx flags
Global Const BASS_LEVEL_MONO = 1
Global Const BASS_LEVEL_STEREO = 2
Global Const BASS_LEVEL_RMS = 4
Global Const BASS_LEVEL_VOLPAN = 8

' BASS_ChannelGetTags types : what's returned
Global Const BASS_TAG_ID3 = 0                ' ID3v1 tags : TAG_ID3 structure
Global Const BASS_TAG_ID3V2 = 1              ' ID3v2 tags : variable length block
Global Const BASS_TAG_OGG = 2                ' OGG comments : series of null-terminated UTF-8 strings
Global Const BASS_TAG_HTTP = 3               ' HTTP headers : series of null-terminated ANSI strings
Global Const BASS_TAG_ICY = 4                ' ICY headers : series of null-terminated ANSI strings
Global Const BASS_TAG_META = 5               ' ICY metadata : ANSI string
Global Const BASS_TAG_APE = 6                ' APEv2 tags : series of null-terminated UTF-8 strings
Global Const BASS_TAG_MP4 = 7                ' MP4/iTunes metadata : series of null-terminated UTF-8 strings
Global Const BASS_TAG_WMA = 8                ' WMA tags : series of null-terminated UTF-8 strings
Global Const BASS_TAG_VENDOR = 9             ' OGG encoder : UTF-8 string
Global Const BASS_TAG_LYRICS3 = 10           ' Lyric3v2 tag : ASCII string
Global Const BASS_TAG_CA_CODEC = 11          ' CoreAudio codec info : TAG_CA_CODEC structure
Global Const BASS_TAG_MF = 13                ' Media Foundation tags : series of null-terminated UTF-8 strings
Global Const BASS_TAG_WAVEFORMAT = 14        ' WAVE format : WAVEFORMATEEX structure
Global Const BASS_TAG_RIFF_INFO = &H100      ' RIFF "INFO" tags : series of null-terminated ANSI strings
Global Const BASS_TAG_RIFF_BEXT = &H101      ' RIFF/BWF "bext" tags : TAG_BEXT structure
Global Const BASS_TAG_RIFF_CART = &H102      ' RIFF/BWF "cart" tags : TAG_CART structure
Global Const BASS_TAG_RIFF_DISP = &H103      ' RIFF "DISP" text tag : ANSI string
Global Const BASS_TAG_RIFF_CUE = &H104       ' RIFF "cue " chunk : TAG_CUE structure
Global Const BASS_TAG_RIFF_SMPL = &H105      ' RIFF "smpl" chunk : TAG_SMPL structure
Global Const BASS_TAG_APE_BINARY = &H1000    ' + index #, binary APEv2 tag : TAG_APE_BINARY structure
Global Const BASS_TAG_MUSIC_NAME = &H10000   ' MOD music name : ANSI string
Global Const BASS_TAG_MUSIC_ORDERS = &H10002 ' MOD order list : BYTE array of pattern numbers
Global Const BASS_TAG_MUSIC_MESSAGE = &H10001 ' MOD message : ANSI string
Global Const BASS_TAG_MUSIC_AUTH = &H10003   ' MOD author : UTF-8 string
Global Const BASS_TAG_MUSIC_INST = &H10100   ' + instrument #, MOD instrument name : ANSI string
Global Const BASS_TAG_MUSIC_SAMPLE = &H10300 ' + sample #, MOD sample name : ANSI string

' ID3v1 tag structure
Type TAG_ID3
    id As String * 3
    title As String * 30
    artist As String * 30
    album As String * 30
    year As String * 4
    comment As String * 30
    genre As Byte
End Type

' Binary APEv2 tag structure
Type TAG_APE_BINARY
    key As Long
    data As Long
    length As Long
End Type

' BWF "bext" tag structure
Type TAG_BEXT
    Description As String * 256         ' description
    Originator As String * 32           ' name of the originator
    OriginatorReference As String * 32  ' reference of the originator
    OriginationDate As String * 10      ' date of creation (yyyy-mm-dd)
    OriginationTime As String * 8       ' time of creation (hh-mm-ss)
    TimeReferenceLo As Long             ' low 32 bits of first sample count since midnight (little-endian)
    TimeReferenceHi As Long             ' high 32 bits of first sample count since midnight (little-endian)
    Version As Integer                  ' BWF version (little-endian)
    UMID(0 To 63) As Byte               ' SMPTE UMID
    Reserved(0 To 189) As Byte
    CodingHistory() As String           ' history
End Type

' BASS_ChannelGetLength/GetPosition/SetPosition modes
Global Const BASS_POS_BYTE = 0          ' byte position
Global Const BASS_POS_MUSIC_ORDER = 1   ' order.row position, MAKELONG(order,row)
Global Const BASS_POS_OGG = 3           ' OGG bitstream number
Global Const BASS_POS_RESET = &H2000000 ' flag: reset user file buffers
Global Const BASS_POS_RELATIVE = &H4000000 ' flag: seek relative to the current position
Global Const BASS_POS_INEXACT = &H8000000 ' flag: allow seeking to inexact position
Global Const BASS_POS_DECODE = &H10000000 ' flag: get the decoding (not playing) position
Global Const BASS_POS_DECODETO = &H20000000 ' flag: decode to the position instead of seeking
Global Const BASS_POS_SCAN = &H40000000 ' flag: scan to the position

' BASS_ChannelSetDevice/GetDevice option
Global Const BASS_NODEVICE = &H20000

' BASS_RecordSetInput flags
Global Const BASS_INPUT_OFF = &H10000
Global Const BASS_INPUT_ON = &H20000

Global Const BASS_INPUT_TYPE_MASK = &HFF000000
Global Const BASS_INPUT_TYPE_UNDEF = &H0
Global Const BASS_INPUT_TYPE_DIGITAL = &H1000000
Global Const BASS_INPUT_TYPE_LINE = &H2000000
Global Const BASS_INPUT_TYPE_MIC = &H3000000
Global Const BASS_INPUT_TYPE_SYNTH = &H4000000
Global Const BASS_INPUT_TYPE_CD = &H5000000
Global Const BASS_INPUT_TYPE_PHONE = &H6000000
Global Const BASS_INPUT_TYPE_SPEAKER = &H7000000
Global Const BASS_INPUT_TYPE_WAVE = &H8000000
Global Const BASS_INPUT_TYPE_AUX = &H9000000
Global Const BASS_INPUT_TYPE_ANALOG = &HA000000

' BASS_ChannelSetFX effect types
Global Const BASS_FX_DX8_CHORUS = 0
Global Const BASS_FX_DX8_COMPRESSOR = 1
Global Const BASS_FX_DX8_DISTORTION = 2
Global Const BASS_FX_DX8_ECHO = 3
Global Const BASS_FX_DX8_FLANGER = 4
Global Const BASS_FX_DX8_GARGLE = 5
Global Const BASS_FX_DX8_I3DL2REVERB = 6
Global Const BASS_FX_DX8_PARAMEQ = 7
Global Const BASS_FX_DX8_REVERB = 8
Global Const BASS_FX_VOLUME = 9

Type BASS_DX8_CHORUS
    fWetDryMix As Single
    fDepth As Single
    fFeedback As Single
    fFrequency As Single
    lWaveform As Long   ' 0=triangle, 1=sine
    fDelay As Single
    lPhase As Long              ' BASS_DX8_PHASE_xxx
End Type

Type BASS_DX8_COMPRESSOR
    fGain As Single
    fAttack As Single
    fRelease As Single
    fThreshold As Single
    fRatio As Single
    fPredelay As Single
End Type

Type BASS_DX8_DISTORTION
    fGain As Single
    fEdge As Single
    fPostEQCenterFrequency As Single
    fPostEQBandwidth As Single
    fPreLowpassCutoff As Single
End Type

Type BASS_DX8_ECHO
    fWetDryMix As Single
    fFeedback As Single
    fLeftDelay As Single
    fRightDelay As Single
    lPanDelay As Long
End Type

Type BASS_DX8_FLANGER
    fWetDryMix As Single
    fDepth As Single
    fFeedback As Single
    fFrequency As Single
    lWaveform As Long   ' 0=triangle, 1=sine
    fDelay As Single
    lPhase As Long              ' BASS_DX8_PHASE_xxx
End Type

Type BASS_DX8_GARGLE
    dwRateHz As Long               ' Rate of modulation in hz
    dwWaveShape As Long            ' 0=triangle, 1=square
End Type

Type BASS_DX8_I3DL2REVERB
    lRoom As Long                    ' [-10000, 0]      default: -1000 mB
    lRoomHF As Long                  ' [-10000, 0]      default: 0 mB
    flRoomRolloffFactor As Single    ' [0.0, 10.0]      default: 0.0
    flDecayTime As Single            ' [0.1, 20.0]      default: 1.49s
    flDecayHFRatio As Single         ' [0.1, 2.0]       default: 0.83
    lReflections As Long             ' [-10000, 1000]   default: -2602 mB
    flReflectionsDelay As Single     ' [0.0, 0.3]       default: 0.007 s
    lReverb As Long                  ' [-10000, 2000]   default: 200 mB
    flReverbDelay As Single          ' [0.0, 0.1]       default: 0.011 s
    flDiffusion As Single            ' [0.0, 100.0]     default: 100.0 %
    flDensity As Single              ' [0.0, 100.0]     default: 100.0 %
    flHFReference As Single          ' [20.0, 20000.0]  default: 5000.0 Hz
End Type

Type BASS_DX8_PARAMEQ
    fCenter As Single
    fBandwidth As Single
    fGain As Single
End Type

Type BASS_DX8_REVERB
    fInGain As Single                ' [-96.0,0.0]            default: 0.0 dB
    fReverbMix As Single             ' [-96.0,0.0]            default: 0.0 db
    fReverbTime As Single            ' [0.001,3000.0]         default: 1000.0 ms
    fHighFreqRTRatio As Single       ' [0.001,0.999]          default: 0.001
End Type

Global Const BASS_DX8_PHASE_NEG_180 = 0
Global Const BASS_DX8_PHASE_NEG_90 = 1
Global Const BASS_DX8_PHASE_ZERO = 2
Global Const BASS_DX8_PHASE_90 = 3
Global Const BASS_DX8_PHASE_180 = 4

Type BASS_FX_VOLUME_PARAM
    fTarget As Single
    fCurrent As Single
    fTime As Single
    lCurve As Long
End Type

Type GUID       ' used with BASS_Init - use VarPtr(guid) in clsid parameter
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(0 To 7) As Byte
End Type


Declare Function BASS_SetConfig Lib "bass.dll" (ByVal opt As Long, ByVal value As Long) As Long
Declare Function BASS_GetConfig Lib "bass.dll" (ByVal opt As Long) As Long
Declare Function BASS_SetConfigPtr Lib "bass.dll" (ByVal opt As Long, ByVal value As Any) As Long
Declare Function BASS_GetConfigPtr Lib "bass.dll" (ByVal opt As Long) As Long
Declare Function BASS_GetVersion Lib "bass.dll" () As Long
Declare Function BASS_ErrorGetCode Lib "bass.dll" () As Long
Declare Function BASS_GetDeviceInfo Lib "bass.dll" (ByVal device As Long, ByRef info As BASS_DEVICEINFO) As Long
Declare Function BASS_Init Lib "bass.dll" (ByVal device As Long, ByVal freq As Long, ByVal flags As Long, ByVal win As Long, ByVal clsid As Long) As Long
Declare Function BASS_SetDevice Lib "bass.dll" (ByVal device As Long) As Long
Declare Function BASS_GetDevice Lib "bass.dll" () As Long
Declare Function BASS_Free Lib "bass.dll" () As Long
Declare Function BASS_GetDSoundObject Lib "bass.dll" (ByVal object As Long) As Long
Declare Function BASS_GetInfo Lib "bass.dll" (ByRef info As BASS_INFO) As Long
Declare Function BASS_Update Lib "bass.dll" (ByVal legnth As Long) As Long
Declare Function BASS_GetCPU Lib "bass.dll" () As Single
Declare Function BASS_Start Lib "bass.dll" () As Long
Declare Function BASS_Stop Lib "bass.dll" () As Long
Declare Function BASS_Pause Lib "bass.dll" () As Long
Declare Function BASS_IsStarted Lib "bass.dll" () As Long
Declare Function BASS_SetVolume Lib "bass.dll" (ByVal volume As Single) As Long
Declare Function BASS_GetVolume Lib "bass.dll" () As Single

Declare Function BASS_PluginLoad Lib "bass.dll" (ByVal filename As String, ByVal flags As Long) As Long
Declare Function BASS_PluginFree Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_PluginGetInfo_ Lib "bass.dll" Alias "BASS_PluginGetInfo" (ByVal handle As Long) As Long

Declare Function BASS_Set3DFactors Lib "bass.dll" (ByVal distf As Single, ByVal rollf As Single, ByVal doppf As Single) As Long
Declare Function BASS_Get3DFactors Lib "bass.dll" (ByRef distf As Single, ByRef rollf As Single, ByRef doppf As Single) As Long
Declare Function BASS_Set3DPosition Lib "bass.dll" (ByRef pos As Any, ByRef vel As Any, ByRef front As Any, ByRef top As Any) As Long
Declare Function BASS_Get3DPosition Lib "bass.dll" (ByRef pos As Any, ByRef vel As Any, ByRef front As Any, ByRef top As Any) As Long
Declare Function BASS_Apply3D Lib "bass.dll" () As Long
Declare Function BASS_SetEAXParameters Lib "bass.dll" (ByVal env As Long, ByVal vol As Single, ByVal decay As Single, ByVal damp As Single) As Long
Declare Function BASS_GetEAXParameters Lib "bass.dll" (ByRef env As Long, ByRef vol As Single, ByRef decay As Single, ByRef damp As Single) As Long

Declare Function BASS_MusicLoad64 Lib "bass.dll" Alias "BASS_MusicLoad" (ByVal mem As Long, ByVal file As Any, ByVal offset As Long, ByVal offsethigh As Long, ByVal length As Long, ByVal flags As Long, ByVal freq As Long) As Long
Declare Function BASS_MusicFree Lib "bass.dll" (ByVal handle As Long) As Long

Declare Function BASS_SampleLoad64 Lib "bass.dll" Alias "BASS_SampleLoad" (ByVal mem As Long, ByVal file As Any, ByVal offset As Long, ByVal offsethigh As Long, ByVal length As Long, ByVal max As Long, ByVal flags As Long) As Long
Declare Function BASS_SampleCreate Lib "bass.dll" (ByVal length As Long, ByVal freq As Long, ByVal chans As Long, ByVal max As Long, ByVal flags As Long) As Long
Declare Function BASS_SampleFree Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_SampleSetData Lib "bass.dll" (ByVal handle As Long, ByRef buffer As Any) As Long
Declare Function BASS_SampleGetData Lib "bass.dll" (ByVal handle As Long, ByRef buffer As Any) As Long
Declare Function BASS_SampleGetInfo Lib "bass.dll" (ByVal handle As Long, ByRef info As BASS_SAMPLE) As Long
Declare Function BASS_SampleSetInfo Lib "bass.dll" (ByVal handle As Long, ByRef info As BASS_SAMPLE) As Long
Declare Function BASS_SampleGetChannel Lib "bass.dll" (ByVal handle As Long, ByVal onlynew As Long) As Long
Declare Function BASS_SampleGetChannels Lib "bass.dll" (ByVal handle As Long, ByRef channels As Long) As Long
Declare Function BASS_SampleStop Lib "bass.dll" (ByVal handle As Long) As Long

Declare Function BASS_StreamCreate Lib "bass.dll" (ByVal freq As Long, ByVal chans As Long, ByVal flags As Long, ByVal proc As Long, ByVal user As Long) As Long
Declare Function BASS_StreamCreateFile64 Lib "bass.dll" Alias "BASS_StreamCreateFile" (ByVal mem As Long, ByVal file As Any, ByVal offset As Long, ByVal offsethigh As Long, ByVal length As Long, ByVal lengthhigh As Long, ByVal flags As Long) As Long
Declare Function BASS_StreamCreateURL Lib "bass.dll" (ByVal url As String, ByVal offset As Long, ByVal flags As Long, ByVal proc As Long, ByVal user As Long) As Long
Declare Function BASS_StreamCreateFileUser Lib "bass.dll" (ByVal system As Long, ByVal flags As Long, ByVal procs As Long, ByVal user As Long) As Long
Declare Function BASS_StreamFree Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_StreamGetFilePosition Lib "bass.dll" (ByVal handle As Long, ByVal mode As Long) As Long
Declare Function BASS_StreamPutData Lib "bass.dll" (ByVal handle As Long, ByRef buffer As Any, ByVal length As Long) As Long
Declare Function BASS_StreamPutFileData Lib "bass.dll" (ByVal handle As Long, ByRef buffer As Any, ByVal length As Long) As Long

Declare Function BASS_RecordGetDeviceInfo Lib "bass.dll" (ByVal device As Long, ByRef info As BASS_DEVICEINFO) As Long
Declare Function BASS_RecordInit Lib "bass.dll" (ByVal device As Long) As Long
Declare Function BASS_RecordSetDevice Lib "bass.dll" (ByVal device As Long) As Long
Declare Function BASS_RecordGetDevice Lib "bass.dll" () As Long
Declare Function BASS_RecordFree Lib "bass.dll" () As Long
Declare Function BASS_RecordGetInfo Lib "bass.dll" (ByRef info As BASS_RECORDINFO) As Long
Declare Function BASS_RecordGetInputName Lib "bass.dll" (ByVal inputn As Long) As Long
Declare Function BASS_RecordSetInput Lib "bass.dll" (ByVal inputn As Long, ByVal flags As Long, ByVal volume As Single) As Long
Declare Function BASS_RecordGetInput Lib "bass.dll" (ByVal inputn As Long, ByRef volume As Single) As Long
Declare Function BASS_RecordStart Lib "bass.dll" (ByVal freq As Long, ByVal chans As Long, ByVal flags As Long, ByVal proc As Long, ByVal user As Long) As Long

Declare Function BASS_ChannelBytes2Seconds64 Lib "bass.dll" Alias "BASS_ChannelBytes2Seconds" (ByVal handle As Long, ByVal pos As Long, ByVal poshigh As Long) As Double
Declare Function BASS_ChannelSeconds2Bytes Lib "bass.dll" (ByVal handle As Long, ByVal pos As Double) As Long
Declare Function BASS_ChannelGetDevice Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_ChannelSetDevice Lib "bass.dll" (ByVal handle As Long, ByVal device As Long) As Long
Declare Function BASS_ChannelIsActive Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_ChannelGetInfo Lib "bass.dll" (ByVal handle As Long, ByRef info As BASS_CHANNELINFO) As Long
Declare Function BASS_ChannelGetTags Lib "bass.dll" (ByVal handle As Long, ByVal tags As Long) As Long
Declare Function BASS_ChannelFlags Lib "bass.dll" (ByVal handle As Long, ByVal flags As Long, ByVal mask As Long) As Long
Declare Function BASS_ChannelUpdate Lib "bass.dll" (ByVal handle As Long, ByVal length As Long) As Long
Declare Function BASS_ChannelLock Lib "bass.dll" (ByVal handle As Long, ByVal lock_ As Long) As Long
Declare Function BASS_ChannelPlay Lib "bass.dll" (ByVal handle As Long, ByVal restart As Long) As Long
Declare Function BASS_ChannelStop Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_ChannelPause Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_ChannelSetAttribute Lib "bass.dll" (ByVal handle As Long, ByVal attrib As Long, ByVal value As Single) As Long
Declare Function BASS_ChannelGetAttribute Lib "bass.dll" (ByVal handle As Long, ByVal attrib As Long, ByRef value As Single) As Long
Declare Function BASS_ChannelSlideAttribute Lib "bass.dll" (ByVal handle As Long, ByVal attrib As Long, ByVal value As Single, ByVal time As Long) As Long
Declare Function BASS_ChannelIsSliding Lib "bass.dll" (ByVal handle As Long, ByVal attrib As Long) As Long
Declare Function BASS_ChannelSetAttributeEx Lib "bass.dll" (ByVal handle As Long, ByVal attrib As Long, ByRef value As Any, ByVal size As Long) As Long
Declare Function BASS_ChannelGetAttributeEx Lib "bass.dll" (ByVal handle As Long, ByVal attrib As Long, ByRef value As Any, ByVal size As Long) As Long
Declare Function BASS_ChannelSet3DAttributes Lib "bass.dll" (ByVal handle As Long, ByVal mode As Long, ByVal min As Single, ByVal max As Single, ByVal iangle As Long, ByVal oangle As Long, ByVal outvol As Single) As Long
Declare Function BASS_ChannelGet3DAttributes Lib "bass.dll" (ByVal handle As Long, ByRef mode As Long, ByRef min As Single, ByRef max As Single, ByRef iangle As Long, ByRef oangle As Long, ByRef outvol As Single) As Long
Declare Function BASS_ChannelSet3DPosition Lib "bass.dll" (ByVal handle As Long, ByRef pos As Any, ByRef orient As Any, ByRef vel As Any) As Long
Declare Function BASS_ChannelGet3DPosition Lib "bass.dll" (ByVal handle As Long, ByRef pos As Any, ByRef orient As Any, ByRef vel As Any) As Long
Declare Function BASS_ChannelGetLength Lib "bass.dll" (ByVal handle As Long, ByVal mode As Long) As Long
Declare Function BASS_ChannelSetPosition64 Lib "bass.dll" Alias "BASS_ChannelSetPosition" (ByVal handle As Long, ByVal pos As Long, ByVal poshigh As Long, ByVal mode As Long) As Long
Declare Function BASS_ChannelGetPosition Lib "bass.dll" (ByVal handle As Long, ByVal mode As Long) As Long
Declare Function BASS_ChannelGetLevel Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_ChannelGetLevelEx Lib "bass.dll" (ByVal handle As Long, ByRef levels As Single, ByVal length As Single, ByVal flags As Long) As Long
Declare Function BASS_ChannelGetData Lib "bass.dll" (ByVal handle As Long, ByRef buffer As Any, ByVal length As Long) As Long
Declare Function BASS_ChannelSetSync64 Lib "bass.dll" Alias "BASS_ChannelSetSync" (ByVal handle As Long, ByVal type_ As Long, ByVal param As Long, ByVal paramhigh As Long, ByVal proc As Long, ByVal user As Long) As Long
Declare Function BASS_ChannelRemoveSync Lib "bass.dll" (ByVal handle As Long, ByVal sync As Long) As Long
Declare Function BASS_ChannelSetDSP Lib "bass.dll" (ByVal handle As Long, ByVal proc As Long, ByVal user As Long, ByVal priority As Long) As Long
Declare Function BASS_ChannelRemoveDSP Lib "bass.dll" (ByVal handle As Long, ByVal dsp As Long) As Long
Declare Function BASS_ChannelSetLink Lib "bass.dll" (ByVal handle As Long, ByVal chan As Long) As Long
Declare Function BASS_ChannelRemoveLink Lib "bass.dll" (ByVal handle As Long, ByVal chan As Long) As Long
Declare Function BASS_ChannelSetFX Lib "bass.dll" (ByVal handle As Long, ByVal type_ As Long, ByVal priority As Long) As Long
Declare Function BASS_ChannelRemoveFX Lib "bass.dll" (ByVal handle As Long, ByVal fx As Long) As Long
Declare Function BASS_FXSetParameters Lib "bass.dll" (ByVal handle As Long, ByRef par As Any) As Long
Declare Function BASS_FXGetParameters Lib "bass.dll" (ByVal handle As Long, ByRef par As Any) As Long
Declare Function BASS_FXReset Lib "bass.dll" (ByVal handle As Long) As Long
Declare Function BASS_FXSetPriority Lib "bass.dll" (ByVal handle As Long, ByVal priority As Long) As Long

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef Destination As Any, ByRef Source As Any, ByVal length As Long)
Private Declare Function lstrlen Lib "kernel32" Alias "lstrlenA" (ByVal lpString As Long) As Long

Public Function BASS_SPEAKER_N(ByVal n As Long) As Long
BASS_SPEAKER_N = n * (2 ^ 24)
End Function

' 32-bit wrappers for 64-bit BASS functions
Function BASS_MusicLoad(ByVal mem As Long, ByVal file As Long, ByVal offset As Long, ByVal length As Long, ByVal flags As Long, ByVal freq As Long) As Long
BASS_MusicLoad = BASS_MusicLoad64(mem, file, offset, 0, length, flags Or BASS_UNICODE, freq)
End Function

Function BASS_SampleLoad(ByVal mem As Long, ByVal file As Long, ByVal offset As Long, ByVal length As Long, ByVal max As Long, ByVal flags As Long) As Long
BASS_SampleLoad = BASS_SampleLoad64(mem, file, offset, 0, length, max, flags Or BASS_UNICODE)
End Function

Function BASS_StreamCreateFile(ByVal mem As Long, ByVal file As Long, ByVal offset As Long, ByVal length As Long, ByVal flags As Long) As Long
BASS_StreamCreateFile = BASS_StreamCreateFile64(mem, file, offset, 0, length, 0, flags Or BASS_UNICODE)
End Function

Function BASS_ChannelBytes2Seconds(ByVal handle As Long, ByVal pos As Long) As Double
BASS_ChannelBytes2Seconds = BASS_ChannelBytes2Seconds64(handle, pos, 0)
End Function

Function BASS_ChannelSetPosition(ByVal handle As Long, ByVal pos As Long, ByVal mode As Long) As Long
BASS_ChannelSetPosition = BASS_ChannelSetPosition64(handle, pos, 0, mode)
End Function

Function BASS_ChannelSetSync(ByVal handle As Long, ByVal type_ As Long, ByVal param As Long, ByVal proc As Long, ByVal user As Long) As Long
BASS_ChannelSetSync = BASS_ChannelSetSync64(handle, type_, param, 0, proc, user)
End Function

' BASS_PluginGetInfo wrappers
Function BASS_PluginGetInfo(ByVal handle As Long) As BASS_PLUGININFO
Dim pinfo As BASS_PLUGININFO, plug As Long
plug = BASS_PluginGetInfo_(handle)
If plug Then
    Call CopyMemory(pinfo, ByVal plug, LenB(pinfo))
End If
BASS_PluginGetInfo = pinfo
End Function

Function BASS_PluginGetInfoFormat(ByVal handle As Long, ByVal index As Long) As BASS_PLUGINFORM
Dim pform As BASS_PLUGINFORM, plug As Long
plug = BASS_PluginGetInfo(handle).formats
If plug Then
    plug = plug + (index * LenB(pform))
    Call CopyMemory(pform, ByVal plug, LenB(pform))
End If
BASS_PluginGetInfoFormat = pform
End Function

' callback functions
Function STREAMPROC(ByVal handle As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long) As Long
    
    'CALLBACK FUNCTION !!!
    
    ' User stream callback function
    ' NOTE: A stream function should obviously be as quick
    ' as possible, other streams (and MOD musics) can't be mixed until it's finished.
    ' handle : The stream that needs writing
    ' buffer : Buffer to write the samples in
    ' length : Number of bytes to write
    ' user   : The 'user' parameter value given when calling BASS_StreamCreate
    ' RETURN : Number of bytes written. Set the BASS_STREAMPROC_END flag to end
    '          the stream.
    
End Function

Sub DOWNLOADPROC(ByVal buffer As Long, ByVal length As Long, ByVal user As Long)
    
    'CALLBACK FUNCTION !!!

    ' Internet stream download callback function.
    ' buffer : Buffer containing the downloaded data... NULL=end of download
    ' length : Number of bytes in the buffer
    ' user   : The 'user' parameter given when calling BASS_StreamCreateURL
    
End Sub

Sub SYNCPROC(ByVal handle As Long, ByVal channel As Long, ByVal data As Long, ByVal user As Long)
    
    'CALLBACK FUNCTION !!!
    
    'Similarly in here, write what to do when sync function
    'is called, i.e screen flash etc.
    
    ' NOTE: a sync callback function should be very quick as other
    ' syncs cannot be processed until it has finished.
    ' handle : The sync that has occured
    ' channel: Channel that the sync occured in
    ' data   : Additional data associated with the sync's occurance
    ' user   : The 'user' parameter given when calling BASS_ChannelSetSync */
    
End Sub

Sub DSPPROC(ByVal handle As Long, ByVal channel As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long)

    'CALLBACK FUNCTION !!!

    ' VB doesn't support pointers, so you should copy the buffer into an array,
    ' process it, and then copy it back into the buffer.

    ' DSP callback function. NOTE: A DSP function should obviously be as quick as
    ' possible... other DSP functions, streams and MOD musics can not be processed
    ' until it's finished.
    ' handle : The DSP handle
    ' channel: Channel that the DSP is being applied to
    ' buffer : Buffer to apply the DSP to
    ' length : Number of bytes in the buffer
    ' user   : The 'user' parameter given when calling BASS_ChannelSetDSP
    
End Sub

Function RECORDPROC(ByVal handle As Long, ByVal buffer As Long, ByVal length As Long, ByVal user As Long) As Long

    'CALLBACK FUNCTION !!!

    ' Recording callback function.
    ' handle : The recording handle
    ' buffer : Buffer containing the recorded samples
    ' length : Number of bytes
    ' user   : The 'user' parameter value given when calling BASS_RecordStart
    ' RETURN : BASSTRUE = continue recording, BASSFALSE = stop

End Function

' User file stream callback functions (BASS_FILEPROCS)
Sub FILECLOSEPROC(ByVal user As Long)

End Sub

Function FILELENPROC(ByVal user As Long) As Currency ' ???

End Function

Function FILEREADPROC(ByVal buffer As Long, ByVal length As Long, ByVal user As Long) As Long

End Function

Function FILESEEKPROC(ByVal offset As Long, ByVal offsethigh As Long, ByVal user As Long) As Long

End Function


Function BASS_SetEAXPreset(preset) As Long
' This function is a workaround, because VB doesn't support multiple comma seperated
' paramaters for each Global Const, simply pass the EAX_ENVIRONMENT_xxx value to this function
' instead of BASS_SetEAXParameters as you would do in C/C++
Select Case preset
    Case EAX_ENVIRONMENT_GENERIC
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_GENERIC, 0.5, 1.493, 0.5)
    Case EAX_ENVIRONMENT_PADDEDCELL
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_PADDEDCELL, 0.25, 0.1, 0)
    Case EAX_ENVIRONMENT_ROOM
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_ROOM, 0.417, 0.4, 0.666)
    Case EAX_ENVIRONMENT_BATHROOM
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_BATHROOM, 0.653, 1.499, 0.166)
    Case EAX_ENVIRONMENT_LIVINGROOM
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_LIVINGROOM, 0.208, 0.478, 0)
    Case EAX_ENVIRONMENT_STONEROOM
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_STONEROOM, 0.5, 2.309, 0.888)
    Case EAX_ENVIRONMENT_AUDITORIUM
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_AUDITORIUM, 0.403, 4.279, 0.5)
    Case EAX_ENVIRONMENT_CONCERTHALL
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_CONCERTHALL, 0.5, 3.961, 0.5)
    Case EAX_ENVIRONMENT_CAVE
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_CAVE, 0.5, 2.886, 1.304)
    Case EAX_ENVIRONMENT_ARENA
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_ARENA, 0.361, 7.284, 0.332)
    Case EAX_ENVIRONMENT_HANGAR
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_HANGAR, 0.5, 10, 0.3)
    Case EAX_ENVIRONMENT_CARPETEDHALLWAY
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_CARPETEDHALLWAY, 0.153, 0.259, 2)
    Case EAX_ENVIRONMENT_HALLWAY
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_HALLWAY, 0.361, 1.493, 0)
    Case EAX_ENVIRONMENT_STONECORRIDOR
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_STONECORRIDOR, 0.444, 2.697, 0.638)
    Case EAX_ENVIRONMENT_ALLEY
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_ALLEY, 0.25, 1.752, 0.776)
    Case EAX_ENVIRONMENT_FOREST
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_FOREST, 0.111, 3.145, 0.472)
    Case EAX_ENVIRONMENT_CITY
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_CITY, 0.111, 2.767, 0.224)
    Case EAX_ENVIRONMENT_MOUNTAINS
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_MOUNTAINS, 0.194, 7.841, 0.472)
    Case EAX_ENVIRONMENT_QUARRY
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_QUARRY, 1, 1.499, 0.5)
    Case EAX_ENVIRONMENT_PLAIN
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_PLAIN, 0.097, 2.767, 0.224)
    Case EAX_ENVIRONMENT_PARKINGLOT
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_PARKINGLOT, 0.208, 1.652, 1.5)
    Case EAX_ENVIRONMENT_SEWERPIPE
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_SEWERPIPE, 0.652, 2.886, 0.25)
    Case EAX_ENVIRONMENT_UNDERWATER
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_UNDERWATER, 1, 1.499, 0)
    Case EAX_ENVIRONMENT_DRUGGED
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_DRUGGED, 0.875, 8.392, 1.388)
    Case EAX_ENVIRONMENT_DIZZY
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_DIZZY, 0.139, 17.234, 0.666)
    Case EAX_ENVIRONMENT_PSYCHOTIC
        BASS_SetEAXPreset = BASS_SetEAXParameters(EAX_ENVIRONMENT_PSYCHOTIC, 0.486, 7.563, 0.806)
End Select
End Function

Public Function LoByte(ByVal lparam As Long) As Long
LoByte = lparam And &HFF&
End Function
Public Function HiByte(ByVal lparam As Long) As Long
HiByte = (lparam And &HFF00&) / &H100&
End Function
Public Function LoWord(ByVal lparam As Long) As Long
LoWord = lparam And &HFFFF&
End Function
Public Function HiWord(ByVal lparam As Long) As Long
If lparam < 0 Then
    HiWord = (lparam \ &H10000 - 1) And &HFFFF&
Else
    HiWord = lparam \ &H10000
End If
End Function
Function MakeWord(ByVal LoByte As Long, ByVal HiByte As Long) As Long
MakeWord = (LoByte And &HFF&) Or ((HiByte And &HFF&) * &H100&)
End Function
Function MakeLong(ByVal LoWord As Long, ByVal HiWord As Long) As Long
MakeLong = LoWord And &HFFFF&
HiWord = HiWord And &HFFFF&
If HiWord And &H8000& Then
    MakeLong = MakeLong Or (((HiWord And &H7FFF&) * &H10000) Or &H80000000)
Else
    MakeLong = MakeLong Or (HiWord * &H10000)
End If
End Function

Public Function VBStrFromAnsiPtr(ByVal lpStr As Long) As String
Dim bStr() As Byte
Dim cChars As Long
On Error Resume Next
' Get the number of characters in the buffer
cChars = lstrlen(lpStr)
If cChars Then
    ' Resize the byte array
    ReDim bStr(0 To cChars - 1) As Byte
    ' Grab the ANSI buffer
    Call CopyMemory(bStr(0), ByVal lpStr, cChars)
End If
' Now convert to a VB Unicode string
VBStrFromAnsiPtr = StrConv(bStr, vbUnicode)
End Function
