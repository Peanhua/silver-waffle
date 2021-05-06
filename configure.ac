AC_INIT([silver-waffle], [0.1])
AC_PREREQ([2.69])
AM_INIT_AUTOMAKE([foreign dist-xz subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([17], [noext], [mandatory])

PKG_CHECK_MODULES([SDL2], [sdl2 >= 2], [], [
  PKG_CHECK_MODULES([SDL2], [sdl >= 2])]
)

PKG_CHECK_MODULES([SDL2_image], [SDL2_image >= 2])

PKG_CHECK_MODULES([GLEW], [glew], [],
  [AC_CHECK_LIB([GLEW], [glewInit], [],
    [AC_MSG_ERROR([unable to find GLEW library])] )]
)

PKG_CHECK_MODULES([GLM], [glm])

PKG_CHECK_MODULES([ASSIMP], [assimp >= 3.3.1])

PKG_CHECK_MODULES([JSON11], [json11 >= 1])

AX_CHECK_GL


AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
