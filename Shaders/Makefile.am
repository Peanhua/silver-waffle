shadersdir = $(pkgdatadir)/Shaders

GaussianBlur-Horizontal.vert : GaussianBlur.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ '#define HORIZONTAL'

GaussianBlur-Horizontal.frag : GaussianBlur.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ '#define HORIZONTAL'

GaussianBlur-Vertical.vert : GaussianBlur.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ '#define VERTICAL'

GaussianBlur-Vertical.frag : GaussianBlur.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ '#define VERTICAL'

SceneObject-Texture.vert : SceneObject-Color.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ '#define USE_TEXTURE'

SceneObject-Texture.frag : SceneObject-Color.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ '#define USE_TEXTURE'

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
	SceneObject-Color.frag		\
	SceneObject-Color.vert		\
	Star.frag			\
	Star.vert			
