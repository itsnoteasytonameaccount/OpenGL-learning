include make_params.make

main: c/examples/*.cpp
	$(CXX) -O2 c/examples/*.cpp $(INCLUDE_PATH) $(REQUIRED_LIBS) -Ldist -lmyopengl -o dist/examples.exe