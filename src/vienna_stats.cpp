
#include "gpuR/windows_check.hpp"

// eigen headers for handling the R input data
#include <RcppEigen.h>

#include "gpuR/dynEigenMat.hpp"
#include "gpuR/dynEigenVec.hpp"
#include "gpuR/dynVCLMat.hpp"
#include "gpuR/dynVCLVec.hpp"

// Use OpenCL with ViennaCL
#define VIENNACL_WITH_OPENCL 1

// Use ViennaCL algorithms on Eigen objects
#define VIENNACL_WITH_EIGEN 1

// ViennaCL headers
#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/linalg/prod.hpp"
#include "viennacl/linalg/sum.hpp"

using namespace Rcpp;

/*** gpuMatrix Templates ***/

template <typename T>
void 
cpp_gpuMatrix_colmean(
    SEXP ptrA_, 
    SEXP ptrC_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }

    XPtr<dynEigenMat<T> > ptrA(ptrA_);
    XPtr<dynEigenVec<T> > ptrC(ptrC_);
    
    Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > colMeans = ptrC->data();
    
    viennacl::matrix<T> vcl_A = ptrA->device_data();
    
    const int K = vcl_A.size1();
    const int V = colMeans.size();
    
    viennacl::vector<T> vcl_colMeans(V);
    
    vcl_colMeans = viennacl::linalg::column_sum(vcl_A);
    vcl_colMeans *= (T)(1)/(T)(K);
    
    viennacl::copy(vcl_colMeans, colMeans);
}

template <typename T>
void 
cpp_gpuMatrix_colsum(
    SEXP ptrA_, SEXP ptrC_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }

    XPtr<dynEigenMat<T> > ptrA(ptrA_);
    XPtr<dynEigenVec<T> > ptrC(ptrC_);
    
    viennacl::matrix<T> vcl_A = ptrA->device_data();
    Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > colSums = ptrC->data();
    
    const int V = colSums.size();
    
    viennacl::vector<T> vcl_colSums(V);
    
    vcl_colSums = viennacl::linalg::column_sum(vcl_A);
    
    viennacl::copy(vcl_colSums, colSums);
}

template <typename T>
void 
cpp_gpuMatrix_rowmean(
    SEXP ptrA_, SEXP ptrC_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }
    XPtr<dynEigenMat<T> > ptrA(ptrA_);
    XPtr<dynEigenVec<T> > ptrC(ptrC_);
    
    Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > rowMeans = ptrC->data();
    
    viennacl::matrix<T> vcl_A = ptrA->device_data();
    
    const int M = vcl_A.size2();
    const int V = rowMeans.size();
    
    viennacl::vector<T> vcl_rowMeans(V);
    
    vcl_rowMeans = viennacl::linalg::row_sum(vcl_A);
    vcl_rowMeans *= (T)(1)/(T)(M);
    
    viennacl::copy(vcl_rowMeans, rowMeans);
}

template <typename T>
void
cpp_gpuMatrix_rowsum(
    SEXP ptrA_, SEXP ptrC_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }
    
    XPtr<dynEigenMat<T> > ptrA(ptrA_);
    XPtr<dynEigenVec<T> > ptrC(ptrC_);
    
    Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > rowSums = ptrC->data();
    
    viennacl::matrix<T> vcl_A = ptrA->device_data();
    
    const int V = rowSums.size();
    
    viennacl::vector<T> vcl_rowSums(V);
    
    vcl_rowSums = viennacl::linalg::row_sum(vcl_A);
    
    viennacl::copy(vcl_rowSums, rowSums);
}

/*** vclMatrix Templates ***/

template <typename T>
void 
cpp_vclMatrix_colmean(
    SEXP ptrA_, 
    SEXP ptrC_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }


    Rcpp::XPtr<dynVCLMat<T> > ptrA(ptrA_);
    Rcpp::XPtr<dynVCLVec<T> > pC(ptrC_);
    
    viennacl::vector_range<viennacl::vector<T> > vcl_colMeans  = pC->data();
    viennacl::matrix_range<viennacl::matrix<T> > vcl_A = ptrA->data();
    
