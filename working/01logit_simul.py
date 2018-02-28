#######################################
#
# File: 01logit_simul.py
# Author: Michel Bierlaire, EPFL
# Date: Mon May  7 08:35:00 2012
#
#######################################
#

# This example illustrates how to use pythonbiogeme for simulation,
# based on the 'Enumerate' operator. It includes sensitivity analysis.

#
from biogeme import *
from headers import *
from loglikelihood import *
from statistics import *

ASC_TRAIN = Beta('ASC_TRAIN', -0.701188,-1000,1000,0)
B_TIME = Beta('B_TIME', -1.27786,-1000,1000,0)
B_COST = Beta('B_COST', -1.08379,-1000,1000,0)
ASC_SM = Beta('ASC_SM', 0,-1000,1000,0)
ASC_CAR = Beta('ASC_CAR', -0.154633,-1000,1000,0)

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
TRAIN_TT_SCALED = TRAIN_TT / 100.0
TRAIN_COST_SCALED = TRAIN_COST / 100
SM_TT_SCALED = SM_TT / 100.0
SM_COST_SCALED = SM_COST / 100
CAR_TT_SCALED = CAR_TT / 100
CAR_CO_SCALED = CAR_CO / 100

V1 = ASC_TRAIN + B_TIME * TRAIN_TT_SCALED + B_COST * TRAIN_COST_SCALED
V2 = ASC_SM + B_TIME * SM_TT_SCALED + B_COST * SM_COST_SCALED
V3 = ASC_CAR + B_TIME * CAR_TT_SCALED + B_COST * CAR_CO_SCALED

# Associate utility functions with the numbering of alternatives
V = {1: V1,
     2: V2,
     3: V3}

# Associate the availability conditions with the alternatives

CAR_AV_SP =  CAR_AV  * (  SP   !=  0  )
TRAIN_AV_SP =  TRAIN_AV  * (  SP   !=  0  )

av = {1: TRAIN_AV_SP,
      2: SM_AV,
      3: CAR_AV_SP}

# The choice model is a logit, with availability conditions
prob1 = bioLogit(V,av,1)
prob2 = bioLogit(V,av,2)
prob3 = bioLogit(V,av,3)

# Elasticities can be computed. We illustrate below two
# formulas. Check in the output file that they produce the same
# result.

# First, the general definition of elasticities. This illustrates the
# use of the Derive expression, and can be used with any model,
# however complicated it is. Note the quotes in the Derive opertor.

genelas1 = Derive(prob1,'TRAIN_TT') * TRAIN_TT / prob1
genelas2 = Derive(prob2,'SM_TT') * SM_TT / prob2
genelas3 = Derive(prob3,'CAR_TT') * CAR_TT / prob3

# Second, the elasticity of logit models. See Ben-Akiva and Lerman for
# the formula

logitelas1 = TRAIN_AV_SP * (1.0 - prob1) * TRAIN_TT_SCALED * B_TIME
logitelas2 = SM_AV * (1.0 - prob2) * SM_TT_SCALED * B_TIME
logitelas3 = CAR_AV_SP * (1.0 - prob3) * CAR_TT_SCALED * B_TIME

# Defines an itertor on the data
rowIterator('obsIter') 

# All observations verifying the following expression will not be
# considered for estimation
# The modeler here has developed the model only for work trips.  
# Observations such that the dependent variable CHOICE is 0 are also removed.
exclude = (( PURPOSE != 1 ) * (  PURPOSE   !=  3  ) + ( CHOICE == 0 )) > 0

BIOGEME_OBJECT.EXCLUDE = exclude

simulate = {'Prob. train': prob1,
            'Prob. Swissmetro': prob2,
            'Prob. car':prob3,
            'logit elas. 1':logitelas1,
            'generic elas. 1':genelas1,
            'logit elas. 2':logitelas2,
            'generic elas. 2':genelas2,
            'logit elas. 3':logitelas3,
            'generic elas. 3':genelas3}

# Sensitivity analysis is performed by simulation The estimates of the
# parameters are assumed to follow a multivariate normal distribution,
# with variance-covariance matrix estimated using the robust
# (sandwich) estimator. The matrix can be recuperated from the
# estimation run.

# This vector associates the parameter with the rows and columns in
# the var-covar matrix.
names = ['ASC_TRAIN','B_TIME','B_COST','ASC_CAR']

# The variance covariance maatrix must be symmetric and positive
# definite.  In principle, these values are copied and pasted from the
# results of an estimation run.
values = [[ 0.00301115, -0.00225392,  8.21793e-06,  0.00137693 ],
          [-0.00225392,  0.00323571,  0.000549901, -0.00143774 ],
          [ 8.21793e-06, 0.000549901, 0.00268637,   0.000484763],
          [ 0.00137693, -0.00143774,  0.000484763,  0.00186931 ]]

# The matrix is created and sent to biogeme 
BIOGEME_OBJECT.VARCOVAR = bioMatrix(4,names,values)
BIOGEME_OBJECT.SIMULATE = Enumerate(simulate,'obsIter')

# Statistics

nullLoglikelihood(av,'obsIter')
choiceSet = [1,2,3]
cteLoglikelihood(choiceSet,CHOICE,'obsIter')
availabilityStatistics(av,'obsIter')

BIOGEME_OBJECT.PARAMETERS['DumpSensitivityAnalysis'] = "0"

BIOGEME_OBJECT.FORMULAS['Train utility'] = V1
BIOGEME_OBJECT.FORMULAS['Swissmetro utility'] = V2
BIOGEME_OBJECT.FORMULAS['Car utility'] = V3
