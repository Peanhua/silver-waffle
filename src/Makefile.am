AM_CXXFLAGS =                \
        $(SDL2_CFLAGS)       \
        $(SDL2_image_CFLAGS) \
        $(GL_CFLAGS)         \
        $(GLEW_CFLAGS)       \
        $(GLM_CFLAGS)        \
	$(ASSIMP_CFLAGS)     \
	$(JSON11_CFLAGS)

bin_PROGRAMS = silverwaffle

silverwaffle_LDADD =       \
        $(SDL2_LIBS)       \
        $(SDL2_image_LIBS) \
        $(GL_LIBS)         \
        $(GLEW_LIBS)       \
	$(ASSIMP_LIBS)     \
	$(JSON11_LIBS)

silverwaffle_SOURCES =		\
	Camera.cc		\
	Camera2.cc		\
	Explosion.cc		\
	GameState.cc		\
	GameStateGame.cc	\
	GameStateTitle.cc	\
	Image.cc		\
	Mesh.cc			\
	Mesh_LoadFromFile.cc	\
	MeshOverlay.cc		\
	Object.cc		\
	ObjectInvader.cc	\
	ObjectMovable.cc	\
	ObjectProjectile.cc	\
	ObjectSpaceship.cc	\
	Scene.cc		\
	ScoreReel.cc		\
	Shader.cc		\
	ShaderProgram.cc	\
	Subsystem.cc		\
	SubsystemAssetLoader.cc	\
	SubsystemGameState.cc	\
	SubsystemGfx.cc		\
	SubsystemHandler.cc	\
	SubsystemInput.cc	\
	Widget.cc		\
	glm.cc			\
	main.cc