//    viennacl::matrix<T> &vcl_A = *ptrA;
//    viennacl::vector<T> &vcl_colMeans = *ptrC;
    
    const int K = vcl_A.size1();
        
    vcl_colMeans = viennacl::linalg::column_sum(vcl_A);
    vcl_colMeans *= (T)(1)/(T)(K);
}

template <typename T>
void 
cpp_vclMatrix_colsum(
    SEXP ptrA_, SEXP ptrC_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }
    
    Rcpp::XPtr<dynVCLMat<T> > ptrA(ptrA_);
    Rcpp::XPtr<dynVCLVec<T> > pC(ptrC_);
    
    viennacl::vector_range<viennacl::vector<T> > vcl_colSums  = pC->data();
    viennacl::matrix_range<viennacl::matrix<T> > vcl_A = ptrA->data();
    
//    viennacl::matrix<T> &vcl_A = *ptrA;
//    viennacl::vector<T> &vcl_colSums = *ptrC;
    
    vcl_colSums = viennacl::linalg::column_sum(vcl_A);
}

template <typename T>
void 
cpp_vclMatrix_rowmean(
    SEXP ptrA_, SEXP ptrC_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }

    Rcpp::XPtr<dynVCLMat<T> > ptrA(ptrA_);
    Rcpp::XPtr<dynVCLVec<T> > pC(ptrC_);
    viennacl::vector_range<viennacl::vector<T> > vcl_rowMeans  = pC->data();
    viennacl::matrix_range<viennacl::matrix<T> > vcl_A = ptrA->data();
    
//    viennacl::matrix<T> &vcl_A = *ptrA;
//    viennacl::vector<T> &vcl_rowMeans = *ptrC;

    const int M = vcl_A.size2();
    
    vcl_rowMeans = viennacl::linalg::row_sum(vcl_A);
    vcl_rowMeans *= (T)(1)/(T)(M);
}

template <typename T>
void
cpp_vclMatrix_rowsum(
    SEXP ptrA_, SEXP ptrC_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }

    Rcpp::XPtr<dynVCLMat<T> > ptrA(ptrA_);
    Rcpp::XPtr<dynVCLVec<T> > pC(ptrC_);
    viennacl::vector_range<viennacl::vector<T> > vcl_rowSums  = pC->data();
    viennacl::matrix_range<viennacl::matrix<T> > vcl_A = ptrA->data();
    
//    viennacl::matrix<T> &vcl_A = *ptrA;
//    viennacl::vector<T> &vcl_rowSums = *ptrC;
    
    vcl_rowSums = viennacl::linalg::row_sum(vcl_A);
}

template <typename T>
void 
cpp_gpuMatrix_pmcc(
    SEXP ptrA_, 
    SEXP ptrB_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }
    
    XPtr<dynEigenMat<T> > ptrA(ptrA_);
    XPtr<dynEigenMat<T> > ptrB(ptrB_);
    
    viennacl::matrix<T> vcl_A = ptrA->device_data();
    
    const int K = vcl_A.size1();
    const int M = vcl_A.size2();
    
    viennacl::vector<T> ones = viennacl::scalar_vector<T>(K, 1);
    viennacl::vector<T> vcl_meanVec(M);
    viennacl::matrix<T> vcl_meanMat(K,M);
    
    // vector of column means
    vcl_meanVec = viennacl::linalg::column_sum(vcl_A);
    vcl_meanVec *= (T)(1)/(T)(K);
    
    // matrix of means
    vcl_meanMat = viennacl::linalg::outer_prod(ones, vcl_meanVec);
    
    viennacl::matrix<T> tmp = vcl_A - vcl_meanMat;
    
    // calculate pearson covariance
    viennacl::matrix<T> vcl_B = viennacl::linalg::prod(trans(tmp), tmp);
    vcl_B *= (T)(1)/(T)(K-1);
    
    ptrB->to_host(vcl_B);
}

template <typename T>
void 
cpp_vclMatrix_pmcc(
    SEXP ptrA_, 
    SEXP ptrB_,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }
