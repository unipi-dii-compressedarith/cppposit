template <class T>
void fallbackConv(std::vector<T>& A,std::vector<T>& F,std::vector<T>& O,int R,int C);

template <class T>
std::vector<unsigned long int> testConv(vectorized::PositVectorizedFrontend<T>* fv, int runs,int matrixRows,int matrixCols) {
	std::vector<unsigned long int> times;
	std::vector<T> out((matrixRows-2)*(matrixCols-2));
	for(int i = 0; i < runs; ++i) {
		auto data = genMatrix<T>(matrixRows,matrixCols);
		auto filter = genMatrix<T>(3,3);
		auto start = get_time::now();
		(fv != nullptr)?fv->sv_conv3x3(data,filter,out,matrixRows,matrixCols):fallbackConv<T>(data,filter,out,matrixRows,matrixCols);
		auto end = get_time::now();
        if(fv == nullptr) std::cout << std::chrono::duration_cast<ns>(end - start).count() << std::endl;		
        //times.push_back(std::chrono::duration_cast<ns>(end - start).count());
	}
	return times;
}
#ifdef _EPI_
template <class T>
std::vector<unsigned long int> testConv5(vectorized::PositRVVBackend<T>& fv, int runs,int matrixRows,int matrixCols) {
	std::vector<unsigned long int> times;
	T* out = new T[((matrixRows-2)*(matrixCols-2))];
	for(int i = 0; i < runs; ++i) {
		auto data = genMatrix<T>(matrixRows,matrixCols);
		auto filter = genMatrix<T>(3,3);
		auto start = get_time::now();
		fv.rvv_conv5x5(data.data(),filter.data(),out,matrixRows,matrixCols);
		auto end = get_time::now();
	}
	return times;
}

template <class T>
std::vector<unsigned long int> testConv3X(vectorized::PositRVVBackend<T>& fv, int runs,int matrixRows,int matrixCols) {
	std::vector<unsigned long int> times;
	std::vector<T> out((matrixRows-2)*(matrixCols-2));
	for(int i = 0; i < runs; ++i) {
		auto data = genMatrix<T>(matrixRows,matrixCols);
		auto filter = genMatrix<T>(3,3);
		auto start = get_time::now();
		fv.vConv3X(data,filter,out,matrixRows,matrixCols);
		auto end = get_time::now();
	}
	return times;
}
#endif
template <class T>
void fallbackConv(std::vector<T>& A,std::vector<T>& F,std::vector<T>& O,int R,int C) {
        // Loop on rows
        for(int i = 0; i < R-2;++i) {

                // Loop on columns
                for(int j = 0; j < C-2; ++j) {

                        T accum(0);

                        // Loop on kernel rows
                        for(int k = 0; k < 3; ++k) {
                                // Loop on kernel columns
                                for(int l = 0; l < 3; ++l) {
                                        accum = accum + A[(i+k)*C + j + l ]*F[k*3+l];
                                }
                        }

                        O[i*(C-2)+j] = accum;
                }

        }
}
