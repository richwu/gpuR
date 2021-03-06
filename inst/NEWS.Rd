\name{NEWS}
\title{News for Package 'gpuR'}

\section{Version 1.1.0}{
    \itemize{
        \item New Features:
        \itemize{
            \item Internal class structure simplified
            \item Scalar Arith methods (e.g. 2*X) for gpuMatrix/vclMatrix objects
            \item Unary '-' operator (i.e. -X) for gpuMatrix/vclMatrix objects 
            \item Slice methods to point to gpuVector/vclVector subsets
            \item Block methods to point to gpuMatrix/vclMatrix subsets
            \item deepcopy method to explicitly copy object contents
            \item 'log','log10', & 'exp' methods for vector objects
            \item 'abs' method added for all objects
            \item 'max' & 'min' method for all objects
            \item 'cbind' & 'rbind' methods for gpuMatrix/vclMatrix objects
            \item 'distance' method for pairwise distances
            \item 'squared euclidean' distance added ('sqEuclidean')
            \item 't' method added
        }
        \item Features in Progress:
        \itemize{
            \item Additional matrix functions (QR-decompsition, SVD, Cholesky, solve)
            \item 'device type' checks (i.e. deal with CPU & GPU objects), perhaps detect type from object?
        }
    }
}

\section{Initial Release version 1.0.0}{
    \itemize{
        \item Implemented Features:
        \itemize{
            \item gpu* classes for vector and matrix objects
            \item vcl* classes for matrix objects
            \item Support for 'integer', 'float' and 'double' data types
            \item gpuVector * vclVector functions (addition, subtraction)
            \item gpuMatrix & vclMatrix functions (multiplication, addition, subtraction)
            \item gpuMatrix eigen decomposition (values and vectors)
            \item trig functions for gpu* & vcl* classes
            \item print method for gpuMatrix & vclMatrix objects
            \item dist method gpuMatrix & vclMatrix objects
        }
        \item Features in Progress:
        \itemize{
            \item Additional matrix functions (QR-decompsition, SVD)
        }
    }
}
