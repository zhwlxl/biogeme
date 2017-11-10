# This file has automatically been generated.
# Tue Apr 21 09:36:59 2015
# Michel Bierlaire, EPFL
# biogeme 2.4 [Lun 19 jan 2015 18:40:42 CET]
# Michel Bierlaire, EPFL

#####################################################
# This file complies with the syntax of pythonbiogeme
# In general, it may require to be edited by hand before being operational
# It is meant to help users translating their models from the previous version of biogeme to the python version.
#####################################################

from biogeme import *
from headers import *
from loglikelihood import *
from statistics import *
  
# [Choice]
__chosenAlternative = CHOICE 

# [Weight]
# NONE

#[Beta]
#Parameters to be estimated
# Arguments:
#   1  Name for report. Typically, the same as the variable
#   2  Starting value
#   3  Lower bound
#   4  Upper bound
#   5  0: estimate the parameter, 1: keep it fixed
ASC_CAR	 = Beta('ASC_CAR',0,-10,10,0)
ASC_SM	 = Beta('ASC_SM',0,-10,10,1)
ASC_TRAIN	 = Beta('ASC_TRAIN',0,-10,10,0)
B_COST	 = Beta('B_COST',0,-10,10,0)
B_TIME	 = Beta('B_TIME',0,-10,10,0)







# [Expressions] 
# Define here arithmetic expressions for name that are not directly 
# available from the data

one  = DefineVariable('one',1)
CAR_AV_SP  = DefineVariable('CAR_AV_SP', CAR_AV   * (  SP   !=  0  ))
TRAIN_AV_SP  = DefineVariable('TRAIN_AV_SP', TRAIN_AV   * (  SP   !=  0  ))
SM_COST  = DefineVariable('SM_COST', SM_CO   * (  GA   ==  0  ))
TRAIN_COST  = DefineVariable('TRAIN_COST', TRAIN_CO   * (  GA   ==  0  ))
TRAIN_TT_SCALED  = DefineVariable('TRAIN_TT_SCALED', TRAIN_TT   /  100 )
TRAIN_COST_SCALED  = DefineVariable('TRAIN_COST_SCALED', TRAIN_COST   /  100 )
SM_TT_SCALED  = DefineVariable('SM_TT_SCALED', SM_TT   /  100 )
SM_COST_SCALED  = DefineVariable('SM_COST_SCALED', SM_COST   /  100 )
CAR_TT_SCALED  = DefineVariable('CAR_TT_SCALED', CAR_TT   /  100 )
CAR_CO_SCALED  = DefineVariable('CAR_CO_SCALED', CAR_CO   /  100 )

#[Group]

#[Utilities]
__A1_TRAIN = ASC_TRAIN * one + B_TIME * TRAIN_TT_SCALED + B_COST * TRAIN_COST_SCALED
__A2_SM = ASC_SM * one + B_TIME * SM_TT_SCALED + B_COST * SM_COST_SCALED
__A3_Car = ASC_CAR * one + B_TIME * CAR_TT_SCALED + B_COST * CAR_CO_SCALED
__V = {1: __A1_TRAIN,2: __A2_SM,3: __A3_Car}
__av = {1: TRAIN_AV_SP,2: SM_AV,3: CAR_AV_SP}





#[Draws]
BIOGEME_OBJECT.PARAMETERS['NbrOfDraws'] = "150"
#[Exclude]
BIOGEME_OBJECT.EXCLUDE = ( (  PURPOSE   !=  1  ) * (  PURPOSE   !=  3  ) ) + (  CHOICE   ==  0  )








#[Model]
# MNL  // Logit Model
# The choice model is a logit, with availability conditions
prob = bioLogit(__V,__av,__chosenAlternative)
__l = log(prob)

# Defines an itertor on the data
rowIterator('obsIter') 

# Define the likelihood function for the estimation
BIOGEME_OBJECT.ESTIMATE = Sum(__l,'obsIter')

