// Not sure if we will use it

class bioObservationData {

public :

  bioObservationData() {}

  bioObservationData(vector<patReal> listOfVariables) {
    this->listOfVariables = listOfVariables ;
  }

  void addVariable(patReal val) {
    listOfVariables.push_back(val) ;
  }

  void printObservationData() const {
    for (vector<patReal>::const_iterator it=listOfVariables.begin() ;
         it != listOfVariables.end() ;
         it++ ) {
       std::cout << *it << "   " ;
    }
    std::cout << std::endl ;
  }

  patReal at(int index) {
    return listOfVariables.at(index) ;
  }

private :
  vector<patReal> listOfVariables ;
};
