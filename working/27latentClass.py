########################################
#
# @file 27latentClass.py
# @author: Michel Bierlaire, EPFL
# @date: Wed Dec 21 13:23:27 2011
#
# Logit model with latent classes
# Three alternatives: Train, Car and Swissmetro
# SP data
#
#######################################

from biogeme import *
from headers import *
from statistics import *

#Parameters to be estimated
# Arguments:
#   - 1  Name for report; Typically, the same as the variable.
#   - 2  Starting value.
#   - 3  Lower bound.
#   - 4  Upper bound.
#   - 5  0: estimate the parameter, 1: keep it fixed.
#
ASC_CAR_C1 = Beta('ASC_CAR_C1',1,-1000,1000,0)
ASC_TRAIN_C1 = Beta('ASC_TRAIN_C1',1,-1000,1000,0)
B_TIME_C1 = Beta('B_TIME_C1',-1,-1000,1000,0)
B_COST_C1 = Beta('B_COST_C1',-1,-1000,1000,0)

ASC_CAR_C2 = Beta('ASC_CAR_C2',0,-1000,1000,0)
ASC_TRAIN_C2 = Beta('ASC_TRAIN_C2',0,-1000,1000,0)
B_TIME_C2 = Beta('B_TIME_C2',0,-1000,1000,0)
B_COST_C2 = Beta('B_COST_C2',0,-1000,1000,0)

ASC_CAR_C3 = Beta('ASC_CAR_C3',-1,-1000,1000,0)
ASC_TRAIN_C3 = Beta('ASC_TRAIN_C3',-1,-1000,1000,0)
B_TIME_C3 = Beta('B_TIME_C3',0,-1000,1000,0)
B_COST_C3 = Beta('B_COST_C3',0,-1000,1000,0)

#previous_ASC_CAR = [1.0, -0.155]
#previous_ASC_TRAIN = [1.0, -0.701]
#previous_B_TIME = [2.0, -1.28]
#previous_B_COST = [3.0, -1.08]

#BIOGEME_OBJECT.OLDBETAS = {'ASC_CAR': previous_ASC_CAR, 'ASC_TRAIN': previous_ASC_TRAIN, 'B_TIME': previous_B_TIME, 'B_COST':previous_B_COST}
#BIOGEME_OBJECT.PARAMETERS['prematureConvergenceThreshold'] = "1.0"

# Utility functions

#If the person has a GA (season ticket) her incremental cost 
#is actually 0 rather than the cost value gathered from the
# network data. 

SM_COST =  SM_CO   * (  GA   ==  0  ) 
TRAIN_COST =  TRAIN_CO   * (  GA   ==  0  )

# For numerical reasons, it is good practice to scale the data to
# that the values of the parameters are around 1.0. 
# A previous estimation with the unscaled data has generated
# parameters around -0.01 for both cost and time. Therefore, time
# and cost are divided by 100.

# The following statements are designed to preprocess the data. 
# It is like creating a new columns in the data file. This should
# be preferred to the statement like
# TRAIN_TT_SCALED = TRAIN_TT / 100.0
# which causes the division to be reevaluated again and again,
# throuh the iterations. For models taking a long time to
# estimate, it may make a significant difference.
 
TRAIN_TT_SCALED = DefineVariable('TRAIN_TT_SCALED',\
   TRAIN_TT / 100.0)
TRAIN_COST_SCALED = DefineVariable('TRAIN_COST_SCALED',\
   TRAIN_COST / 100)
SM_TT_SCALED = DefineVariable('SM_TT_SCALED', SM_TT / 100.0)
SM_COST_SCALED = DefineVariable('SM_COST_SCALED', SM_COST / 100)
CAR_TT_SCALED = DefineVariable('CAR_TT_SCALED', CAR_TT / 100)
CAR_CO_SCALED = DefineVariable('CAR_CO_SCALED', CAR_CO / 100)

V1_Class1 = ASC_TRAIN_C1 + \
     B_TIME_C1 * TRAIN_TT_SCALED + \
     B_COST_C1 * TRAIN_COST_SCALED
V2_Class1 = B_TIME_C1 * SM_TT_SCALED + \
     B_COST_C1 * SM_COST_SCALED
