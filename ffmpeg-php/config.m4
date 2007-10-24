PHP_ARG_WITH(ffmpeg,for ffmpeg support, 
[  --with-ffmpeg[=DIR]       Include ffmpeg support (requires ffmpeg >= 0.49.0).])

PHP_ARG_ENABLE(ffmpeg-force-gd, whether to force gd support in ffmpeg-php, [  --ffmpeg-force-gd     skip checks for gd libs and assume they are present.], no, no)

AC_DEFUN([FFMPEG_FORCE_GD_CK],[
    AC_DEFINE(HAVE_LIBGD20, 1, [ ])
])

if test "$PHP_FFMPEG_FORCE_GD" != "no"; then
    FFMPEG_FORCE_GD_CK
fi

dnl Determine path to ffmpeg libs
if test "$PHP_FFMPEG" != "no"; then

  AC_MSG_CHECKING(for ffmpeg headers)
  for i in $PHP_FFMPEG /usr/local /usr ; do
    if test -f $i/include/ffmpeg/avcodec.h; then
      FFMPEG_DIR=$i
      FFMPEG_INCDIR=$i/include/ffmpeg
      break
    elif test -f $i/include/avcodec.h; then
      FFMPEG_DIR=$i
      FFMPEG_INCDIR=$i/include
      break
    fi
  done

  if test -z "$FFMPEG_DIR"; then
    AC_MSG_ERROR(ffmpeg headers not found. Make sure you've built ffmpeg as shared libs using the --enable-shared option)
  else
    dnl For debugging
    AC_MSG_RESULT(...found in $FFMPEG_INCDIR)
  fi

  AC_MSG_CHECKING(for ffmpeg libavcodec.so)
  for i in $PHP_FFMPEG /usr/local /usr ; do
    if test -f $i/lib/libavcodec.so; then
      FFMPEG_LIBDIR=$i/lib
      break
    fi
    dnl PATCH: 1785450 x86_64 support (Bent Nagstrup Terp)
    if test -f $i/lib64/libavcodec.so; then
      FFMPEG_LIBDIR=$i/lib64
      break
    fi
    dnl MacOS-X support (Alexey Zakhlestin)
    if test -f $i/lib/libavcodec.dylib; then
      FFMPEG_LIBDIR=$i/lib
      break
    fi
    done

  if test -z "$FFMPEG_LIBDIR"; then
    AC_MSG_ERROR(ffmpeg share libraries not found. Make sure you've built ffmpeg as shared libs using the --enable-shared option)
  else
    dnl For debugging
    AC_MSG_RESULT(...found in $FFMPEG_LIBDIR)
  fi

  dnl FFMPEG_LIBDIR=$FFMPEG_DIR/lib

  CFLAGS="$CFLAGS -Wall -fno-strict-aliasing"

  PHP_ADD_LIBRARY_WITH_PATH(avcodec, $FFMPEG_LIBDIR, FFMPEG_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(avformat, $FFMPEG_LIBDIR, FFMPEG_SHARED_LIBADD)
  PHP_ADD_INCLUDE($FFMPEG_INCDIR)

  PHP_NEW_EXTENSION(ffmpeg, ffmpeg-php.c ffmpeg_movie.c ffmpeg_frame.c ffmpeg_animated_gif.c, $ext_shared,, \\$(GDLIB_CFLAGS))
  PHP_SUBST(FFMPEG_SHARED_LIBADD)
  AC_DEFINE(HAVE_FFMPEG_PHP,1,[ ])
    

dnl PHP_DEBUG_MACRO(test.dbg)
fi
