[Choice]
CHOICE   

[Beta]
// Name Value  LowerBound UpperBound  status (0=variable, 1=fixed)
ASC_CAR 	0 -10              10              0
ASC_TRAIN  	0 -10              10              0
ASC_SM	        0 -10              10              1
B_TIME		0 -10              10              0
B_COST		0 -10              10              0

[Utilities]
// Id Name     Avail       linear-in-parameter expression (beta1*x1 + beta2*x2 + ... )
    1 A1_TRAIN TRAIN_AV_SP ASC_TRAIN * one 
                            + B_TIME * TRAIN_TT_SCALED 
                            + B_COST * TRAIN_COST_SCALED
    2 A2_SM    SM_AV          ASC_SM * one
                            + B_TIME * SM_TT_SCALED
                            + B_COST * SM_COST_SCALED
    3 A3_Car   CAR_AV_SP     ASC_CAR * one 
                            + B_TIME * CAR_TT_SCALED
                            + B_COST * CAR_CO_SCALED

[Expressions] 
one = 1
CAR_AV_SP =  CAR_AV   * (  SP   !=  0  )
TRAIN_AV_SP =  TRAIN_AV   * (  SP   !=  0  )
SM_COST =  SM_CO   * (  GA   ==  0  ) 
TRAIN_COST =  TRAIN_CO   * (  GA   ==  0  )
TRAIN_TT_SCALED = TRAIN_TT / 100.0
TRAIN_COST_SCALED = TRAIN_COST / 100
SM_TT_SCALED = SM_TT / 100.0
SM_COST_SCALED = SM_COST / 100
CAR_TT_SCALED = CAR_TT / 100
CAR_CO_SCALED = CAR_CO / 100

[Exclude]
(( PURPOSE != 1 ) * (  PURPOSE   !=  3  ) + ( CHOICE == 0 )) 

[Model]
$MNL
