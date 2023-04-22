#ifndef PROTO_UTF8_H
#define PROTO_UTF8_H

/*
**    File made whit idltool.
**
**    Copyright (c) 2014 Kjetil Hvalstrand
**	All Rights Reserved.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef LIBRARIES_UTF8_H
#include <libraries/UTF8.h>
#endif

/****************************************************************************/

#ifndef __NOLIBBASE__
 #if defined(__cplusplus) && defined(__USE_AMIGAOS_NAMESPACE__)
  extern struct AmigaOS::Library * UTF8Base;
 #else
  extern struct Library * UTF8Base;
 #endif
#endif /* __NOLIBBASE__ */

/****************************************************************************/

#ifdef __amigaos4__
 #include <interfaces/UTF8.h>
 #ifdef __USE_INLINE__
  #include <inline4/UTF8.h>
 #endif /* __USE_INLINE__ */
 #ifndef CLIB_UTF8_PROTOS_H
  #define CLIB_UTF8_PROTOS_H 1
 #endif /* CLIB_UTF8_PROTOS_H */
 #ifndef __NOGLOBALIFACE__
  #if defined(__cplusplus) && defined(__USE_AMIGAOS_NAMESPACE__)
   extern struct AmigaOS::UTF8IFace * IUTF8;
  #else
   extern struct UTF8IFace * IUTF8;
  #endif
 #endif /* __NOGLOBALIFACE__ */
#else /* __amigaos4__ */
 #ifndef CLIB_UTF8_PROTOS_H
  #include <clib/UTF8_protos.h>
 #endif /* CLIB_UTF8_PROTOS_H */
 #if defined(__GNUC__)
  #ifndef __PPC__
   #include <inline/UTF8.h>
  #else /* __PPC__ */
   #include <ppcinline/UTF8.h>
  #endif /* __PPC__ */
 #elif defined(__VBCC__)
  #ifndef __PPC__
   #include <inline/UTF8_protos.h>
  #endif /* __PPC__ */
 #else /* __GNUC__ */
  #include <pragmas/UTF8_pragmas.h>
 #endif /* __GNUC__ */
#endif /* __amigaos4__ */

/****************************************************************************/

#endif /* PROTO_UTF8_H */
