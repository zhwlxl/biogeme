#set working directory to the location of this script
setwd("/Users/nikola.obrenovic/switchdrive/Projects/RCPP/code/PackageTest");

library(Rcpp)
library(Biogeme3)

data <- read.csv("input.csv");
print(data);

#call Biogeme package
Biogeme3::calcStatsR2(data);
