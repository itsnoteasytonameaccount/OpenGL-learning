include make_params.make
# CURRENT_PATH = phong_shading
CURRENT_SHADER_PATH = $(SHADER_PATH)\$(CURRENT_PATH)

.PHONY: copy_shader

main: copy_shader c/$(CURRENT_PATH)/*.cpp
	$(CXX) -O2 c/$(CURRENT_PATH)/*.cpp $(INCLUDE_PATH) -I c/object $(REQUIRED_LIBS) -Ldist -lmyopengl -lmyobject -o dist/$(CURRENT_PATH).exe

copy_shader:
	@if not exist $(SHADER_PATH) (mkdir $(SHADER_PATH) && mkdir $(CURRENT_SHADER_PATH)) else if not exist $(CURRENT_SHADER_PATH) (mkdir $(CURRENT_SHADER_PATH))
	@copy c\$(CURRENT_PATH)\shader $(CURRENT_SHADER_PATH)
	