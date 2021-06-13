include make_params.make

FILES_TO_COMPILE = $(BASE_DIR)/libs/src/*.cpp $(BASE_DIR)/libs/src/*.c
MID_PRODUCT = glad.o
TARGET = dist/libmyopengl.dll

main: $(MID_PRODUCT) $(FILES_TO_COMPILE)
	$(CXX) -shared -fPIC -O2 $(FILES_TO_COMPILE) $(MID_PRODUCT) $(INCLUDE_PATH) $(REQUIRED_LIBS) -o $(TARGET)

$(MID_PRODUCT): $(CUSTOM_LIBS_PATH)/src/glad.c
	$(CXX) -c $(CUSTOM_LIBS_PATH)/src/glad.c $(INCLUDE_PATH)
	