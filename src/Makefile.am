AM_CXXFLAGS =                \
        $(SDL2_CFLAGS)       \
        $(SDL2_image_CFLAGS) \
        $(GL_CFLAGS)         \
        $(GLEW_CFLAGS)       \
        $(GLM_CFLAGS)       

bin_PROGRAMS = silverwaffle

silverwaffle_LDADD =       \
        $(SDL2_LIBS)       \
        $(SDL2_image_LIBS) \
        $(GL_LIBS)         \
        $(GLEW_LIBS)       

silverwaffle_SOURCES = main.cc Object.cc GameLevel.cc glm.cc Mesh.cc MeshCube.cc Shader.cc ShaderProgram.cc OpenGL.cc Camera.cc
