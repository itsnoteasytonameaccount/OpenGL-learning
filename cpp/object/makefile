include make_params.make

FILES_TO_COMPILE = c/object/*.cpp
TARGET = dist/libmyobject.dll

main: $(FILES_TO_COMPILE) copy_assets c/object
	$(CXX) -shared -fPIC -O2 $(FILES_TO_COMPILE) $(INCLUDE_PATH) $(REQUIRED_LIBS) -Ldist -lmyopengl -Lc/libs -lassimp -lIrrXML -lzlibstatic -o $(TARGET)

copy_assets:
	@if not exist dist\assets (mkdir dist\assets)
	@if exist c\object\assets (copy c\object\assets dist\assets)