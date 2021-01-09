VERSION = 53
REVISION = 1

.macro DATE
.ascii "6.3.2014"
.endm

.macro VERS
.ascii "UTF8.library 53.1"
.endm

.macro VSTRING
.ascii "UTF8.library 53.1 (6.3.2014)"
.byte 13,10,0
.endm

.macro VERSTAG
.byte 0
.ascii "$VER: UTF8.library 53.1 (6.3.2014)"
.byte 0
.endm
