AC_INIT([silver-waffle], [0.1])
AC_PREREQ([2.69])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign dist-xz subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIRS([m4])

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([20], [noext], [mandatory])

AX_PTHREAD

PKG_CHECK_MODULES([SDL2], [sdl2 >= 2], [], [
  PKG_CHECK_MODULES([SDL2], [sdl >= 2])]
)


PKG_CHECK_MODULES([GLEW], [glew], [],
  [AC_CHECK_LIB([GLEW], [glewInit], [],
    [AC_MSG_ERROR([unable to find GLEW library])] )]
)

PKG_CHECK_MODULES([GLM], [glm], [], [
  AC_LANG_PUSH([C++])
  AC_CHECK_HEADER([glm/glm.hpp], [], [AC_MSG_ERROR([Unable to find GLM library (http://glm.g-truc.net) headers.])])
  AC_LANG_POP
])

AC_LANG_PUSH([C++])
AC_CHECK_HEADER([format], [], [
  PKG_CHECK_MODULES([FMT], [fmt], [AC_DEFINE([HAVE_FMT], [1], [Use fmt])])
])

## https://github.com/dmilos/color
AC_CHECK_HEADER([color/color.hpp], [], [AC_MSG_ERROR([Unable to find color library (https://github.com/dmilos/color) headers.])])

AC_LANG_POP



PKG_CHECK_MODULES([ALUT],       [freealut])
PKG_CHECK_MODULES([ASSIMP],     [assimp >= 3.3.1])
PKG_CHECK_MODULES([FREETYPE2],  [freetype2])
PKG_CHECK_MODULES([JSON11],     [json11 >= 1])
PKG_CHECK_MODULES([LIBPNG],     [libpng])
PKG_CHECK_MODULES([OPENAL],     [openal])
PKG_CHECK_MODULES([SDL2_IMAGE], [SDL2_image >= 2])
PKG_CHECK_MODULES([SIDPLAYFP],  [libsidplayfp])
PKG_CHECK_MODULES([LIBFMSYNTH], [libfmsynth >= 0.2])


AC_ARG_WITH([valgrind], AS_HELP_STRING([--with-valgrind], [Build with support for Valgrind.]))
AS_IF([test "x$with_valgrind" = "xyes"], [
  PKG_CHECK_MODULES([VALGRIND], [valgrind])
  AC_DEFINE([WITH_VALGRIND], [1], [Define to use Valgrind.])
])

AX_CHECK_GL

AC_ARG_VAR([CONVERT], [Path to the convert program from ImageMagick or alternative.])
AC_CHECK_PROG([CONVERT], [convert], [convert])
if test "x$CONVERT" == "x" ; then
  AC_MSG_ERROR(['convert' not found error, install ImageMagick or alternative.])
fi

DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([SilverWaffle])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile 3d-models/Makefile Data/Makefile Fonts/bitstream-vera-sans-mono-fonts/Makefile Images/Makefile Sounds/Makefile Shaders/Makefile Shaders/Include/Makefile Doxyfile])
AC_OUTPUT
