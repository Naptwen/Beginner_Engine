#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef TNEURON_CUDA_H__
#define TNEURON_CUDA_H__

#define KENERL <<< blockPerGrid, threadsPerBlock >>>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <curand_kernel.h>
#include <time.h>
#include <stdio.h>
#include "TNEURON.h"
#include "TCUDA_MATRIX.cuh"

namespace TCUDA 
{
	
	template<typename vtype>
	__global__ void TCUDA_WEIGHT(int seed, int size, vtype* temp)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		curandState state;
		curand_init(seed, i, 0, &state);
		if (i < size)
		{
			if (i % 2 == 0)
				temp[i] = curand_uniform_double(&state);
			else
				temp[i] = 0;
		}

	}
	template<typename vtype>
	__global__ void TCUDA_XWEIGHT(int seed, int size, vtype* temp, double min, double max)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		curandState state;
		curand_init(seed, i, 0, &state);
		if (i < size)
		{
			if (i % 2 == 0)
			{
				vtype random_var = curand_uniform_double(&state);
				random_var *= (max - min + 0.999999);
				random_var += min;
				temp[i] = random_var;
			}				
			else
				temp[i] = 0;
		}

	}
	template<typename vtype>
	__global__ void TCUDA_HWEIGHT(int seed, int size, vtype* temp, double min, double max)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		curandState state;
		curand_init(seed, i, 0, &state);
		if (i < size)
		{
			if (i % 2 == 0)
			{
				vtype random_var = curand_uniform_double(&state);
				random_var *= (max - min + 0.999999);
				random_var += min;
				temp[i] = random_var;
			}				
			else
				temp[i] = 0.001;
		}
	}
	template<typename vtype>
	__global__
		void TCUDA_ZEROS(int size, vtype* target)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		if (i < size)
			target[i] = 0;
	}
	template<typename vtype>
	__global__
		void TCUDA_partial_E(int size, vtype* result, vtype* actual, vtype* par_E, char TYPE)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		if (i < size)
		{
			if (TYPE == MEAN)
				par_E[i] = result[i] - actual[i];
			else if (TYPE == CROSS)
				par_E[i] = actual[i] / result[i];
		}
	}
	template<typename vtype>
	__global__
		void TCUDA_HIDDEN(int _m, int _n, int _j, vtype lr, vtype* parEW, vtype* parEB, vtype* Hidden)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		if (i < _m)
		{
			Hidden[2 * _j + i * _n]
				= Hidden[2 * _j + i * _n] - lr * parEW[i] * Hidden[2 * _j + i * _n];
			Hidden[2 * _j + 1 + i * _n]
				= Hidden[2 * _j + 1 + i * _n] - lr * parEB[i] * Hidden[2 * _j + 1 + i * _n];
		}
	}
	template<typename vtype>
	__global__
		void TCUDA_MOMENTUM(int _m, int _n, int _j, vtype lr, vtype* parEW, vtype* parEB, vtype* Hidden, vtype vr, vtype* momentum)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		if (i < _m)
		{
			momentum[2 * _j + i * _n] = vr * momentum[2 * _j + i * _n] - lr * (parEW[i] * Hidden[2 * _j + i * _n]);
			momentum[2 * _j + 1 + i * _n] = vr * momentum[2 * _j + 1 + i * _n] - lr * (parEB[i] * Hidden[2 * _j + 1 + i * _n]);
			Hidden[2 * _j + i * _n] = Hidden[2 * _j + i * _n] + momentum[2 * _j + i * _n];
			Hidden[2 * _j + 1 + i * _n] = Hidden[2 * _j + 1 + i * _n] + momentum[2 * _j + 1 + i * _n];
		}
	}
	template<typename vtype>
	__global__
		void TCUDA_NESTEROV(int _m, int _n, int _j, vtype lr, vtype* parEW, vtype* parEB, vtype* Hidden, vtype vr, vtype* momentum)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		vtype V_pw = 0;
		vtype V_pb = 0;
		if (i < _m)
		{
			V_pw = momentum[2 * _j + i * _n];
			V_pb = momentum[2 * _j + 1 + i * _n];
			momentum[2 * _j + i * _n] = parEW[i] + vr * momentum[2 * _j + i * _n];
			momentum[2 * _j + 1 + i * _n] = parEB[i] + vr * momentum[2 * _j + i * _n];
			Hidden[2 * _j + i * _n] = (1 + vr) * momentum[2 * _j + i * _n] - vr * V_pw;;
			Hidden[2 * _j + 1 + i * _n] = (1 + vr) * momentum[2 * _j + 1 + i * _n] - vr * V_pb;
		}
	}
	template<typename vtype>
	__global__
		void TCUDA_PARTIAL(int k, int _m, int _n, vtype error, vtype lr, vtype vr, vtype* Z_layer, vtype* A_layer, vtype* W_layer, vtype* V_layer, char fn_TYPE, char back_TYPE)
	{
		int i = threadIdx.x + blockIdx.x * blockDim.x;
		vtype x = 0;
		vtype a_z = 0;
		vtype z_w = 0;
		if (i < _m)
		{
			x = Z_layer[k / 2 + i * _n / 2];
			a_z = 0;
			switch (fn_TYPE)
			{
			case ReLU:
				a_z = 1;
				break;
			case Soft_ReLU:
				a_z = 1 / (1 + exp(-x));
				break;
			case Noisy_ReLU:
				a_z = ((0 < x) ? 0 : 1);
				break;
			case SWISH:
				a_z = (x - 1) / (exp(x) + 1) - x / ((exp(x) + 1) * (exp(x) + 1)) + 1;
				break;
			case LOGISTIC:
				a_z = exp(x) / ((exp(x) + 1) * (exp(x) + 1));
				break;
			case HYPERBOLIC:
				a_z = 4 * exp(2 * x) / ((exp(2 * x) + 1) * (exp(2 * x) + 1));
				break;
			}
			z_w = A_layer[k / 2 + i * _n / 2];
			switch (back_TYPE)
			{
			case MOMEN:

				if ((k + i * _n) % 2 == 0)
				{
					V_layer[k + i * _n] = (vr * V_layer[k + i * _n] + lr * error * a_z * z_w);
					W_layer[k + i * _n] = W_layer[k + i * _n] + V_layer[k + i * _n];
				}
				else
				{
					V_layer[k + i * _n] = vr * V_layer[k + i * _n] + lr * error;
					W_layer[k + i * _n] = W_layer[k + i * _n] + V_layer[k + i * _n];
				}


				break;
			case NESTEROV:
				break;
			default:
				if ((k + i * _n) % 2 == 0)
					W_layer[k + i * _n] = W_layer[k + i * _n] + lr * error * a_z * z_w;
				else
					W_layer[k + i * _n] = W_layer[k + i * _n] + lr * error;
				V_layer[k + i * _n] = 0;
				break;
			}
		}
	}
	//-----------------------------------------------------------------------------------------------
	template<typename vtype>
	void TCUDA_WEIGHT_INIT(TNEURAL_NET<vtype>* net, char type)
	{
		dim3 blockPerGrid(512, 1, 1);
		dim3 threadsPerBlock(512, 1, 1);
		unsigned int size = net->Hidden_layer.m * net->Hidden_layer.n;
		const int input = net->r;
		const int output = net->e;
		vtype* weight;
		cudaMalloc(&weight, size * sizeof(vtype));
		unsigned long seed = clock();
		switch (type)
		{
		case XAVIER:
		{
			double _min = -sqrt(6 / (input + output));
			double _max = sqrt(6 / (input + output));
			TCUDA_XWEIGHT<vtype> KENERL(seed, size, weight, _min, _max);
		}
			break;
		case HENORM:
		{
			double _min = -sqrt(6 / (input));
			double _max = sqrt(6 / (input));
			TCUDA_HWEIGHT<vtype> KENERL(seed, size, weight, _min, _max);
		}
			break;
		default:
			TCUDA_WEIGHT<vtype> KENERL(seed, size, weight);
			break;
		}
		cudaMemcpy(net->Hidden_layer.vectors, weight, size * sizeof(vtype), cudaMemcpyDeviceToHost);
		cudaFree(weight);
	}
	template<typename vtype>
	void TCUDA_MOMENTUM_INIT(TNEURAL_NET<vtype>* net)
	{
		dim3 blockPerGrid(512, 1, 1);
		dim3 threadsPerBlock(512, 1, 1);
		unsigned int size = net->Momentum_layer.m * net->Momentum_layer.n;
		vtype* zeros;
		cudaMalloc(&zeros, size * sizeof(vtype));
		cudaMemset(zeros, 0, size * sizeof(vtype));
		cudaMemcpy(net->Momentum_layer.vectors, zeros, size * sizeof(vtype), cudaMemcpyDeviceToHost);
		cudaFree(zeros);
	}
	template<typename vtype>
	void TCUDA_BACK_PROPA(TNEURAL_NET<vtype>* net, vtype* actual_data, char error_TYPE)
	{

		dim3 blockPerGrid(512, 1, 1);
		dim3 threadsPerBlock(512, 1, 1);
		unsigned int _m = net->Hidden_layer.m;
		unsigned int _n = net->Hidden_layer.n;
		unsigned int _e = net->e;
		//--------------------------------
		vtype* c_Z_layer = NULL;
		vtype* c_A_layer = NULL;
		vtype* c_W_layer = NULL;
		vtype* c_V_layer = NULL;

		cudaMalloc(&c_Z_layer, _m * _n / 2 * sizeof(vtype));
		cudaMalloc(&c_A_layer, _m * _n / 2 * sizeof(vtype));
		cudaMalloc(&c_W_layer, _m * _n * sizeof(vtype));
		cudaMalloc(&c_V_layer, _m * _n * sizeof(vtype));

		cudaMemcpy(c_Z_layer, net->Z_layer.vectors, _m * _n / 2 * sizeof(vtype), cudaMemcpyHostToDevice);
		cudaMemcpy(c_A_layer, net->A_layer.vectors, _m * _n / 2 * sizeof(vtype), cudaMemcpyHostToDevice);
		cudaMemcpy(c_W_layer, net->Hidden_layer.vectors, _m * _n * sizeof(vtype), cudaMemcpyHostToDevice);
		cudaMemcpy(c_V_layer, net->Momentum_layer.vectors, _m * _n * sizeof(vtype), cudaMemcpyHostToDevice);

		//--------------------------------------------
		vtype* c_actual_data = NULL;
		cudaMalloc(&c_actual_data, _e * sizeof(vtype));
		cudaMemcpy(c_actual_data, actual_data, _e * sizeof(vtype), cudaMemcpyHostToDevice);
		//--------------------------------------------
		vtype* c_result_data = NULL;
		cudaMalloc(&c_result_data, _e * sizeof(vtype));
		cudaMemcpy(c_result_data, net->Output_layer.vectors, _e * sizeof(vtype), cudaMemcpyHostToDevice);
		//--------------------------------------------	
		int k = _n - 1;
		vtype error = 0;
		while (k >= 0)
		{
			for (unsigned int i = 0; i < _e; i++)
			{
				error = 0;
				if (error_TYPE == MEAN)
					error = actual_data[i] - net->Output_layer.vectors[i];
				else if (error_TYPE == CROSS)
					error = -net->Output_layer.vectors[i] * log(actual_data[i]);

				TCUDA_PARTIAL<vtype> KENERL (k, _m, _n, error, net->Learning_rate, net->Velocity_rate, c_Z_layer, c_A_layer, c_W_layer, c_V_layer, net->Active_type, net->BACK_type);

			}
			k--;
		}

		cudaMemcpy(net->Hidden_layer.vectors, c_W_layer, _m * _n * sizeof(vtype), cudaMemcpyDeviceToHost);
		cudaMemcpy(net->Momentum_layer.vectors, c_V_layer, _m * _n * sizeof(vtype), cudaMemcpyDeviceToHost);
		cudaFree(c_Z_layer);
		cudaFree(c_A_layer);
		cudaFree(c_W_layer);
		cudaFree(c_V_layer);
		cudaFree(c_actual_data);
		cudaFree(c_result_data);
	}
	template<typename vtype>
	void TCUDA_WORKING(TNEURAL_NET<vtype>* NET)
	{
		int i = 0;
		int _m = NET->Hidden_layer.m;
		int _n = NET->Hidden_layer.n / 2;
		TMATRIX<vtype> TEMP(_m + 1, 1);

		//FIRST RECIVE VALUE
		for (int k = 0; k < _m + 1; k++)
		{
			if (k == _m)
				TEMP.vectors[k] = 1;
			else
				TEMP.vectors[k] = NET->Input_layer.vectors[k];
		}
		//NOW OTHER
		for (int i = 0; i < _n * 2; i += 2)
		{
			//CUT
			TMATRIX<vtype>* X = TCUDA_MATRIX_CONVERT<vtype>(&NET->Hidden_layer, NULL, i, i, CUT_COL);
			TMATRIX<vtype>* B = TCUDA_MATRIX_CONVERT<vtype>(&NET->Hidden_layer, NULL, i + 1, i + 1, CUT_COL);
			//EXTEND
			TMATRIX<vtype>* X_ = TCUDA_MATRIX_CONVERT<vtype>(X, 0, 0, _m - 1, COPY_COL);
			TMATRIX<vtype>* XB = TCUDA_MATRIX_CONVERT<vtype>(X_, B, 0, 0, ADD_COL);
			//X * DATA
			TMATRIX<vtype>* XBA = TCUDA_MATRIX_CONVERT<vtype>(XB, &TEMP, 0, 0, MULTI);
			//ACTIVATION TO DATA
			for (int j = 0; j < _m; j++)
			{
				TEMP.vectors[j] = NET->TACTIVATION_FUN(XBA->vectors[j]);
				//DATA SAVE
				NET->Z_layer.vectors[i / 2 + j * _n] = XBA->vectors[j];
				NET->A_layer.vectors[i / 2 + j * _n] = TEMP.vectors[j];
			}
			std::free(X);
			std::free(B);
			std::free(X_);
			std::free(XB);
			std::free(XBA);
		}
		for (int p = 0; p < NET->m; p++)
		{
			if (p < NET->e)
				NET->Output_layer.vectors[p] = TEMP.vectors[p];
			else
				NET->Output_layer.vectors[p] = 0;
		}

	}
}
#endif