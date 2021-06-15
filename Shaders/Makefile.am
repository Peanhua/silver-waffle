shadersdir = $(pkgdatadir)/Shaders

GaussianBlur-Horizontal.vert : GaussianBlur.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'HORIZONTAL'

GaussianBlur-Horizontal.frag : GaussianBlur.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'HORIZONTAL'

GaussianBlur-Vertical.vert : GaussianBlur.vert
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'VERTICAL'

GaussianBlur-Vertical.frag : GaussianBlur.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'VERTICAL'

SceneObject-Color.vert : SceneObject.vert Include/Input.vert Include/Output.vert Include/Data.glsl Include/Uniforms.glsl
	$(AM_V_GEN) ./generate-variants.sh $< $@ ''

SceneObject-Color.frag : SceneObject.frag Include/Input.frag Include/Output.frag Include/Data.glsl Include/Uniforms.glsl Include/DirectionalLight.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ ''

SceneObject-Texture.vert : SceneObject.vert Include/Input.vert Include/Output.vert Include/Data.glsl Include/Uniforms.glsl 
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'USE_TEXTURE'

SceneObject-Texture.frag : SceneObject.frag Include/Data.glsl Include/Output.frag Include/Data.glsl Include/Uniforms.glsl Include/DirectionalLight.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'USE_TEXTURE'

BonusLevelEntrance.vert : SceneObject.vert  Include/Input.vert Include/Output.vert Include/Data.glsl Include/Uniforms.glsl
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'USE_ALPHA_DISTANCE20_FADE USE_BLEND'

BonusLevelEntrance.frag : SceneObject.frag Include/Input.frag Include/Output.frag Include/Data.glsl Include/Uniforms.glsl Include/DirectionalLight.frag
	$(AM_V_GEN) ./generate-variants.sh $< $@ 'USE_ALPHA_DISTANCE20_FADE USE_BLEND'


shaders_DATA = \
	AdditiveBlending.frag		\
	AdditiveBlending.vert		\
	BonusLevelEntrance.frag		\
	BonusLevelEntrance.vert		\
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
	BonusLevelEntrance.frag		\
	BonusLevelEntrance.vert		\
	GaussianBlur-Horizontal.frag	\
	GaussianBlur-Horizontal.vert	\
	GaussianBlur-Vertical.frag	\
	GaussianBlur-Vertical.vert	\
	SceneObject-Color.frag		\
	SceneObject-Color.vert		\
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
	SceneObject.geom		\
	SceneObject.vert		\
	Star.frag			\
	Star.vert			
