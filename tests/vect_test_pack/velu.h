template <class T>
void fallbackELU(std::vector<T>& data,std::vector<T>& out);

template <class T>
void testELU(vectorized::PositVectorizedFrontend<T>* fv, int runs,int vectorSize) {
	std::vector<T> out(vectorSize);
	auto data = genVector<T>(vectorSize);    
	for(int i = 0; i < runs; ++i) {
		auto start = get_time::now();
		(fv != nullptr)?fv->vFastELU(data,out):fallbackELU(data,out);
		auto end = get_time::now();
		std::cout << std::chrono::duration_cast<ns>(end - start).count() << std::endl;
	}
}
template <class T>
void fallbackELU(std::vector<T>& data,std::vector<T>& out) {
        for(int i = 0; i < data.size(); ++i)
		out[i] = data[i].fastELU();
}
