template <class T>
void fallbackTanh(std::vector<T>& data,std::vector<T>& out);

template <class T>
std::vector<unsigned long int> testTanh(vectorized::PositVectorizedFrontend<T>* fv, int runs,int vectorSize) {
	std::vector<unsigned long int> times;
	std::vector<T> out(vectorSize);
	for(int i = 0; i < runs; ++i) {
		auto data = genVector<T>(vectorSize);
		auto start = get_time::now();
		(fv != nullptr)?fv->vFastTanh(data,out):fallbackTanh(data,out);
		auto end = get_time::now();
		std::cout << std::chrono::duration_cast<ns>(end - start).count() << std::endl;
		//times.push_back(std::chrono::duration_cast<ns>(end - start).count());
	}
	return times;
}
template <class T>
void fallbackTanh(std::vector<T>& data,std::vector<T>& out) {
        for(int i = 0; i < data.size(); ++i)
		out[i] = data[i].fastTanh();
}
