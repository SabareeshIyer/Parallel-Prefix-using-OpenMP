/*  Sabareesh
 *  Iyer
 */

#ifndef A0_HPP
#define A0_HPP
#include <iostream>
#include <omp.h>
#include <math.h>
#include <time.h>

using namespace std;

template <typename T, typename Op>
void omp_scan(int n, const T* in, T* out, Op op) {
	int p;
	#pragma omp parallel
	{
		p = omp_get_num_threads();
	}
	
	if (n < 1000){
		p = 10;
		omp_set_num_threads(10);
	}
	/*
	else{
		if(p>10){
			omp_set_num_threads(30);
			p = 30;
		}
	}
	*/
	#pragma omp parallel for
	for (int i = 1; i<=n/p; i++){
		out[p*(i-1)] = in[p*(i-1)];
		for (int j = 1;j<p;j++){
			out[p*(i-1)+j] = op(out[p*(i-1)+j-1], in[p*(i-1)+j]);
		}
	}
		
	std::vector<T> r(ceil(n/p)+1);
	r[0] = 0;
	r[1] = out[p-1];
	for(int i=2;i<=n/p;i++){
		r[i] = op(r[i-1], out[p*i-1]);
	}
	
	#pragma omp parallel for
	for(int i = 2; i<=n/p; i++){
		for(int j = p*(i-1); j<p*i; j++){
			out[j] = op(r[i-1], out[j]);
		}
	}	
	
	int index;
	if(n%p != 0){
		index = (n/p)*p;
		for(;index<n;index++){
			out[index] = op(out[index-1], in[index]);
		}
	}	
} // omp_scan

#endif // A0_HPP
