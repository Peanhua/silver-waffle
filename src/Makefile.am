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

silverwaffle_SOURCES =				\
	Camera.cc				\
	Explosion.cc				\
	GameState.cc				\
	GameStateGame.cc			\
	GameStateTitle.cc			\
	Image.cc				\
	Level.cc				\
	Mesh.cc					\
	Mesh_LoadFromFile.cc			\
	MeshProgressBar.cc			\
	Milkyway.cc				\
	Object.cc				\
	ObjectCollectible.cc			\
	ObjectInvader.cc			\
	ObjectMovable.cc			\
	ObjectPlanet.cc				\
	ObjectProjectile.cc			\
	ObjectSpaceship.cc			\
	Scene.cc				\
	ScoreReel.cc				\
	Shader.cc				\
	ShaderProgram.cc			\
	SpaceParticles.cc			\
	Subsystem.cc				\
	SubsystemAssetLoader.cc			\
	SubsystemGameState.cc			\
	SubsystemGfx.cc				\
	SubsystemHandler.cc			\
	SubsystemInput.cc			\
	SubsystemSettings.cc			\
	TextureRenderer.cc			\
	Widget.cc				\
	WidgetPlayerShip.cc			\
	WidgetSpaceshipStatus.cc		\
	WidgetWeaponStatus.cc			\
	WormholeWall.cc				\
	glm.cc					\
	main.cc
