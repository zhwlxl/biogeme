#set working directory to the location of this script
#setwd("/Users/nikola.obrenovic/switchdrive/Projects/RCPP/code/PackageTest");

library(Rcpp)
library(libbiogemedylib)

data <- read.csv("swissmetro.dat");
print(data);

#call Biogeme package
#Biogeme3::calcStatsR2(data);
