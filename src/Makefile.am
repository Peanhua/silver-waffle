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
	BonusLevel.cc				\
	Camera.cc				\
	Explosion.cc				\
	Font.cc					\
	GameState.cc				\
	GameStateBonusLevel.cc			\
	GameStateGame.cc			\
	GameStateTitle.cc			\
	GameStats.cc				\
	GaussianBlur.cc				\
	HighscoreEntry.cc			\
	Image.cc				\
	Level.cc				\
	Loot.cc					\
	MainLevel.cc				\
	Mesh.cc					\
	Mesh_LoadFromFile.cc			\
	MeshOverlay.cc				\
	MeshProgressBar.cc			\
	Milkyway.cc				\
	Object.cc				\
	ObjectBonusLevelEntrance.cc		\
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
	SpaceshipControlProgram.cc		\
	SpaceshipUpgrade.cc			\
	Subsystem.cc				\
	SubsystemAssetLoader.cc			\
	SubsystemGameState.cc			\
	SubsystemGfx.cc				\
	SubsystemHandler.cc			\
	SubsystemHighscores.cc			\
	SubsystemInput.cc			\
	SubsystemSettings.cc			\
	TextureRenderer.cc			\
	UpgradeMaterial.cc			\
	Widget.cc				\
	WidgetButton.cc				\
	WidgetHighscores.cc			\
	WidgetPlayerShip.cc			\
	WidgetShopItem.cc			\
	WidgetSpaceshipMaintenance.cc		\
	WidgetSpaceshipStatus.cc		\
	WidgetSpaceshipUpgradeStatus.cc		\
	WidgetTeletyper.cc			\
	WidgetWeaponStatus.cc			\
	WidgetUpgradeMaterial.cc		\
	WormholeWall.cc				\
	glm.cc					\
	main.cc
