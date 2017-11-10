#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "testState.h"
#include "patErrOutOfRange.h"

testState::testState() : bioMarkovState() {

}

testState::testState(patULong id, vector<patReal> w) :
  theId(id),
  weights(w) {

}
patReal testState::getWeight(patError*& err) const {
  if (theId >= weights.size()) {
    err = new patErrOutOfRange<patULong>(theId,0,weights.size()-1) ;
    WARNING(err->describe()) ;
    return patReal() ;
  }
  return weights[theId] ;
}


patULong testState::getIndicator(patError*& err) const {
  return theId ;
}
