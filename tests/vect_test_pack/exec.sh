#!/bin/bash
. epienv
. armenv
. riscvenv
clang++ -mepi -D_EPI_ -I../../../include ../main.cpp -static -o rv_test
clang++ -mepi -D_EPI_ -D_TEST_SIG -I../../../include ../main.cpp -static -o sig_rv_test
clang++ -mepi -D_EPI_ -D_TEST_TANH -I../../../include ../main.cpp -static -o tan_rv_test
clang++ -mepi -D_EPI_ -D_TEST_ELU -I../../../include ../main.cpp -static -o elu_rv_test

#aarch64-linux-g++ -O0 -fno-tree-vectorize -march=armv8-a+sve -I../../include main.cpp -static -o sv-test
clang++ -O0 -fno-vectorize -I../../../include ../main.cpp -static -o naive_test
clang++ -O0 -fno-vectorize -D_TEST_SIG -I../../../include ../main.cpp -static -o sig_naive_test
clang++ -O0 -fno-vectorize  -D_TEST_TANH -I../../../include ../main.cpp -static -o tan_naive_test
clang++ -O0 -fno-vectorize -D_TEST_ELU -I../../../include ../main.cpp -static -o elu_naive_test

echo "GEMM RV-128"
spike --isa=rv64gcv --varch=v128:e64:s128 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test  $1 $2 $3 $4 > tmp/gemm_rvv_128.csv
echo "GEMM RV-256"
spike --isa=rv64gcv --varch=v256:e64:s256 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 $3 $4 > tmp/gemm_rvv_256.csv
echo "GEMM RV-512"
spike --isa=rv64gcv --varch=v512:e64:s512 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 $3 $4 > tmp/gemm_rvv_512.csv
echo "GEMM RV-1024"
spike --isa=rv64gcv --varch=v1024:e64:s1024 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 $3 $4 > tmp/gemm_rvv_1024.csv
echo "GEMM RV-2048"
spike --isa=rv64gcv --varch=v2048:e64:s2048 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 $3 $4 > tmp/gemm_rvv_2048.csv
echo "NAIVE"
spike --isa=rv64gcv /opt/riscv/riscv64-unknown-linux-gnu/bin/pk naive_test $1 $2 $3 $4 > tmp/gemm_naive.csv

echo "Dot RV-128"
spike --isa=rv64gcv --varch=v128:e64:s128 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test  $1 $2 > tmp/dot_rvv_128.csv
echo "Dot RV-256"
spike --isa=rv64gcv --varch=v256:e64:s256 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 > tmp/dot_rvv_256.csv
echo "Dot RV-512"
spike --isa=rv64gcv --varch=v512:e64:s512 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 > tmp/dot_rvv_512.csv
echo "Dot RV-1024"
spike --isa=rv64gcv --varch=v1024:e64:s1024 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 > tmp/dot_rvv_1024.csv
echo "Dot RV-2048"
spike --isa=rv64gcv --varch=v2048:e64:s2048 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 > tmp/dot_rvv_2048.csv
echo "NAIVE"
spike --isa=rv64gcv --varch=v2048:e64:s1024 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk naive_test $1 $2 > tmp/dot_naive.csv

echo "Conv RV-128"
spike --isa=rv64gcv --varch=v128:e64:s128 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test  $1 $2 $3 > tmp/conv_rvv_128.csv
echo "Conv RV-256"
spike --isa=rv64gcv --varch=v256:e64:s256 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 $3 > tmp/conv_rvv_256.csv
echo "Conv RV-512"
spike --isa=rv64gcv --varch=v512:e64:s512 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 $3 > tmp/conv_rvv_512.csv
echo "Conv RV-1024"
spike --isa=rv64gcv --varch=v1024:e64:s1024 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 $3 > tmp/conv_rvv_1024.csv
echo "Conv RV-2048"
spike --isa=rv64gcv --varch=v2048:e64:s2048 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk rv_test $1 $2 $3 > tmp/conv_rvv_2048.csv
echo "NAIVE"
spike --isa=rv64gcv /opt/riscv/riscv64-unknown-linux-gnu/bin/pk naive_test $1 $2 $3 > tmp/conv_naive.csv

echo "SIGMOID RV-128"
spike --isa=rv64gcv --varch=v128:e64:s128 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk sig_rv_test  $1 > tmp/sig_rvv_128.csv
echo "SIGMOID RV-256"
spike --isa=rv64gcv --varch=v256:e64:s256 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk sig_rv_test $1 > tmp/sig_rvv_256.csv
echo "SIGMOID RV-512"
spike --isa=rv64gcv --varch=v512:e64:s512 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk sig_rv_test $1  > tmp/sig_rvv_512.csv
echo "SIGMOID RV-1024"
spike --isa=rv64gcv --varch=v1024:e64:s1024 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk sig_rv_test $1  > tmp/sig_rvv_1024.csv
echo "SIGMOID RV-2048"
spike --isa=rv64gcv --varch=v2048:e64:s2048 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk sig_rv_test $1  > tmp/sig_rvv_2048.csv
echo "NAIVE"
spike --isa=rv64gcv /opt/riscv/riscv64-unknown-linux-gnu/bin/pk sig_naive_test $1 > tmp/sig_naive.csv


echo "ELU RV-128"
spike --isa=rv64gcv --varch=v128:e64:s128 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk elu_rv_test  $1 > tmp/elu_rvv_128.csv
echo "ELU RV-256"
spike --isa=rv64gcv --varch=v256:e64:s256 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk elu_rv_test $1 > tmp/elu_rvv_256.csv
echo "ELU RV-512"
spike --isa=rv64gcv --varch=v512:e64:s512 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk elu_rv_test $1  > tmp/elu_rvv_512.csv
echo "ELU RV-1024"
spike --isa=rv64gcv --varch=v1024:e64:s1024 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk elu_rv_test $1  > tmp/elu_rvv_1024.csv
echo "ELU RV-2048"
spike --isa=rv64gcv --varch=v2048:e64:s2048 /opt/riscv/riscv64-unknown-linux-gnu/bin/pk elu_rv_test $1  > tmp/elu_rvv_2048.csv
echo "NAIVE"
spike --isa=rv64gcv /opt/riscv/riscv64-unknown-linux-gnu/bin/pk elu_naive_test $1 > tmp/elu_naive.csv





 