//    
//    viennacl::matrix<T> &vcl_A = *ptrA;
//    viennacl::matrix<T> &vcl_B = *ptrB;
    
    Rcpp::XPtr<dynVCLMat<T> > ptrA(ptrA_);
    Rcpp::XPtr<dynVCLMat<T> > ptrB(ptrB_);
    
    viennacl::matrix_range<viennacl::matrix<T> > vcl_A = ptrA->data();
    viennacl::matrix_range<viennacl::matrix<T> > vcl_B = ptrB->data();
    
    const int M = vcl_A.size2();
    const int K = vcl_A.size1();
    
    viennacl::vector<T> ones = viennacl::scalar_vector<T>(K, 1);
    viennacl::vector<T> vcl_meanVec(M);
    viennacl::matrix<T> vcl_meanMat(K,M);
    
    // vector of column means
    vcl_meanVec = viennacl::linalg::column_sum(vcl_A);
    vcl_meanVec *= (T)(1)/(T)(K);
    
    // matrix of means
    vcl_meanMat = viennacl::linalg::outer_prod(ones, vcl_meanVec);
    
    viennacl::matrix<T> tmp = vcl_A - vcl_meanMat;
    
    // calculate pearson covariance
    vcl_B = viennacl::linalg::prod(trans(tmp), tmp);
    vcl_B *= (T)(1)/(T)(K-1);
}


template <typename T>
void 
cpp_gpuMatrix_eucl(
    SEXP ptrA_, 
    SEXP ptrD_,
    bool squareDist,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }
    
    XPtr<dynEigenMat<T> > ptrA(ptrA_);
    XPtr<dynEigenMat<T> > ptrD(ptrD_);
    
    viennacl::matrix<T> vcl_A = ptrA->device_data();
    viennacl::matrix<T> vcl_D;
    
    // other temp objects
    viennacl::matrix<T> twos;
    viennacl::matrix<T> square_A;
    
    const int K = vcl_A.size1();
      
    // temp objects
    viennacl::vector<T> row_ones = viennacl::scalar_vector<T>(K, 1);
    viennacl::vector<T> vcl_sqrt = viennacl::zero_vector<T>(K);
    
    // this will definitely need to be updated with the next ViennaCL release
    // currently doesn't support the single scalar operation with
    // element_pow below
    twos = viennacl::scalar_matrix<T>(K, K, 2);
    
    square_A = viennacl::linalg::element_pow(vcl_A, twos);
    vcl_sqrt = viennacl::linalg::row_sum(square_A);
    
    vcl_D = viennacl::linalg::outer_prod(vcl_sqrt, row_ones);
    
    vcl_D += trans(vcl_D);
    
    vcl_D -= 2 * (viennacl::linalg::prod(vcl_A, trans(vcl_A)));
    
    if(!squareDist){
        vcl_D = viennacl::linalg::element_sqrt(vcl_D);    
    }
    
    for(unsigned int i=0; i < vcl_D.size1(); i++){
        vcl_D(i,i) = 0;
    }
    
    ptrD->to_host(vcl_D);
}

template <typename T>
void 
cpp_gpuMatrix_peucl(
    SEXP ptrA_, 
    SEXP ptrB_,
    SEXP ptrD_,
    bool squareDist,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }    
    
    XPtr<dynEigenMat<T> > ptrA(ptrA_);
    XPtr<dynEigenMat<T> > ptrB(ptrB_);
    XPtr<dynEigenMat<T> > ptrD(ptrD_);
    
    // copy to GPU
    viennacl::matrix<T> vcl_A = ptrA->device_data();
    viennacl::matrix<T> vcl_B = ptrB->device_data();
    viennacl::matrix<T> vcl_D;
    
    const int M = vcl_A.size2();
    const int K = vcl_A.size1();
    const int R = vcl_B.size2();
    const int Q = vcl_B.size1();
    
    viennacl::vector<T> A_row_ones = viennacl::scalar_vector<T>(K, 1);
    viennacl::vector<T> B_row_ones = viennacl::scalar_vector<T>(Q, 1);
    
    viennacl::matrix<T> square_A;
    viennacl::matrix<T> square_B;
    
    // this will definitely need to be updated with the next ViennaCL release
    // currently doesn't support the single scalar operation with
    // element_pow below
    {
        viennacl::matrix<T> twos = viennacl::scalar_matrix<T>(std::max(K, Q), std::max(M, R), 2);
    
        square_A = viennacl::linalg::element_pow(vcl_A, twos);
        square_B = viennacl::linalg::element_pow(vcl_B, twos);
    }
    
    {
        viennacl::vector<T> vcl_A_rowsum = viennacl::zero_vector<T>(K);
        viennacl::vector<T> vcl_B_rowsum = viennacl::zero_vector<T>(Q);
        
        vcl_A_rowsum = viennacl::linalg::row_sum(square_A);
        vcl_B_rowsum = viennacl::linalg::row_sum(square_B);
        
        viennacl::matrix<T> vclXX = viennacl::linalg::outer_prod(vcl_A_rowsum, B_row_ones);
        viennacl::matrix<T> vclYY = viennacl::linalg::outer_prod(A_row_ones, vcl_B_rowsum);
        
        vcl_D = vclXX + vclYY;
        vcl_D -= 2 * (viennacl::linalg::prod(vcl_A, trans(vcl_B)));
    }
    
    if(!squareDist){
        vcl_D = viennacl::linalg::element_sqrt(vcl_D);    
    }
    
    ptrD->to_host(vcl_D);
        
}

