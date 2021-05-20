#include "TokenBucketAlgorithm.h"


int main(int argc, char* argv[])
{
    uint64_t b_sz, b_tr, q_sz, q_par, p_mxsz;

    if (argc == 6) {
        b_sz = atoi(argv[1]);
        b_tr = atoi(argv[2]);
        q_sz = atoi(argv[3]);
        q_par = atoi(argv[4]);
        p_mxsz = atoi(argv[5]);
    }
    else {
        std::cout << "Usage: " << argv[0] << " [bucket maximum size]" <<
            "[bucket tocken rate] [queue size] [queue maximum packet arrival rate]" <<
            "[packets maximum size]\n\n";

        std::cout << "Maximum packet arrival rate :\t"; std::cin >> q_par;
        std::cout << "Maximum packet size :\t"; std::cin >> p_mxsz;
        std::cout << "Size of the queue :\t"; std::cin >> q_sz;
        std::cout << "Token rate :\t"; std::cin >> b_tr;
        std::cout << "Bucket capacity :\t"; std::cin >> b_sz;
    }


    TokenBucketAlgorithm tba(b_sz, b_tr, q_sz, q_par, p_mxsz);
#ifdef DEBUG
    std::cout << "Class created\n";
#endif

    tba.Run();
    tba.print();

    return 0;
}
