include make_params.make
# CURRENT_PATH = phong_shading
CURRENT_SHADER_PATH = $(SHADER_PATH)\$(CURRENT_PATH)
CURRENT_ASSETS_PATH = $(ASSETS_PATH)\$(CURRENT_PATH)

.PHONY: copy_shader copy_assets

main: copy_shader copy_assets c/$(CURRENT_PATH)/*.cpp
	$(CXX) -O2 c/$(CURRENT_PATH)/*.cpp $(INCLUDE_PATH) -I c/object $(REQUIRED_LIBS) -Ldist -lmyopengl -lmyobject -limgui -o dist/$(CURRENT_PATH).exe

copy_shader:
	@if not exist $(SHADER_PATH) (mkdir $(SHADER_PATH) && mkdir $(CURRENT_SHADER_PATH)) else if not exist $(CURRENT_SHADER_PATH) (mkdir $(CURRENT_SHADER_PATH))
	@if exist c\$(CURRENT_PATH)\shader (copy c\$(CURRENT_PATH)\shader $(CURRENT_SHADER_PATH))

copy_assets:
	@if not exist $(ASSETS_PATH) (mkdir $(ASSETS_PATH) && mkdir $(CURRENT_ASSETS_PATH)) else if not exist $(CURRENT_ASSETS_PATH) (mkdir $(CURRENT_ASSETS_PATH))
	@if exist c\$(CURRENT_PATH)\assets (copy c\$(CURRENT_PATH)\assets $(CURRENT_ASSETS_PATH))