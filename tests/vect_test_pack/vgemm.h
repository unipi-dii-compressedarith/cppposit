template <class T>
void fallbackGemm(std::vector<T>& A,std::vector<T>& B,std::vector<T>& C,int M,int N,int K);

template <class T>
std::vector<unsigned long int> testGemm(vectorized::PositVectorizedFrontend<T>* fv, int runs,int M,int N,int K) {
	std::vector<unsigned long int> times;
	std::vector<T> out(M*N);
	for(int i = 0; i < runs; ++i) {
		auto data1 = genMatrix<T>(M,K);
		auto data2 = genMatrix<T>(K,N);
		auto start = get_time::now();
		(fv != nullptr)?fv->vGemm(data1,data2,out,M,N,K):fallbackGemm<T>(data1,data2,out,M,N,K);
		auto end = get_time::now();
        if(fv == nullptr) std::cout << std::chrono::duration_cast<ns>(end - start).count() << std::endl;		
		//times.push_back(std::chrono::duration_cast<ns>(end - start).count());
	}
	return times;
}

#ifdef _EPI_
template <class T>
std::vector<unsigned long int> testGemm4(vectorized::PositRVVBackend<T>& fv, int runs,int M,int N,int K) {
	std::vector<unsigned long int> times;
	std::vector<T> out(M*N);
	for(int i = 0; i < runs; ++i) {
		auto data1 = genMatrix<T>(M,K);
		auto data2 = genMatrix<T>(K,N);
		auto start = get_time::now();
		fv.rvvGemmX_next_8(data1,data2,out,M,N,K);
		auto end = get_time::now();
		//times.push_back(std::chrono::duration_cast<ns>(end - start).count());
	}
	return times;
}
#endif


template <class T>
void fallbackGemm(std::vector<T>& A,std::vector<T>& B,std::vector<T>& C,int M,int N,int K) {
        for (int i = 0; i < M; i ++)
                for (int j = 0; j < N; j ++) {
                        C[i * N + j] = 0;
                        for (int k = 0; k < K; k ++)
                                C[i * N + j] += A[i * K + k] * B[k * N + j];
                        }
}
