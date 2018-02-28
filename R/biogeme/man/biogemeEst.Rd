\name{biogemeEst}
\alias{biogemeEst}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
%%  ~~function to do ... ~~
}
\description{
%%  ~~ A concise (1-5 lines) description of what the function does. ~~
}
\usage{
biogemeEst(modelFile, dataFile)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{modelFile}{
%%     ~~Describe \code{modelFile} here~~
}
  \item{dataFile}{
%%     ~~Describe \code{dataFile} here~~
}
}
\details{
%%  ~~ If necessary, more details than the description above ~~
}
\value{
%%  ~Describe the value returned
%%  If it is a LIST, use
%%  \item{comp1 }{Description of 'comp1'}
%%  \item{comp2 }{Description of 'comp2'}
%% ...
}
\references{
%% ~put references to the literature/web site here ~
}
\author{
%%  ~~who you are~~
}
\note{
%%  ~~further notes~~
}

%% ~Make other sections like Warning with \section{Warning }{....} ~

\seealso{
%% ~~objects to See Also as \code{\link{help}}, ~~~
}
\examples{
##---- Should be DIRECTLY executable !! ----
##-- ==>  Define data, use random,
##--	or do  help(data=index)  for the standard data sets.

## The function is currently defined as
function (modelFile, dataFile) 
{
    size = 5
    coef = runif(5, min = -1, max = 1)
    v = create.random.matrix(5, 5)
    varCov = t(v) \%*\% v
    colnames(coef) <- c("beta1", "beta2", "beta3", "beta4", "beta5")
    colnames(vcov) <- rownames(vcov) <- colnames(coef)
    list(coefficients = coef, vcov = varCov)
  }
}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ ~kwd1 }% use one of  RShowDoc("KEYWORDS")
\keyword{ ~kwd2 }% __ONLY ONE__ keyword per line
