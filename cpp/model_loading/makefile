include make_params.make
# CURRENT_PATH = phong_shading
CURRENT_SHADER_PATH = $(SHADER_PATH)\$(CURRENT_PATH)
CURRENT_ASSETS_PATH = $(ASSETS_PATH)\$(CURRENT_PATH)

CC=gcc

.PHONY: copy_shader copy_model copy_assets

CPP_LIST:=$(wildcard $(BASE_DIR)/$(CURRENT_PATH)/*.cpp)
OBJECT_LIST:=$(patsubst %.cpp,%.o,$(CPP_LIST))

all: copy_shader copy_model dist/model_loading.exe

dist/model_loading.exe: $(OBJECT_LIST)
	$(CXX) $(OBJECT_LIST) -o dist/$(CURRENT_PATH).exe -Ldist -lmyopengl -limgui -lmyobject -L$(BASE_DIR)/libs -lassimp -lIrrXML -lzlibstatic

$(BASE_DIR)/$(CURRENT_PATH)/%.o: $(BASE_DIR)/$(CURRENT_PATH)/%.cpp
	$(CXX) -c -O2 $< $(INCLUDE_PATH) -I $(BASE_DIR)/object $(REQUIRED_LIBS) -o $@

copy_shader:
	@if not exist $(SHADER_PATH) (mkdir $(SHADER_PATH) && mkdir $(CURRENT_SHADER_PATH)) else if not exist $(CURRENT_SHADER_PATH) (mkdir $(CURRENT_SHADER_PATH))
	@if exist $(BASE_DIR)\$(CURRENT_PATH)\shader (copy $(BASE_DIR)\$(CURRENT_PATH)\shader $(CURRENT_SHADER_PATH))

copy_assets:
	@if not exist $(ASSETS_PATH) (mkdir $(ASSETS_PATH) && mkdir $(CURRENT_ASSETS_PATH)) else if not exist $(CURRENT_ASSETS_PATH) (mkdir $(CURRENT_ASSETS_PATH))
	@if exist $(BASE_DIR)\$(CURRENT_PATH)\assets (copy $(BASE_DIR)\$(CURRENT_PATH)\assets $(CURRENT_ASSETS_PATH))

copy_model:
	@if not exist dist\IronMan (mkdir dist\IronMan)
	@if not exist dist\nanosuit (mkdir dist\nanosuit)
	@copy $(BASE_DIR)\$(CURRENT_PATH)\IronMan dist\IronMan
	@copy $(BASE_DIR)\$(CURRENT_PATH)\nanosuit dist\nanosuit