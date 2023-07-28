#include "posit.h"

using namespace std;


template <class pp_t, class ft_t>
void fxt(pp_t dummy,ft_t af,const char * name)
{

     pp_t bf1 = af;
     pp_t bf2(af);
	 pp_t bf3;
	 bf3 = af;	
	 std::cout << "\n\n-----\ntesting: " << name << std::endl;
	 std::cout << "in:" << af << std::endl;
	 std::cout << bf1 << " " << bf2 << " " << bf3 << std::endl;
	 std::cout << (ft_t)bf1 << " " << (ft_t)bf2 << " " << (ft_t)bf3 << std::endl;
	 assert(bf1 == bf2);
	 assert(bf1 == bf3);
	 std::cout << "\n\n";
}
int main(int argc, char const *argv[])
{
	 using pp_t = Posit<int32_t, 16, 2, uint64_t, PositSpec::WithNan>;

	 fxt(pp_t(),-0.0446161f,"float");
	 fxt(pp_t(),-0.0446161,"double");
	 fxt(pp_t(),-5,"int");
	return 0;
}