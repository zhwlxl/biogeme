#######################################

#

# File: 21probit.py

# Author: Michel Bierlaire, EPFL

# Date: Tue Aug  9 07:56:18 2011

#

#######################################



from biogeme import *

from headers import *

from loglikelihood import *

from statistics import *



# This estimates a binary probit model. All observations where Swissmetro was chosen are discarded



#Parameters to be estimated

# Arguments:

#   1  Name for report. Typically, the same as the variable

#   2  Starting value

#   3  Lower bound

#   4  Upper bound

#   5  0: estimate the parameter, 1: keep it fixed



ASC_CAR = Beta('ASC_CAR',0,-10,10,0,'Car cte.')

ASC_TRAIN = Beta('ASC_TRAIN',0,-10,10,1,'Train cte.')

B_TIME = Beta('B_TIME',0,-10,10,0,'Travel time')

B_COST = Beta('B_COST',0,-10,10,0,'Travel cost')


# Utility functions



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

CAR_TT_SCALED = DefineVariable('CAR_TT_SCALED', CAR_TT / 100)

CAR_CO_SCALED = DefineVariable('CAR_CO_SCALED', CAR_CO / 100)



V1 = ASC_TRAIN + B_TIME * TRAIN_TT_SCALED + B_COST * TRAIN_COST_SCALED

V3 = ASC_CAR + B_TIME * CAR_TT_SCALED + B_COST * CAR_CO_SCALED # 



# Associate choice probability with the numbering of alternatives

P = {1: bioNormalCdf(V1-V3),

     3: bioNormalCdf(V3-V1)}



# Associate the availability conditions with the alternatives

CAR_AV_SP =  DefineVariable('CAR_AV_SP',CAR_AV  * (  SP   !=  0  ))

TRAIN_AV_SP =  DefineVariable('TRAIN_AV_SP',TRAIN_AV  * (  SP   !=  0  ))



av = {1: TRAIN_AV_SP,

      3: CAR_AV_SP}



prob = Elem(P,CHOICE)



# Defines an itertor on the data

rowIterator('obsIter') 



# DEfine the likelihood function for the estimation

BIOGEME_OBJECT.ESTIMATE = Sum(log(prob),'obsIter')



# All observations verifying the following expression will not be

# considered for estimation

# The modeler here has developed the model only for work trips.  

# Observations such that the dependent variable CHOICE is 0 are also removed.

exclude = (TRAIN_AV_SP == 0) + (CAR_AV_SP == 0) + ( CHOICE == 2 ) + (( PURPOSE != 1 ) * (  PURPOSE   !=  3  ) + ( CHOICE == 0 )) > 0



BIOGEME_OBJECT.EXCLUDE = exclude



# Statistics



nullLoglikelihood(av,'obsIter')

choiceSet = [1,3]

cteLoglikelihood(choiceSet,CHOICE,'obsIter')

availabilityStatistics(av,'obsIter')

BIOGEME_OBJECT.FORMULAS['Train utility'] = V1
BIOGEME_OBJECT.FORMULAS['Car utility'] = V3
