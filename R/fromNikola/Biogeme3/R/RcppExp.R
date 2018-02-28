calcStatsR <- function(data)
{
  #This loading of libRTest.dylib must be done during package loading.
  #dyn.load("exec/libRTest.dylib");

  #RCPP interface
  .Call('calcStats', data);
}

calcStatsR2 <- function(data)
{
  #This loading of libRTest.dylib must be done during package loading.
  #dyn.load("exec/libRTest.dylib");

  #RCPP interface
  .Call('calcStats2', data);
}

