shadersdir = $(pkgdatadir)/Shaders

SceneObject-Texture.vert : SceneObject-Color.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@

SceneObject-Texture.frag : SceneObject-Color.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@

shaders_DATA = \
	SceneObject-Color.vert		\
	SceneObject-Color.frag		\
	SceneObject-Texture.vert	\
	SceneObject-Texture.frag	\
	Explosion.vert			\
	Explosion.frag			\
	Font.vert			\
	Font.frag			\
	Generic-Color.vert		\
	Generic-Color.frag		\
	Generic-Texture.vert		\
	Generic-Texture.frag		\
	Generic-Texture.frag		\
	Mask.vert			\
	Mask.frag			\
	ProgressBar.vert		\
	ProgressBar.frag		\
	Star.vert			\
	Star.frag			

CLEANFILES = \
	SceneObject-Texture.vert	\
	SceneObject-Texture.frag	

EXTRA_DIST = \
	SceneObject-Color.vert		\
	SceneObject-Color.frag		\
	Explosion.vert			\
	Explosion.frag			\
	Font.vert			\
	Font.frag			\
	Generic-Color.vert		\
	Generic-Color.frag		\
	Generic-Texture.vert		\
	Generic-Texture.frag		\
	Generic-Texture.frag		\
	Mask.vert			\
	Mask.frag			\
	ProgressBar.vert		\
	ProgressBar.frag		\
	Star.vert			\
	Star.frag			
