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
	Controller.cc				\
	CollisionShape.cc			\
	CollisionShapeOBB.cc			\
	CollisionShapeSphere.cc			\
	ControllerPlanet.cc			\
	ControllerSpace.cc			\
	Explosion.cc				\
	Font.cc					\
	GameStats.cc				\
	GaussianBlur.cc				\
	HighscoreEntry.cc			\
	Image.cc				\
	Level.cc				\
	LevelBonus.cc				\
	LevelMain.cc				\
	LevelPlanet.cc				\
	Loot.cc					\
	Mesh.cc					\
	MeshOverlay.cc				\
	MeshProgressBar.cc			\
	Mesh_LoadFromFile.cc			\
	Milkyway.cc				\
	Object.cc				\
	ObjectBonusLevelEntrance.cc		\
	ObjectCollectible.cc			\
	ObjectInvader.cc			\
	ObjectMovable.cc			\
	ObjectPlanet.cc				\
	ObjectPlanetAtmosphere.cc		\
	ObjectPlanetGround.cc			\
	ObjectProjectile.cc			\
	ObjectSpaceship.cc			\
	Scene.cc				\
	ScenePlanet.cc				\
	SceneSpace.cc				\
	ScoreReel.cc				\
	Screen.cc				\
	ScreenLevel.cc				\
	ScreenLevelBonus.cc			\
	ScreenLevelMain.cc			\
	ScreenLevelPlanet.cc			\
	ScreenTitle.cc				\
	Shader.cc				\
	ShaderProgram.cc			\
	SolarSystemObject.cc			\
	SpaceParticles.cc			\
	SpaceshipControlProgram.cc		\
	SpaceshipUpgrade.cc			\
	Subsystem.cc				\
	SubsystemAssetLoader.cc			\
	SubsystemGfx.cc				\
	SubsystemHandler.cc			\
	SubsystemHighscores.cc			\
	SubsystemInput.cc			\
	SubsystemScreen.cc			\
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
	WidgetUpgradeMaterial.cc		\
	WidgetWeaponStatus.cc			\
	WormholeWall.cc				\
	glm.cc					\
	main.cc
