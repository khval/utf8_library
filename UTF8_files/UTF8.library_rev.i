VERSION		EQU	53
REVISION	EQU	1

DATE	MACRO
		dc.b '6.3.2014'
		ENDM

VERS	MACRO
		dc.b 'UTF8.library 53.1'
		ENDM

VSTRING	MACRO
		dc.b 'UTF8.library 53.1 (6.3.2014)',13,10,0
		ENDM

VERSTAG	MACRO
		dc.b 0,'$VER: UTF8.library 53.1 (6.3.2014)',0
		ENDM
