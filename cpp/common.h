#ifndef EXPORT
// emscripten
#	if defined(__EMSCRIPTEN__)
#		include <emscripten.h>
#		if defined(__cplusplus)
// EMSCRIPTEN_KEEPALIVE为emscripten自带变量，导出函数必须加
#			define EXPORT(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#		else
#			define EXPORT(rettype) rettype EMSCRIPTEN_KEEPALIVE
#		endif
#	else
// c++
#		if defined(__cplusplus)
#			define EXPORT(rettype) extern "C" rettype
#		else
#			define EXPORT(rettype) rettype
#		endif
#	endif
#endif