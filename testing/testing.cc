//-*-c++-*------------------------------------------------------------
//
// File name : testing.cc
// Author :    Michel Bierlaire
// Date :      Sun Jul 12 16:54:24 2015
//
//--------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <dlfcn.h>
#include "patDisplay.h"

#include "patGetWorkingDir.h"

int main(int argc, char *argv[]) {
  void* lib_handle = dlopen("libbiogemedylib.dylib", RTLD_LOCAL|RTLD_LAZY);
  if (!lib_handle) {
    WARNING("Unable to open libbiogemedylib.dylib: " << dlerror()) ;
    exit(-1) ;
 }
  typedef void (*runBiogeme_t)(const char*, const char*);
  runBiogeme_t runBiogeme = (runBiogeme_t) dlsym(lib_handle, "runBiogeme");
  if (!runBiogeme) {    
    WARNING("Unable to get runBiogeme: " << dlerror());
    exit(-1);
  }
  patString m("01logit") ;
  patString d("swissmetro.dat") ;
  runBiogeme(m.c_str(),d.c_str()) ;
}
