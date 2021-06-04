ROOT_PATH ?= $(shell echo %cd%)

INCLUDE_PATH = -I $(CUSTOM_LIBS_PATH)/include -I $(CUSTOM_LIBS_PATH)/imgui -I  $(CUSTOM_LIBS_PATH)/imgui/backends -I c/libs/include -I c/object
REQUIRED_LIBS = -Llib -lglfw3 -LC:/Windows/WinSxS/amd64_microsoft-windows-gdi32_31bf3856ad364e35_10.0.18362.1_none_2bdc19f7c689fa4b -lgdi32

DST_PATH := $(ROOT_PATH)\dist

# 由于在cmd命令中引用了这两个路径，所以只能反斜杠分隔
RSC_PATH := $(DST_PATH)\$(CURRENT_PATH)
SRC_PATH := $(ROOT_PATH)\c\$(CURRENT_PATH)

SHADER_PATH = dist\shader
ASSETS_PATH = dist\assets