template <typename T>
void 
cpp_vclMatrix_eucl(
    SEXP ptrA_, 
    SEXP ptrD_,
    bool squareDist,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }
//    
//    viennacl::matrix<T> &vcl_A = *ptrA;
//    viennacl::matrix<T> &vcl_D = *ptrD;
    
    
    Rcpp::XPtr<dynVCLMat<T> > ptrA(ptrA_);
    Rcpp::XPtr<dynVCLMat<T> > ptrD(ptrD_);
    
    viennacl::matrix_range<viennacl::matrix<T> > vcl_A = ptrA->data();
    viennacl::matrix_range<viennacl::matrix<T> > vcl_D = ptrD->data();
    
//    std::cout << "pulled data" << std::endl;
//    std::cout << vcl_A.size1() << std::endl;
    
//    viennacl::vector<T> vcl_sqrt = viennacl::zero_vector<T>(vcl_A.size1());
    viennacl::vector<T> vcl_sqrt;
    
//    std::cout << "row of zeros" << std::endl;
    
    // this will definitely need to be updated with the next ViennaCL release
    // currently doesn't support the single scalar operation with
    // element_pow below
    {
        viennacl::matrix<T> twos = viennacl::scalar_matrix<T>(vcl_A.size1(), vcl_A.size2(), 2);
    
//        std::cout << "create 'twos' matrix" << std::endl;
        
        viennacl::matrix<T> square_A = viennacl::linalg::element_pow(vcl_A, twos);
        vcl_sqrt = viennacl::linalg::row_sum(square_A);
    }
    
    
//    std::cout << "powers and rowsum completed" << std::endl;
    
    {
        viennacl::vector<T> row_ones = viennacl::scalar_vector<T>(vcl_A.size1(), 1);
        
//        std::cout << "row of ones" << std::endl;
        
        vcl_D = viennacl::linalg::outer_prod(vcl_sqrt, row_ones);
    }
    
//    std::cout << "outer product completed" << std::endl;
    
//    std::cout << vcl_D << std::endl;
    
    vcl_D += trans(vcl_D);
    
//    std::cout << vcl_D << std::endl;
    
//    viennacl::matrix<T> temp = 2 * (viennacl::linalg::prod(vcl_A, trans(vcl_A)));
    
//    std::cout << temp << std::endl;
    vcl_D -= 2 * (viennacl::linalg::prod(vcl_A, trans(vcl_A)));
//    vcl_D -= temp;
    if(!squareDist){
        vcl_D = viennacl::linalg::element_sqrt(vcl_D);    
    }
    
    for(unsigned int i=0; i < vcl_D.size1(); i++){
        vcl_D(i,i) = 0;
    }
//    viennacl::diag(vcl_D) = viennacl::zero_vector<T>(vcl_A.size1());
        
}

