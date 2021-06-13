include make_params.make

main: $(BASE_DIR)/examples/*.cpp
	$(CXX) -O2 $(BASE_DIR)/examples/*.cpp $(INCLUDE_PATH) $(REQUIRED_LIBS) -Ldist -lmyopengl -o dist/examples.exe