// Armadillo headers (disable BLAS and LAPACK to avoid linking issues)
#define ARMA_DONT_USE_BLAS
#define ARMA_DONT_USE_LAPACK

// armadillo headers for handling the R input data
#include <RcppArmadillo.h>

// Use OpenCL with ViennaCL
#define VIENNACL_WITH_OPENCL 1

// Use ViennaCL algorithms on Armadillo objects
#define VIENNACL_WITH_ARMADILLO 1

// ViennaCL headers
#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"
#include "viennacl/vector.hpp"

using namespace Rcpp;

template <typename T>
inline
arma::Col<T> cpp_arma_vienna_vec_axpy(
    T const alpha, 
    const arma::Col<T> &Am, 
    const arma::Col<T> &Bm)
{    
    //use only GPUs:
    long id = 0;
    viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
    
    int M = Am.n_elem;
    
    viennacl::vector<T> vcl_A(M);
    viennacl::vector<T> vcl_B(M);
    viennacl::vector<T> vcl_C(M);
    
    arma::Mat<T> Cm = arma::Col<T>(M);
    
    viennacl::copy(Am, vcl_A); 
    viennacl::copy(Bm, vcl_B); 
    
    vcl_C = alpha * vcl_A + vcl_B;
    
    viennacl::copy(vcl_C, Cm);
    
    return Cm;
}