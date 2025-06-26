@echo off

if /i "%1" == "main" (
    g++ main.cpp random.cpp poly.cpp polymat.cpp k_pke.cpp ml_kem.cpp -o main -Wall
)

if /i "%1" == "benchmark" (
    g++ benchmark.cpp random.cpp poly.cpp polymat.cpp k_pke.cpp ml_kem.cpp -o benchmark -Wall
)

if /i "%1" == "clean" (
    del *.exe
)
