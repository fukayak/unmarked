#include "tranProbs.h"

using namespace Rcpp ;

SEXP tranProbs( SEXP Kr, SEXP omegaR, SEXP gammaR, SEXP deltaR, SEXP dynamicsR)
{
    IntegerVector K(Kr);
    int lk = K.size();
    double omega = as<double>(omegaR);
    double gamma = as<double>(gammaR);
    int delta = as<int>(deltaR);
    std::string dynamics = as<std::string>(dynamicsR) ;
    arma::mat bpsum(lk, lk);
    
    for(int i=0; i<lk; i++) {
        for(int j=0; j<lk; j++) {
            int Km = std::min(K[i], K[j]);
            IntegerVector cmin0 = seq(0, Km);
            if(dynamics=="autoreg")
                bpsum(i, j) = sum(dbinom(cmin0, K[j], omega, false) *
                    dpois(K[i]-cmin0, gamma*K[j], false));
            else
                bpsum(i, j) = sum(dbinom(cmin0, K[j], omega, false) *
                    dpois(K[i]-cmin0, gamma, false));
            }
        }
    if(delta > 1) {    
        for(int d=1; d<delta; d++) {
            bpsum *= bpsum;
            arma::mat cs = sum(bpsum, 0);
            arma::mat csm = arma::repmat(cs, lk, 1);
            bpsum = bpsum / csm;
            }
        }
    return wrap(bpsum);
}
