#######################################
#
# File: 15panelDiscrete.py
# Author: Michel Bierlaire, EPFL
# Date: Sun May 17 14:19:12 2015
# Compliant with the syntax of Biogeme 2.4
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
W_OTHER = Beta('W_OTHER',0.5,0,1,0)
B_COST = Beta('B_COST',0,-10,10,0)
SIGMA_CAR = Beta('SIGMA_CAR',1,-10,10,0)
SIGMA_SM = Beta('SIGMA_SM',1,-10,10,0)
SIGMA_TRAIN = Beta('SIGMA_TRAIN',1,-10,10,0)

# Define an error component, normally distirbuted, designed to be used
# for Monte-Carlo simulation.
# Note that draws a generated for
# individuals, and are the same for all observcations of the same
# individuals. Individuals are identified by the variable ID.
EC_CAR = SIGMA_CAR * bioDraws('EC_CAR')
EC_SM = SIGMA_SM * bioDraws('EC_SM')
EC_TRAIN = SIGMA_TRAIN * bioDraws('EC_TRAIN')

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
TRAIN_TT_SCALED = DefineVariable('TRAIN_TT_SCALED', TRAIN_TT / 100.0)
TRAIN_COST_SCALED = DefineVariable('TRAIN_COST_SCALED', TRAIN_COST / 100)
SM_TT_SCALED = DefineVariable('SM_TT_SCALED', SM_TT / 100.0)
SM_COST_SCALED = DefineVariable('SM_COST_SCALED', SM_COST / 100)
CAR_TT_SCALED = DefineVariable('CAR_TT_SCALED', CAR_TT / 100)
CAR_CO_SCALED = DefineVariable('CAR_CO_SCALED', CAR_CO / 100)

# For latent class 1, whete the time coefficient is zero
V11 = ASC_TRAIN + B_COST * TRAIN_COST_SCALED + EC_TRAIN
V12 = ASC_SM + B_COST * SM_COST_SCALED + EC_SM
V13 = ASC_CAR + B_COST * CAR_CO_SCALED + EC_CAR

V1 = {1: V11,
      2: V12,
      3: V13}

# For latent class 1, whete the time coefficient is estimated
V21 = ASC_TRAIN + B_TIME * TRAIN_TT_SCALED + B_COST * TRAIN_COST_SCALED + EC_TRAIN
V22 = ASC_SM + B_TIME * SM_TT_SCALED + B_COST * SM_COST_SCALED + EC_SM
V23 = ASC_CAR + B_TIME * CAR_TT_SCALED + B_COST * CAR_CO_SCALED + EC_CAR

V2 = {1: V21,
      2: V22,
      3: V23}


# Associate the availability conditions with the alternatives

CAR_AV_SP =  DefineVariable('CAR_AV_SP',CAR_AV  * (  SP   !=  0  ))
TRAIN_AV_SP =  DefineVariable('TRAIN_AV_SP',TRAIN_AV  * (  SP   !=  0  ))

av = {1: TRAIN_AV_SP,
      2: SM_AV,
      3: CAR_AV_SP}

# Class membership model
probClass1 = 1 - W_OTHER
probClass2 = W_OTHER

# The choice model is a discrete mixture of logit, with availability conditions
prob1 = bioLogit(V1,av,CHOICE)
prob2 = bioLogit(V2,av,CHOICE)

# Iterator on individuals, that is on groups of rows.
metaIterator('personIter','__dataFile__','panelObsIter','ID')

# For each item of personIter, iterates on the rows of the group. 
rowIterator('panelObsIter','personIter')


#Conditional probability for the sequence of choices of an individual
condProbIndiv1 = Prod(prob1,'panelObsIter')
condProbIndiv2 = Prod(prob2,'panelObsIter')
probIndiv = probClass1 * condProbIndiv1 + probClass2 * condProbIndiv2

# Integration by simulation
prob = MonteCarlo(probIndiv)

# Likelihood function
loglikelihood = Sum(log(prob),'personIter')
BIOGEME_OBJECT.ESTIMATE = loglikelihood


# All observations verifying the following expression will not be
# considered for estimation
# The modeler here has developed the model only for work trips.  
# Observations such that the dependent variable CHOICE is 0 are also removed.
exclude = (( PURPOSE != 1 ) * (  PURPOSE   !=  3  ) + ( CHOICE == 0 )) > 0

BIOGEME_OBJECT.EXCLUDE = exclude

BIOGEME_OBJECT.PARAMETERS['NbrOfDraws'] = "5"

BIOGEME_OBJECT.PARAMETERS['optimizationAlgorithm'] = "BIO"
BIOGEME_OBJECT.DRAWS = { 'EC_CAR': ('NORMAL','ID'),
                         'EC_SM': ('NORMAL','ID'),
                         'EC_TRAIN': ('NORMAL','ID')
                         }
BIOGEME_OBJECT.FORMULAS['Train utility - Class 1'] = V11
BIOGEME_OBJECT.FORMULAS['Swissmetro utility - Class 1'] = V12
BIOGEME_OBJECT.FORMULAS['Car utility - Class 1'] = V13
BIOGEME_OBJECT.FORMULAS['Train utility - Class 2'] = V21
BIOGEME_OBJECT.FORMULAS['Swissmetro utility - Class 2'] = V22
BIOGEME_OBJECT.FORMULAS['Car utility - Class 2'] = V23
