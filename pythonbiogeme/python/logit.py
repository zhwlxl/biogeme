## \file
# Functions for the logit model

from biogeme import *

## Compute the logit choice probability with utilities scaled according to the group
# @ingroup models
# @param V A <a href="http://docs.python.org/py3k/tutorial/datastructures.html#dictionaries" target="_blank">dictionary</a> mapping each alternative id with the
# expression of the utility function.
# @param availability A <a href="http://docs.python.org/py3k/tutorial/datastructures.html#dictionaries" target="_blank">dictionary</a> mapping each alternative id with its availability condition.
# @param choice expression producing the id of the chosen alternative.
# @param group id of the group, or market segment. 
# @param scale A <a href="http://docs.python.org/py3k/tutorial/datastructures.html#dictionaries" target="_blank">dictionary</a> mapping each group id with the expression providing the corresponding scale.
# @return Choice probability for chosen alternative \f$i\f$ and individual in group \f$g\f$: \f[  \frac{e^{\mu_g V_i}}{\sum_j a_j e^{\mu_g V_j}}\f] where \f$a_j\f$ is 1 if alternative \f$j\f$ is available, 0 otherwise, and \f$\mu_g\f$ is the scale parameter associated with group \f$g\f$.
def logit_av_scale(V,availability,choice,group,scale):
    s = Elem(scale,group)
    scaledV = {}
    availBinary = {}
    for k,v in V.items() :
        availBinary[k] = int(availability[k] != 0)
        scaledV[k] = s * v
    P = bioLogit(scaledV,availability,choice) 
    return P

## Compute the logit choice probability, where all alternatives are available.
# @ingroup biogeme
# @ingroup models
# @param V A <a href="http://docs.python.org/py3k/tutorial/datastructures.html#dictionaries" target="_blank">dictionary</a> mapping each alternative id with the
# expression of the utility function.
# @param choice expression producing the id of the chosen alternative.
# @return Choice probability for chosen alternative \f$i\f$ and individual in group \f$g\f$: \f[  \frac{e^{V_i}}{\sum_j e^{V_j}}\f]
def logit(V,choice) :
    avail = {}
    for k,v in V.items() :
        avail[k] = 1 
    P = bioLogit(V,avail,choice)
    return P

## Compute the logit choice probability with utilities scaled according to the group
# @ingroup biogeme
# @ingroup models
# @param V A <a href="http://docs.python.org/py3k/tutorial/datastructures.html#dictionaries" target="_blank">dictionary</a> mapping each alternative id with the
# expression of the utility function.
# @param choice expression producing the id of the chosen alternative.
# @param group id of the group, or market segment. 
# @param scale A <a href="http://docs.python.org/py3k/tutorial/datastructures.html#dictionaries" target="_blank">dictionary</a> mapping each group id with the expression providing the corresponding scale.
# @return Choice probability for chosen alternative \f$i\f$ and individual in group \f$g\f$: \f[  \frac{e^{\mu_g V_i}}{\sum_j e^{\mu_g V_j}}\f] where \f$\mu_g\f$ is the scale parameter associated with group \f$g\f$.
def logit_scale(V,choice,group,scale) :
    s = Elem(scale,group)
    chosen = Elem(V,choice)
    den = 0
    for i,v in V.items() :
        den += exp(s * (v-chosen))
    P = 1.0 / den 
    return P



    
