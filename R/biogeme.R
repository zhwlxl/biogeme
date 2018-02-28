
biogemeEst <- function(modelFile, dataFile)
{
	size = 5
 coef = runif(5,min=-1,max=1)
 v = create.random.matrix(5,5)
 varCov = t(v) %*% v
 colnames(coef) <- c("beta1","beta2","beta3","beta4","beta5")
 colnames(vcov) <- rownames(vcov) <- colnames(coef)
 list(coefficients = coef, vcov = varCov)
}

