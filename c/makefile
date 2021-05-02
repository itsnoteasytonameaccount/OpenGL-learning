
INCLUDE_PATH = $(CUSTOM_LIBS_PATH)/include
REQUIRED_LIBS = -Llib -lglfw3 -LC:/Windows/WinSxS/amd64_microsoft-windows-gdi32_31bf3856ad364e35_10.0.18362.1_none_2bdc19f7c689fa4b -lgdi32
MID_PRODUCT = glad.o stb_image.o
TARGET = dist/main.exe

ifeq ('$(FILE_TO_COMPILE)','')
	FILE_TO_COMPILE = c/Box.cpp c/Texture.cpp c/GLWindow.cpp c/Shader.cpp
endif

# 伪目标
.PHONY: clean clean_mid_product copy_files

main: ${MID_PRODUCT} copy_files
	$(CXX) ${FILE_TO_COMPILE} ${MID_PRODUCT} $(REQUIRED_LIBS) -I $(INCLUDE_PATH) -o $(TARGET)

glad.o: $(CUSTOM_LIBS_PATH)/src/glad.c
	$(CXX) -c $(CUSTOM_LIBS_PATH)/src/glad.c -I $(INCLUDE_PATH)

stb_image.o: c/stb_image.cpp
	$(CXX) -c c/stb_image.cpp -I $(INCLUDE_PATH)

copy_files:
	@if not exist dist\shader (mkdir dist\shader)
	@copy c\shader dist\shader

clean:
	@del ${MID_PRODUCT} $(TARGET)

clean_mid_product:
	@del ${MID_PRODUCT}