V3_Class1 = ASC_CAR_C1 + \
     B_TIME_C1 * CAR_TT_SCALED + \
     B_COST_C1 * CAR_CO_SCALED

V1_Class2 = ASC_TRAIN_C2 + \
     B_TIME_C2 * TRAIN_TT_SCALED + \
     B_COST_C2 * TRAIN_COST_SCALED
V2_Class2 = B_TIME_C2 * SM_TT_SCALED + \
     B_COST_C2 * SM_COST_SCALED
V3_Class2 = ASC_CAR_C2 + \
     B_TIME_C2 * CAR_TT_SCALED + \
     B_COST_C2 * CAR_CO_SCALED

V1_Class3 = ASC_TRAIN_C3 + \
     B_TIME_C3 * TRAIN_TT_SCALED + \
     B_COST_C3 * TRAIN_COST_SCALED
V2_Class3 = B_TIME_C3 * SM_TT_SCALED + \
     B_COST_C3 * SM_COST_SCALED
V3_Class3 = ASC_CAR_C3 + \
     B_TIME_C3 * CAR_TT_SCALED + \
     B_COST_C3 * CAR_CO_SCALED



# Associate utility functions with the numbering of alternatives
V_Class1 = {1: V1_Class1,
            2: V2_Class1,
            3: V3_Class1}

V_Class2 = {1: V1_Class2,
            2: V2_Class2,
            3: V3_Class2}

V_Class3 = {1: V1_Class3,
            2: V2_Class3,
            3: V3_Class3}

# Associate the availability conditions with the alternatives
CAR_AV_SP =  DefineVariable('CAR_AV_SP',CAR_AV  * (  SP   !=  0  ))
TRAIN_AV_SP =  DefineVariable('TRAIN_AV_SP',TRAIN_AV  * (  SP   !=  0  ))

av = {1: TRAIN_AV_SP,
      2: SM_AV,
      3: CAR_AV_SP}

avone = {1:1,2:1,3:1}
# The choice model is a logit, with availability conditions
prob_Class1 = bioLogit(V_Class1,av,CHOICE)
prob_Class2 = bioLogit(V_Class2,av,CHOICE)
prob_Class3 = bioLogit(V_Class3,av,CHOICE)


pi1 = Beta('pi1',0,-1000,1000,1)
pi2 = Beta('pi2',0,-1000,1000,0)
pi3 = Beta('pi3',0,-1000,1000,0)

pi = {1:pi1, 2:pi2, 3:pi3}

prob = bioLogit(pi,avone,1) * prob_Class1 + bioLogit(pi,avone,2) * prob_Class2 + bioLogit(pi,avone,3) * prob_Class3

# Defines an itertor on the data
rowIterator('obsIter') 

# DEfine the likelihood function for the estimation
BIOGEME_OBJECT.ESTIMATE = Sum(log(prob),'obsIter')

# All observations verifying the following expression will not be
# considered for estimation
# The modeler here has developed the model only for work trips.  
# Observations such that the dependent variable CHOICE is 0 
# are also removed.
exclude = (( PURPOSE != 1 ) * (  PURPOSE   !=  3  ) + \
   ( CHOICE == 0 )) > 0

BIOGEME_OBJECT.EXCLUDE = exclude


BIOGEME_OBJECT.PARAMETERS['optimizationAlgorithm'] = "BIO"
BIOGEME_OBJECT.PARAMETERS['biogemeDisplay'] = "3"


# Statistics

nullLoglikelihood(av,'obsIter')
choiceSet = [1,2,3]
cteLoglikelihood(choiceSet,CHOICE,'obsIter')
availabilityStatistics(av,'obsIter')

#BIOGEME_OBJECT.PARAMETERS['printGradient'] = "1"
#BIOGEME_OBJECT.PARAMETERS['forceScientificNotation'] = "0"

#BIOGEME_OBJECT.PARAMETERS['precisionParameters'] = "3"
#BIOGEME_OBJECT.PARAMETERS['precisionStatistics'] = "3"
#BIOGEME_OBJECT.PARAMETERS['precisionTStats'] = "14"

BIOGEME_OBJECT.PARAMETERS['bootstrapStdErr'] = "100"
BIOGEME_OBJECT.PARAMETERS['biogemeDisplay'] = "3"

