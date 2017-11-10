#set working directory to the location of this script
#setwd("/Users/nikola.obrenovic/switchdrive/RTest");

#First load Rcpp library and then the library that uses Rcpp. 
#If the order is reversed, libRTest.dylib will not load correctly.
library(Rcpp)
dyn.load("librinterface.la");

data <- read.csv("input.csv");
print(data);
.Call('biogeme', data);

