calcStatsR <- function(data)
{
  #RCPP interface
  .Call('calcStats', data);
}

runBiogeme <- function(data)
{
    #RCPP interface
    .Call('runBiogeme', data);
}
