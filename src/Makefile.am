AM_CXXFLAGS =                \
        $(SDL2_CFLAGS)       \
        $(SDL2_image_CFLAGS) \
        $(GL_CFLAGS)         \
        $(GLEW_CFLAGS)       \
        $(GLM_CFLAGS)        \
	$(ASSIMP_CFLAGS)     \
	$(JSON11_CFLAGS)     \
	$(FREETYPE2_CFLAGS)

bin_PROGRAMS = silverwaffle

silverwaffle_LDADD =       \
        $(SDL2_LIBS)       \
        $(SDL2_image_LIBS) \
        $(GL_LIBS)         \
        $(GLEW_LIBS)       \
	$(ASSIMP_LIBS)     \
	$(JSON11_LIBS)     \
	$(FREETYPE2_LIBS)

silverwaffle_SOURCES =				\
	AdditiveBlending.cc			\
	Camera.cc				\
	Explosion.cc				\
	Font.cc					\
	GameState.cc				\
	GameStateGame.cc			\
	GameStateTitle.cc			\
	GaussianBlur.cc				\
	Image.cc				\
	Level.cc				\
	Mesh.cc					\
	Mesh_LoadFromFile.cc			\
	MeshOverlay.cc				\
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
	SolarSystemObject.cc			\
	SpaceParticles.cc			\
	SpaceshipUpgrade.cc			\
	Subsystem.cc				\
	SubsystemAssetLoader.cc			\
	SubsystemGameState.cc			\
	SubsystemGfx.cc				\
	SubsystemHandler.cc			\
	SubsystemInput.cc			\
	SubsystemSettings.cc			\
	TextureRenderer.cc			\
	UpgradeMaterial.cc			\
	Widget.cc				\
	WidgetButton.cc				\
	WidgetPlayerShip.cc			\
	WidgetShopItem.cc			\
	WidgetSpaceshipMaintenance.cc		\
	WidgetSpaceshipStatus.cc		\
	WidgetSpaceshipUpgradeStatus.cc		\
	WidgetWeaponStatus.cc			\
	WidgetUpgradeMaterial.cc		\
	WormholeWall.cc				\
	glm.cc					\
	main.cc
