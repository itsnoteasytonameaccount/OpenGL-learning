include make_params.make

DIRS = shader assets

.PHONY: $(DIRS) mk_rsc_dir

$(DST_PATH)/$(CURRENT_PATH).exe: $(SRC_PATH)/*.cpp $(DIRS) mk_rsc_dir
	$(CXX) -O2 $(SRC_PATH)/*.cpp $(INCLUDE_PATH) $(REQUIRED_LIBS) -L$(DST_PATH) -lmyopengl -lmyobject -limgui -o $@

$(DIRS):
	@if not exist $(RSC_PATH)\$@ (mkdir $(RSC_PATH)\$@)
	@xcopy $(SRC_PATH)\$@ $(RSC_PATH)\$@ /E /Y

mk_rsc_dir:
	@if not exist $(RSC_PATH) (mkdir $(RSC_PATH))
