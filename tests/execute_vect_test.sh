vec_size=$1
vec_size2=$2
run_numb=$3
mkdir -p test_results
echo "Compiling"
srun -p arm-hi1616 armclang++ itest_vectorization_fun.cpp -I../include -Ofast -march=armv8-a+sve  -o vec
srun -p arm-hi1616 armclang++ itest_vectorization_fun.cpp -I../include -O0 -o no_vec
echo "Spawning tests..."
srun -p arm-hi1616 armie -msve-vector-bits=128 no_vec 225 225 3 1000 >test_results/naive.txt &
srun -p arm-hi1616 armie -msve-vector-bits=128 vec 225 225 3 1000   >test_results/sve128.txt &
srun -p arm-hi1616 armie -msve-vector-bits=256 vec 225 225 3 1000   >test_results/sve256.txt &
srun -p arm-hi1616 armie -msve-vector-bits=512 vec 225 225 3 1000   >test_results/sve512.txt &
srun -p arm-hi1616 armie -msve-vector-bits=1024 vec 225 225 3 1000  >test_results/sve1024.txt &
srun -p arm-hi1616 armie -msve-vector-bits=2048 vec 225 225 3 1000  >test_results/sve2048.txt &
