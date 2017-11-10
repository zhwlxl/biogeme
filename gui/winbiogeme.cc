#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "patWinBiogeme.h"

// GUI for biogeme

int main() {
  patWinBiogeme* theWin = new patWinBiogeme() ;
  return(Fl::run());

}