template <typename T>
void 
cpp_vclMatrix_peucl(
    SEXP ptrA_, 
    SEXP ptrB_,
    SEXP ptrD_,
    bool squareDist,
    int device_flag)
{
    // define device type to use
    if(device_flag == 0){
        //use only GPUs
        long id = 0;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::gpu_tag());
        viennacl::ocl::switch_context(id);
    }else{
        // use only CPUs
        long id = 1;
        viennacl::ocl::set_context_device_type(id, viennacl::ocl::cpu_tag());
        viennacl::ocl::switch_context(id);
    }    
    
    Rcpp::XPtr<dynVCLMat<T> > ptrA(ptrA_);
    Rcpp::XPtr<dynVCLMat<T> > ptrB(ptrB_);
    Rcpp::XPtr<dynVCLMat<T> > ptrD(ptrD_);
    
    viennacl::matrix_range<viennacl::matrix<T> > vcl_A = ptrA->data();
    viennacl::matrix_range<viennacl::matrix<T> > vcl_B = ptrB->data();
    viennacl::matrix_range<viennacl::matrix<T> > vcl_D = ptrD->data();
    
    viennacl::matrix<T> square_A;
    viennacl::matrix<T> square_B;
    
//    std::cout << "pulled data" << std::endl;
    
    // this will definitely need to be updated with the next ViennaCL release
    // currently doesn't support the single scalar operation with
    // element_pow below
    {
        viennacl::matrix<T> twos = viennacl::scalar_matrix<T>(std::max(vcl_A.size1(), vcl_B.size1()), std::max(vcl_A.size2(), vcl_B.size2()), 2);
    
        square_A = viennacl::linalg::element_pow(vcl_A, twos);
        square_B = viennacl::linalg::element_pow(vcl_B, twos);
    }
    
//    std::cout << "power calculation complete" << std::endl;
    
    {
        viennacl::vector<T> x_row_ones = viennacl::scalar_vector<T>(vcl_A.size1(), 1);
        viennacl::vector<T> y_row_ones = viennacl::scalar_vector<T>(vcl_B.size1(), 1);
        
        viennacl::vector<T> vcl_A_rowsum = viennacl::zero_vector<T>(vcl_A.size1());
        viennacl::vector<T> vcl_B_rowsum = viennacl::zero_vector<T>(vcl_B.size1());
        
        vcl_A_rowsum = viennacl::linalg::row_sum(square_A);
        vcl_B_rowsum = viennacl::linalg::row_sum(square_B);
        
//        std::cout << "row sums complete" << std::endl;
        
        viennacl::matrix<T> vclXX = viennacl::linalg::outer_prod(vcl_A_rowsum, y_row_ones);
        //std::cout << vclXX << std::endl;
        viennacl::matrix<T> vclYY = viennacl::linalg::outer_prod(x_row_ones, vcl_B_rowsum);
//        std::cout << "outer products complete" << std::endl;
        //    std::cout << vclYY << std::endl;
        vcl_D = vclXX + vclYY;
    }
    
    
    vcl_D -= 2 * (viennacl::linalg::prod(vcl_A, trans(vcl_B)));
    
    if(!squareDist){
        vcl_D = viennacl::linalg::element_sqrt(vcl_D);    
    }
    
//    for(unsigned int i=0; i < vcl_D.size1(); i++){
//        vcl_D(i,i) = 0;
//    }
//    viennacl::diag(vcl_D) = viennacl::zero_vector<T>(vcl_A.size1());
        
}

// [[Rcpp::export]]
void
cpp_gpuMatrix_pmcc(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_gpuMatrix_pmcc<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_gpuMatrix_pmcc<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_gpuMatrix_pmcc<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for gpuMatrix object!");
    }
}

// [[Rcpp::export]]
void
cpp_vclMatrix_pmcc(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_vclMatrix_pmcc<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_vclMatrix_pmcc<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_vclMatrix_pmcc<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for vclMatrix object!");
    }
}




// [[Rcpp::export]]
void
cpp_vclMatrix_eucl(
    SEXP ptrA, SEXP ptrD,
    bool squareDist,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_vclMatrix_eucl<int>(ptrA, ptrD, squareDist, device_flag);
            return;
        case 6:
            cpp_vclMatrix_eucl<float>(ptrA, ptrD, squareDist, device_flag);
            return;
        case 8:
            cpp_vclMatrix_eucl<double>(ptrA, ptrD, squareDist, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for vclMatrix object!");
    }
}

