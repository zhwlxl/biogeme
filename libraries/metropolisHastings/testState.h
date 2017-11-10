#include "bioMarkovState.h"

class testState : public bioMarkovState {
 public:
  testState() ;
  testState(patULong id, vector<patReal> w) ;
  virtual patULong getIndicator(patError*& err) const ;
  virtual patReal getWeight(patError*& err) const ;
  patULong theId ;
  
 private:
  vector<patReal> weights ;
};
