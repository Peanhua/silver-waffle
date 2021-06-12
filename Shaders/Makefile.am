shadersdir = $(pkgdatadir)/Shaders

GaussianBlur-Horizontal.vert : GaussianBlur.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'HORIZONTAL'

GaussianBlur-Horizontal.frag : GaussianBlur.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'HORIZONTAL'

GaussianBlur-Vertical.vert : GaussianBlur.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'VERTICAL'

GaussianBlur-Vertical.frag : GaussianBlur.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'VERTICAL'

SceneObject-Color.vert : SceneObject.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ ''

SceneObject-Color.frag : SceneObject.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ ''

SceneObject-Texture.vert : SceneObject.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'USE_TEXTURE'

SceneObject-Texture.frag : SceneObject.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'USE_TEXTURE'

shaders_DATA = \
	AdditiveBlending.frag		\
	AdditiveBlending.vert		\
	Explosion.frag			\
	Explosion.vert			\
	Font.frag			\
	Font.vert			\
	GaussianBlur-Horizontal.frag	\
	GaussianBlur-Horizontal.vert	\
	GaussianBlur-Vertical.frag	\
	GaussianBlur-Vertical.vert	\
	Generic-Color.frag		\
	Generic-Color.vert		\
	Generic-Texture.frag		\
	Generic-Texture.frag		\
	Generic-Texture.vert		\
	Mask.frag			\
	Mask.vert			\
	ProgressBar.frag		\
	ProgressBar.vert		\
	SceneObject-Color.frag		\
	SceneObject-Color.vert		\
	SceneObject-Texture.frag	\
	SceneObject-Texture.vert	\
	Star.frag			\
	Star.vert			

CLEANFILES = \
	GaussianBlur-Horizontal.frag	\
	GaussianBlur-Horizontal.vert	\
	GaussianBlur-Vertical.frag	\
	GaussianBlur-Vertical.vert	\
	SceneObject-Texture.frag	\
	SceneObject-Texture.vert	

EXTRA_DIST = \
	AdditiveBlending.frag		\
	AdditiveBlending.vert		\
	Explosion.frag			\
	Explosion.vert			\
	Font.frag			\
	Font.vert			\
	GaussianBlur.frag		\
	GaussianBlur.vert		\
	Generic-Color.frag		\
	Generic-Color.vert		\
	Generic-Texture.frag		\
	Generic-Texture.frag		\
	Generic-Texture.vert		\
	Mask.frag			\
	Mask.vert			\
	ProgressBar.frag		\
	ProgressBar.vert		\
	SceneObject.frag		\
	SceneObject.vert		\
	Star.frag			\
	Star.vert			
