#######################################
#
# File: 21ordinalLogit.py
# Author: Michel Bierlaire, EPFL
# Date: Sun Jul 14 15:57:13 2013
#
#######################################

from biogeme import *
from headers import *
from loglikelihood import *
from statistics import *
from distributions import *  

# This model is not appropriate for the data. We include it to
# illustrate the syntax for an ordered logit model.  We assume here
# that the response (1,2,3) is ordered. The higher the utility, the
# higher the response.


#Parameters to be estimated
# Arguments:
#   - 1  Name for report; Typically, the same as the variable.
#   - 2  Starting value.
#   - 3  Lower bound.
#   - 4  Upper bound.
#   - 5  0: estimate the parameter, 1: keep it fixed.
#

B_COST	 = Beta('B_COST',0,-10,10,0,"$\beta_\text{cost}$")
B_TIME	 = Beta('B_TIME',0,-10,10,0,"$\beta_\text{time}$")

tau1	 = Beta('tau1',-1,-10000,0,0,"$\tau_1$")
delta2	 = Beta('delta2',2,0,10000,0,"$\delta_2$")

tau2 = tau1 + delta2


#If the person has a GA (season ticket) her incremental cost is actually 0 
#rather than the cost value gathered from the
# network data. 
TRAIN_COST =  TRAIN_CO   * (  GA   ==  0  )

# For numerical reasons, it is good practice to scale the data to
# that the values of the parameters are around 1.0. 
# A previous estimation with the unscaled data has generated
# parameters around -0.01 for both cost and time. Therefore, time and
# cost are multipled my 0.01.

# The following statements are designed to preprocess the data. It is
# like creating a new columns in the data file. This should be
# preferred to the statement like
# TRAIN_TT_SCALED = TRAIN_TT / 100.0
# which will cause the division to be reevaluated again and again,
# throuh the iterations. For models taking a long time to estimate, it
# may make a significant difference.
TRAIN_TT_SCALED = DefineVariable('TRAIN_TT_SCALED', TRAIN_TT / 100.0)
TRAIN_COST_SCALED = DefineVariable('TRAIN_COST_SCALED', TRAIN_COST / 100)


#  Utility

U = B_TIME * TRAIN_TT_SCALED + B_COST * TRAIN_COST_SCALED 


ChoiceProba = {
    1: 1-logisticcdf(U-tau1),
    2: logisticcdf(U-tau1)- logisticcdf(U-tau2),
    3: logisticcdf(U-tau2) }

prob = Elem(ChoiceProba,CHOICE)

# Defines an iterator on the data
rowIterator('obsIter') 

# DEfine the likelihood function for the estimation
BIOGEME_OBJECT.ESTIMATE = Sum(log(prob),'obsIter')


# All observations verifying the following expression will not be
# considered for estimation
# The modeler here has developed the model only for work trips.  
# Observations such that the dependent variable CHOICE is 0 are also removed.
exclude = (( PURPOSE != 1 ) * (  PURPOSE   !=  3  ) + ( CHOICE == 0 )) > 0

BIOGEME_OBJECT.EXCLUDE = exclude


BIOGEME_OBJECT.FORMULAS['Utility'] = U
