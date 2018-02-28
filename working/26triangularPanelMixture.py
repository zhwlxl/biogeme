#######################################
#
# File: 26triangularPanelMixture.py
# Author: Michel Bierlaire, EPFL
# Date: Sun May 17 14:16:41 2015
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
B_COST = Beta('B_COST',0,-10,10,0)
SIGMA_CAR = Beta('SIGMA_CAR',0,-10,10,0)
SIGMA_SM = Beta('SIGMA_SM',0,-10,10,0)
SIGMA_TRAIN = Beta('SIGMA_TRAIN',0,-10,10,0)

# Define draws for Monte-Carlo simulation
BIOGEME_OBJECT.DRAWS = { 
                         'EC_CAR_U1': ('UNIFORMSYM','ID'),
                         'EC_CAR_U2': ('UNIFORMSYM','ID'),
                         'EC_SM_U1': ('UNIFORMSYM','ID'),
                         'EC_SM_U2': ('UNIFORMSYM','ID'),
                         'EC_TRAIN_U1': ('UNIFORMSYM','ID'),
                         'EC_TRAIN_U2': ('UNIFORMSYM','ID')
                         }


# Define an error component following a triangular distribution,
# designed to be used for Monte-Carlo simulation.
# Note that this distribution is not pre-packaged in biogeme, and the
# user has to provide the formula.
# Note also that draws a generated for individuals, and are the same
# for all observcations of the same individuals. Individuals are
# identified by the variable ID.
TRIANG_CAR = DefineDraws('TRIANG_CAR', 0.5 * (bioDraws('EC_CAR_U1')+bioDraws('EC_CAR_U2')),'personIter')
TRIANG_SM = DefineDraws('TRIANG_SM', 0.5 * (bioDraws('EC_SM_U1')+bioDraws('EC_SM_U2')),'personIter')
TRIANG_TRAIN = DefineDraws('TRIANG_TRAIN', 0.5 * (bioDraws('EC_TRAIN_U1')+bioDraws('EC_TRAIN_U2')),'personIter')

EC_CAR = SIGMA_CAR * TRIANG_CAR
EC_SM = SIGMA_SM * TRIANG_SM
EC_TRAIN = SIGMA_TRAIN * TRIANG_TRAIN

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


# The following statements are designed to preprocess the data. It is
# like creating a new columns in the data file. This should be
# preferred to the statement like
# TRAIN_TT_SCALED = TRAIN_TT / 100.0
# which will cause the division to be reevaluated again and again,
# throuh the iterations. For models taking a long time to estimate, it
# may make a significant difference.
 
TRAIN_TT_SCALED = DefineVariable('TRAIN_TT_SCALED', TRAIN_TT / 100.0)
TRAIN_COST_SCALED = DefineVariable('TRAIN_COST_SCALED', TRAIN_COST / 100)
SM_TT_SCALED = DefineVariable('SM_TT_SCALED', SM_TT / 100.0)
SM_COST_SCALED = DefineVariable('SM_COST_SCALED', SM_COST / 100)
CAR_TT_SCALED = DefineVariable('CAR_TT_SCALED', CAR_TT / 100)
CAR_CO_SCALED = DefineVariable('CAR_CO_SCALED', CAR_CO / 100)

V1 = ASC_TRAIN + B_TIME * TRAIN_TT_SCALED + B_COST * TRAIN_COST_SCALED + EC_TRAIN
V2 = ASC_SM + B_TIME * SM_TT_SCALED + B_COST * SM_COST_SCALED + EC_SM
V3 = ASC_CAR + B_TIME * CAR_TT_SCALED + B_COST * CAR_CO_SCALED + EC_CAR

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
prob = bioLogit(V,av,CHOICE)

# Iterator on individuals, that is on groups of rows.
metaIterator('personIter','__dataFile__','panelObsIter','ID')

# For each item of personIter, iterates on the rows of the group. 
rowIterator('panelObsIter','personIter')


#Conditional probability for the sequence of choices of an individual
condProbIndiv = Prod(prob,'panelObsIter')

# Integration by simulation
probIndiv = MonteCarlo(condProbIndiv)

# Likelihood function
loglikelihood = Sum(log(probIndiv),'personIter')
BIOGEME_OBJECT.ESTIMATE = loglikelihood


# All observations verifying the following expression will not be
# considered for estimation
# The modeler here has developed the model only for work trips.  
# Observations such that the dependent variable CHOICE is 0 are also removed.
exclude = (( PURPOSE != 1 ) * (  PURPOSE   !=  3  ) + ( CHOICE == 0 )) > 0

BIOGEME_OBJECT.EXCLUDE = exclude

BIOGEME_OBJECT.PARAMETERS['NbrOfDraws'] = "5"

#Statistics
numberOfIndividuals = Sum(1,'personIter')
BIOGEME_OBJECT.STATISTICS["Number of individuals"] = numberOfIndividuals
BIOGEME_OBJECT.FORMULAS['Train utility'] = V1
BIOGEME_OBJECT.FORMULAS['Swissmetro utility'] = V2
BIOGEME_OBJECT.FORMULAS['Car utility'] = V3
