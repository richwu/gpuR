
#include "gpuR/windows_check.hpp"

#include <RcppEigen.h>

#include "gpuR/dynEigenMat.hpp"
#include "gpuR/cl_helpers.hpp"

using namespace cl;
using namespace Rcpp;


//[[Rcpp::export]]
void cpp_gpuMatrix_iaxpy(SEXP alpha_, SEXP ptrA_, SEXP ptrB_,
    SEXP sourceCode_, int device_type)
{
    // declarations
    cl_int err = 0;
    std::string sourceCode = as<std::string>(sourceCode_);
    cl_device_type ocl_device;
    
    #if defined(__APPLE__) || defined(__MACOSX)
        #ifdef HAVE_OPENCL_CL2_HPP
            std::vector<std::string> sourceCodeVec;
            sourceCodeVec.push_back(sourceCode);
        #endif
    #else
        #ifdef HAVE_CL_CL2_HPP
            std::vector<std::string> sourceCodeVec;
            sourceCodeVec.push_back(sourceCode);
        #endif
    #endif
    
    XPtr<dynEigenMat<int> > ptrA(ptrA_);
    XPtr<dynEigenMat<int> > ptrB(ptrB_);
    
    Eigen::Ref<Eigen::MatrixXi> refA = ptrA->data();
    Eigen::Ref<Eigen::MatrixXi> refB = ptrB->data();
    
    Eigen::Map<Eigen::MatrixXi, 0, Eigen::OuterStride<> > Am(
        refA.data(), refA.rows(), refA.cols(),
        Eigen::OuterStride<>(refA.outerStride())
    );
    Eigen::Map<Eigen::MatrixXi, 0, Eigen::OuterStride<> > Bm(
        refB.data(), refB.rows(), refB.cols(),
        Eigen::OuterStride<>(refB.outerStride())
    );
    
    const int N = Am.size();
    const int alpha = as<int>(alpha_);
    
    // Get available platforms
    std::vector<Platform> platforms;
    getPlatforms(platforms); // cl_helpers.hpp       
    
    // Select the default platform and create a context using this platform and the GPU
    cl_context_properties cps[3] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)(platforms[0])(),
        0
    };

    // need to conditionally do CL_DEVICE_TYPE_CPU
    if(device_type == 0){
        ocl_device = CL_DEVICE_TYPE_GPU;
    }else{
        ocl_device = CL_DEVICE_TYPE_CPU;
    }
    
    Context context = createContext(ocl_device, cps, err);
        
    // Get a list of devices on this platform
    std::vector<Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

    if(devices.size() < 1){
        stop("No GPU devices found");
    }
        
    // Create a command queue and use the first device
    CommandQueue queue = CommandQueue(context, devices[0], 0, &err);

    // Read source file - passed in by R wrapper function
    #if defined(__APPLE__) || defined(__MACOSX)
        #ifndef HAVE_OPENCL_CL2_HPP
            int pl;
            std::pair <const char*, int> sourcePair;
            sourcePair = std::make_pair(sourceCode.c_str(), pl);
            Program::Sources source(1, sourcePair);
        #else
            Program::Sources source(sourceCodeVec);
        #endif
    #else
        #ifndef HAVE_CL_CL2_HPP
            int pl;
            std::pair <const char*, int> sourcePair;
            sourcePair = std::make_pair(sourceCode.c_str(), pl);
            Program::Sources source(1, sourcePair);
        #else
            Program::Sources source(sourceCodeVec);
        #endif
    #endif

    // Make program of the source code in the context
    Program program = Program(context, source);
    if (err != CL_SUCCESS)
    {
       stop("Error: Failed to create compute program!\n");
    }

    // Build program for these specific devices
    try
    {
        program.build(devices);
    }
    catch (cl::Error error)
    {
        if (error.err() == CL_BUILD_PROGRAM_FAILURE)
        {
            // Get the build log for the first device
            std::string log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
            stop(log);
        }
        stop("program failed to build");
    }
        
        
    // Make kernel
    Kernel kernel(program, "iaxpy", &err);
//        Kernel kernel(program, kernel_function, &err);
    
    if (err != CL_SUCCESS)
    {
       stop("Error: Failed to create compute kernel!\n");
    }
        
        
//        cl_int wgSize = kernel.getWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);
//        std::cout << "Kernel made" << std::endl;

    // Create memory buffers
    Buffer bufferA = Buffer(context, CL_MEM_READ_ONLY, N * sizeof(int), NULL, &err);
    Buffer bufferB = Buffer(context, CL_MEM_READ_WRITE, N * sizeof(int), NULL, &err);

    // Copy lists A and B to the memory buffers
    queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, N * sizeof(int), Am.data());
    queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, N * sizeof(int), Bm.data());

    // Set arguments to kernel
    err = kernel.setArg(0, alpha);
    err = kernel.setArg(1, bufferA);
    err = kernel.setArg(2, bufferB);
        
    // Run the kernel on specific ND range
//    err = queue.enqueueNDRangeKernel(kernel, NullRange, global, local);
    err = queue.enqueueNDRangeKernel(kernel, NullRange, 
                                    NDRange(N), NullRange);

//        err = queue.enqueueNDRangeKernel(kernel, NullRange, global, NullRange);
        
    // Read buffer C into a local list        
    err = queue.enqueueReadBuffer(bufferB, CL_TRUE, 0, N * sizeof(int), Bm.data());
}
