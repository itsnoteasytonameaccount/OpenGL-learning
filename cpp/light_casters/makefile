include make_params.make
# CURRENT_PATH = phong_shading
CURRENT_SHADER_PATH = $(SHADER_PATH)\$(CURRENT_PATH)
CURRENT_ASSETS_PATH = $(ASSETS_PATH)\$(CURRENT_PATH)

.PHONY: copy_shader copy_assets

main: copy_shader copy_assets $(BASE_DIR)/$(CURRENT_PATH)/*.cpp
	$(CXX) -O2 $(BASE_DIR)/$(CURRENT_PATH)/*.cpp $(INCLUDE_PATH) -I $(BASE_DIR)/object $(REQUIRED_LIBS) -Ldist -lmyopengl -lmyobject -o dist/$(CURRENT_PATH).exe

copy_shader:
	@if not exist $(SHADER_PATH) (mkdir $(SHADER_PATH) && mkdir $(CURRENT_SHADER_PATH)) else if not exist $(CURRENT_SHADER_PATH) (mkdir $(CURRENT_SHADER_PATH))
	@if exist $(BASE_DIR)\$(CURRENT_PATH)\shader (copy $(BASE_DIR)\$(CURRENT_PATH)\shader $(CURRENT_SHADER_PATH))

copy_assets:
	@if not exist $(ASSETS_PATH) (mkdir $(ASSETS_PATH) && mkdir $(CURRENT_ASSETS_PATH)) else if not exist $(CURRENT_ASSETS_PATH) (mkdir $(CURRENT_ASSETS_PATH))
	@if exist $(BASE_DIR)\$(CURRENT_PATH)\assets (copy $(BASE_DIR)\$(CURRENT_PATH)\assets $(CURRENT_ASSETS_PATH))