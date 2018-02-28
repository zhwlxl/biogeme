#######################################
#
# File: 02weight.py
# Author: Michel Bierlaire, EPFL
# Date: Wed Dec 21 13:24:06 2011
#
#######################################

from biogeme import *
from headers import *
from loglikelihood import *
from statistics import *

#Parameters to be estimated
# Arguments:
#   1  Name for report. Typically, the same as the variable
#   2  Starting value
#   3  Lower bound
#   4  Upper bound
#   5  0: estimate the parameter, 1: keep it fixed

ASC_CAR = Beta('ASC_CAR',0,-10,10,0)
ASC_TRAIN = Beta('ASC_TRAIN',0,-10,10,0)
ASC_SM = Beta('ASC_SM',0,-10,10,1)
B_TIME = Beta('B_TIME',0,-10,10,0)
B_COST = Beta('B_COST',0,-10,10,0)

# Utility functions

#If the person has a GA (season ticket) her incremental cost is actually 0 
#rather than the cost value gathered from the
# network data. 
SM_COST =  SM_CO   * (  GA   ==  0  ) 
TRAIN_COST =  TRAIN_CO   * (  GA   ==  0  )

# For numerical reasons, it is good practice to scale the data to
# that the values of the parameters are around 1.0. 
# A previous estimation with the unscaled data has generated
# parameters around -0.01 for both cost and time. Therefore, time and
# cost are multipled my 0.01.
# The use of user-defined expressions optimizes the run of
# biogeme. Indeed, user-defined expressions are evaluated before the
# estimation starts, and stored as data. This may lead to a
# significant gain of estimation time.

TRAIN_TT_SCALED = DefineVariable('TRAIN_TT_SCALED', TRAIN_TT / 100.0)
TRAIN_COST_SCALED = DefineVariable('TRAIN_COST_SCALED', TRAIN_COST / 100)
SM_TT_SCALED = DefineVariable('SM_TT_SCALED', SM_TT / 100.0)
SM_COST_SCALED = DefineVariable('SM_COST_SCALED', SM_COST / 100)
CAR_TT_SCALED = DefineVariable('CAR_TT_SCALED', CAR_TT / 100)
CAR_CO_SCALED = DefineVariable('CAR_CO_SCALED', CAR_CO / 100)

V1 = ASC_TRAIN + B_TIME * TRAIN_TT_SCALED + B_COST * TRAIN_COST_SCALED
V2 = ASC_SM + B_TIME * SM_TT_SCALED + B_COST * SM_COST_SCALED
V3 = ASC_CAR + B_TIME * CAR_TT_SCALED + B_COST * CAR_CO_SCALED

# Associate utility functions with the numbering of alternatives
V = {1: V1,
     2: V2,
     3: V3}

# Associate the availability conditions with the alternatives
CAR_AV_SP =  DefineVariable('CAR_AV_SP',CAR_AV  * (  SP   !=  0  ))
TRAIN_AV_SP =  DefineVariable('TRAIN_AV_SP',TRAIN_AV  * (  SP   !=  0  ))


av = {1: TRAIN_AV_SP,
      2: SM_AV,
      3: CAR_AV_SP}

# The choice model is a logit, with availability conditions
logprob = bioLogLogit(V,av,CHOICE)

# Defines an itertor on the data
rowIterator('obsIter') 

# To illustrate the syntax for weight, we assign a weigh of
# 1.2 to all observations corresponding to group 3. The weight is also
# normalized so that the sum across the sample is the sample size.
weight = 8.890991e-01 * (1.0 * (GROUP == 2) + 1.2 * (GROUP == 3))

# Define the likelihood function for the estimation

BIOGEME_OBJECT.WEIGHT = weight
BIOGEME_OBJECT.ESTIMATE = Sum(logprob,'obsIter')

# All observations verifying the following expression will not be
# considered for estimation
# The modeler here has developed the model only for work trips.  
# Observations such that the dependent variable CHOICE is 0 are also removed.
exclude = (( PURPOSE != 1 ) * (  PURPOSE   !=  3  ) + ( CHOICE == 0 )) > 0

BIOGEME_OBJECT.EXCLUDE = exclude

# Statistics

nullLoglikelihood(av,'obsIter')
choiceSet = [1,2,3]
cteLoglikelihood(choiceSet,CHOICE,'obsIter')
availabilityStatistics(av,'obsIter')

BIOGEME_OBJECT.STATISTICS['Sum of weights'] = Sum(weight,'obsIter')
BIOGEME_OBJECT.PARAMETERS['optimizationAlgorithm'] = "BIO"

BIOGEME_OBJECT.FORMULAS['Train utility'] = V1
BIOGEME_OBJECT.FORMULAS['Swissmetro utility'] = V2
BIOGEME_OBJECT.FORMULAS['Car utility'] = V3
#BIOGEME_OBJECT.PARAMETERS['bootstrapStdErr'] = "1000"
