AM_CXXFLAGS =                \
	$(ASSIMP_CFLAGS)     \
	$(FREETYPE2_CFLAGS)  \
        $(GL_CFLAGS)         \
        $(GLEW_CFLAGS)       \
        $(GLM_CFLAGS)        \
	$(JSON11_CFLAGS)     \
	$(LIBPNG_CFLAGS)     \
        $(SDL2_CFLAGS)       \
        $(SDL2_image_CFLAGS) \
	$(VALGRIND_CFLAGS)

bin_PROGRAMS = silverwaffle

silverwaffle_LDADD =       \
	$(ASSIMP_LIBS)     \
	$(FREETYPE2_LIBS)  \
        $(GL_LIBS)         \
        $(GLEW_LIBS)       \
	$(JSON11_LIBS)     \
	$(LIBPNG_LIBS)     \
        $(SDL2_LIBS)       \
        $(SDL2_image_LIBS)

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
	Frustum.cc				\
	GameStats.cc				\
	GaussianBlur.cc				\
	HighscoreEntry.cc			\
	Image.cc				\
	Image_Save.cc				\
	Level.cc				\
	LevelBonus.cc				\
	LevelPlanet.cc				\
	LevelSpace.cc				\
	Loot.cc					\
	Mesh.cc					\
	MeshOverlay.cc				\
	MeshProgressBar.cc			\
	Mesh_LoadFromFile.cc			\
	Milkyway.cc				\
	Object.cc				\
	ObjectBonusLevelEntrance.cc		\
	ObjectCloud.cc				\
	ObjectCollectible.cc			\
	ObjectInvader.cc			\
	ObjectMovable.cc			\
	ObjectPlanet.cc				\
	ObjectPlanetAtmosphere.cc		\
	ObjectPlanetGround.cc			\
	ObjectProjectile.cc			\
	ObjectSpaceship.cc			\
	Plane.cc				\
	Scene.cc				\
	ScenePlanet.cc				\
	SceneSpace.cc				\
	ScoreReel.cc				\
	Screen.cc				\
	ScreenLevel.cc				\
	ScreenLevelBonus.cc			\
	ScreenLevelPlanet.cc			\
	ScreenLevelSpace.cc			\
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
	WidgetScoreReel.cc			\
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
