include make_params.make

DYNAMIC_LIBRARY = dist/libmyopengl.dll dist/libmyobject.dll dist/libimgui.dll

all: $(COMPILE_PATH)

ifdef SILENT_OPTION
	SILENT_OPTION = -s
endif

.PHONY: $(COMPILE_PATH) clean test

OBJS := $(wildcard $(BASE_DIR)/libs/sr$(BASE_DIR)/*.cpp)
OBJS2 := $(wildcard $(BASE_DIR)/object/*.cpp)
IMGUI_OBJS := $(wildcard $(CUSTOM_LIBS_PATH)/imgui/*.cpp)
IMGUI_OBJS := $(IMGUI_OBJS) $(CUSTOM_LIBS_PATH)/imgui/backends/imgui_impl_glfw.cpp $(CUSTOM_LIBS_PATH)/imgui/backends/imgui_impl_opengl3.cpp

dist/libmyopengl.dll: $(OBJS)
	$(MAKE) $(SILENT_OPTION) -f $(BASE_DIR)/libs/makefile

dist/libmyobject.dll: dist/libmyopengl.dll $(OBJS2)
	$(MAKE) $(SILENT_OPTION) -f $(BASE_DIR)/object/makefile

dist/libimgui.dll: $(IMGUI_OBJS) dist/libmyopengl.dll
	$(CXX) -shared -fPIC -O3 $(IMGUI_OBJS) -o dist/libimgui.dll $(INCLUDE_PATH) -Ldist -lmyopengl

# 编译方式可能有些许差别，所以每个单独写一个文件
$(COMPILE_PATH): $(DYNAMIC_LIBRARY)
	$(MAKE) $(SILENT_OPTION) -f $(BASE_DIR)/$@/makefile CURRENT_PATH=$@

clean:
	del dist\libmyopengl.dll dist\libmyobject.dll

