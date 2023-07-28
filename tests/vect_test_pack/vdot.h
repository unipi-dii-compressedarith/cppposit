template <class T>
T fallbackDot(std::vector<T>& data);

template <class T>
std::vector<unsigned long int> testDot(vectorized::PositVectorizedFrontend<T>* fv, int runs,int vectorSize) {
	std::vector<unsigned long int> times;
	for(int i = 0; i < runs; ++i) {
		auto data = genVector<T>(vectorSize*vectorSize);
		auto start = get_time::now();
		(fv != nullptr)?fv->vDot(data,data):fallbackDot(data);
		auto end = get_time::now();
        if(fv == nullptr) std::cout << std::chrono::duration_cast<ns>(end - start).count() << std::endl;		
        //times.push_back(std::chrono::duration_cast<ns>(end - start).count());
	}
	return times;
}

template <class T>
std::vector<unsigned long int> testDotX(vectorized::PositRVVBackend<T>& fv, int runs,int vectorSize) {
	std::vector<unsigned long int> times;
	for(int i = 0; i < runs; ++i) {
		auto data = genVector<T>(vectorSize*vectorSize);
		auto start = get_time::now();
		fv.vDotX(data,data);
		auto end = get_time::now();
	}
	return times;
}
template <class T>
T fallbackDot(std::vector<T>& data) {
	T acc = 0;
        for(int i = 0; i < data.size(); ++i)
                acc+=(data[i]*data[i]);
        return acc;
}