// [[Rcpp::export]]
void
cpp_vclMatrix_peucl(
    SEXP ptrA, SEXP ptrB, SEXP ptrD,
    bool squareDist,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_vclMatrix_peucl<int>(ptrA, ptrB, ptrD, squareDist, device_flag);
            return;
        case 6:
            cpp_vclMatrix_peucl<float>(ptrA, ptrB, ptrD, squareDist, device_flag);
            return;
        case 8:
            cpp_vclMatrix_peucl<double>(ptrA, ptrB, ptrD, squareDist, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for vclMatrix object!");
    }
}

// [[Rcpp::export]]
void
cpp_gpuMatrix_eucl(
    SEXP ptrA, SEXP ptrD,
    bool squareDist,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_gpuMatrix_eucl<int>(ptrA, ptrD, squareDist, device_flag);
            return;
        case 6:
            cpp_gpuMatrix_eucl<float>(ptrA, ptrD, squareDist, device_flag);
            return;
        case 8:
            cpp_gpuMatrix_eucl<double>(ptrA, ptrD, squareDist, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for gpuMatrix object!");
    }
}

// [[Rcpp::export]]
void
cpp_gpuMatrix_peucl(
    SEXP ptrA, SEXP ptrB, SEXP ptrD,
    bool squareDist,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_gpuMatrix_peucl<int>(ptrA, ptrB, ptrD, squareDist, device_flag);
            return;
        case 6:
            cpp_gpuMatrix_peucl<float>(ptrA, ptrB, ptrD, squareDist, device_flag);
            return;
        case 8:
            cpp_gpuMatrix_peucl<double>(ptrA, ptrB, ptrD, squareDist, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for gpuMatrix object!");
    }
}

/*** gpuMatrix Functions ***/

// [[Rcpp::export]]
void
cpp_gpuMatrix_colmean(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_gpuMatrix_colmean<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_gpuMatrix_colmean<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_gpuMatrix_colmean<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for gpuMatrix object!");
    }
}

// [[Rcpp::export]]
void
cpp_gpuMatrix_colsum(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_gpuMatrix_colsum<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_gpuMatrix_colsum<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_gpuMatrix_colsum<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for gpuMatrix object!");
    }
}

// [[Rcpp::export]]
void
cpp_gpuMatrix_rowmean(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_gpuMatrix_rowmean<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_gpuMatrix_rowmean<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_gpuMatrix_rowmean<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for gpuMatrix object!");
    }
}


// [[Rcpp::export]]
void
cpp_gpuMatrix_rowsum(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_gpuMatrix_rowsum<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_gpuMatrix_rowsum<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_gpuMatrix_rowsum<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for gpuMatrix object!");
    }
}

/*** vclMatrix Functions ***/

// [[Rcpp::export]]
void
cpp_vclMatrix_colmean(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_vclMatrix_colmean<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_vclMatrix_colmean<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_vclMatrix_colmean<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for vclMatrix object!");
    }
}

// [[Rcpp::export]]
void
cpp_vclMatrix_colsum(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_vclMatrix_colsum<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_vclMatrix_colsum<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_vclMatrix_colsum<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for vclMatrix object!");
    }
}

// [[Rcpp::export]]
void
cpp_vclMatrix_rowmean(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_vclMatrix_rowmean<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_vclMatrix_rowmean<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_vclMatrix_rowmean<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for vclMatrix object!");
    }
}


// [[Rcpp::export]]
void
cpp_vclMatrix_rowsum(
    SEXP ptrA, SEXP ptrB,
    int device_flag,
    const int type_flag)
{
    
    switch(type_flag) {
        case 4:
            cpp_vclMatrix_rowsum<int>(ptrA, ptrB, device_flag);
            return;
        case 6:
            cpp_vclMatrix_rowsum<float>(ptrA, ptrB, device_flag);
            return;
        case 8:
            cpp_vclMatrix_rowsum<double>(ptrA, ptrB, device_flag);
            return;
        default:
            throw Rcpp::exception("unknown type detected for vclMatrix object!");
    }
}


