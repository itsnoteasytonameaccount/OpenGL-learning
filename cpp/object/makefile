include make_params.make

FILES_TO_COMPILE = $(BASE_DIR)/object/*.cpp
TARGET = dist/libmyobject.dll

main: $(FILES_TO_COMPILE) copy_assets $(BASE_DIR)/object
	$(CXX) -shared -fPIC -O2 $(FILES_TO_COMPILE) $(INCLUDE_PATH) $(REQUIRED_LIBS) -Ldist -lmyopengl -L$(BASE_DIR)/libs -lassimp -lIrrXML -lzlibstatic -o $(TARGET)

copy_assets:
	@if not exist dist\assets (mkdir dist\assets)
	@if exist $(BASE_DIR)\object\assets (copy $(BASE_DIR)\object\assets dist\assets)