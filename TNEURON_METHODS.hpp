#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef TNEURON_METHODS_H__
#define TNEURON_METHODS_H__
#define SUPERVISE 71

#include "TNEURON.h"
#include "TNEURON_CUDA.cuh"

typedef void (*call_pinter)(const char*);
template<typename vtype>
void CPU_SUPERVISER(TNEURAL_NET<vtype>* NET, vtype* input, vtype* target, vtype min_error, call_pinter printer)
{
	NET->INPUT_VALUE(input);
	unsigned long int c = 0;
	vtype original_learning_rate = NET->Learning_rate;
	vtype original_Momentum_rate = NET->Velocity_rate;
	vtype* error_list = (vtype*)malloc(sizeof(vtype) * 11);
	vtype* init_weight = (vtype*)malloc(sizeof(vtype) * NET->m * NET->n * 2);
	vtype* init_momentum = (vtype*)malloc(sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
	std::memcpy(init_weight, NET->Hidden_layer.vectors, sizeof(vtype) * NET->m * NET->n * 2);
	std::memcpy(init_momentum, NET->Momentum_layer.vectors, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real<vtype> dist(0.5, 1);
	double error = 0;
	vtype omega = 0;
	while (true)
	{
		NET->CPU_WORKING();
		if (NET->TVALUE_CHECK())
		{
			error = NET->TNEURON_CHECK(target, MEAN);
			//INF and NAN
			if (c > 10)
			{
				omega = STANDARD(error_list, c);
				//Gradient vanishing
				if (omega < 0.004)
				{
					memcpy(NET->Hidden_layer.vectors, init_weight, sizeof(vtype) * NET->Hidden_layer.m * NET->Hidden_layer.n);
					NET->Learning_rate += original_learning_rate * dist(gen);
					memcpy(NET->Momentum_layer.vectors, init_momentum, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
					NET->Velocity_rate = dist(gen);
					c = 0;
				}
				c = 0;
			}
			else if (error <= min_error)
				break;
			else
			{
				NET->BACK_PROPAGATION(target, MEAN);
				printer(std::to_string(error).c_str());
				error_list[c] = error;
				c++;
			}
		}
		//INF NAN error
		else
		{
			std::memcpy(NET->Hidden_layer.vectors, init_weight, sizeof(vtype) * NET->Hidden_layer.m * NET->Hidden_layer.n);
			NET->Learning_rate = original_learning_rate * dist(gen);
			std::memcpy(NET->Momentum_layer.vectors, init_momentum, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
			NET->Velocity_rate = dist(gen);
			c = 0;
		}
	}
	NET->Learning_rate = original_learning_rate;
	NET->Velocity_rate = original_Momentum_rate;
	std::memcpy(NET->Momentum_layer.vectors, init_momentum, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
	std::free(error_list);
	std::free(init_weight);
	std::free(init_momentum);
}
#ifdef TNEURON_CUDA_H__
//
//printer : INF,ERROR,VANISHING dividing by |
template<typename vtype>
void GPU_SUPERVISER(TNEURAL_NET<vtype>* NET, vtype* input, vtype* target, vtype min_error, call_pinter printer)
{
	unsigned long int count = 0;
	unsigned long int INFC = 0;
	//
	NET->INPUT_VALUE(input);
	unsigned long int c = 0;
	vtype original_learning_rate = NET->Learning_rate;
	vtype original_Momentum_rate = NET->Velocity_rate;
	double* error_list = (double*)malloc(sizeof(double) * 51);
	vtype* init_weight = (vtype*)malloc(sizeof(vtype) * NET->Hidden_layer.m * NET->Hidden_layer.n);
	vtype* init_momentum = (vtype*)malloc(sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
	std::memcpy(init_weight, NET->Hidden_layer.vectors, sizeof(vtype) * NET->Hidden_layer.m * NET->Hidden_layer.n);
	std::memcpy(init_momentum, NET->Momentum_layer.vectors, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real<vtype> dist(0.2, 1);

	double error = 0;
	double omega = 0;

	while (true)
	{
		TCUDA::TCUDA_WORKING(NET);
		if (NET->TVALUE_CHECK())
		{
			error = NET->TNEURON_CHECK(target, MEAN);
			//vanishing
			if (c > 50)
			{
				//Gradient vanishing							
				omega = STANDARD(error_list, c);
				if (omega < 0.3)
				{
					//memcpy(NET->Hidden_layer.vectors, init_weight, sizeof(vtype) * NET->Hidden_layer.m * NET->Hidden_layer.n);
					//memcpy(NET->Momentum_layer.vectors, init_momentum, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);					
					NET->Learning_rate += NET->Learning_rate * dist(gen);	
					std::string str_van = "VANISHING|" + std::to_string((double)NET->Learning_rate);
					printer(str_van.c_str());
					//memcpy(NET->Momentum_layer.vectors, init_momentum, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
					count++;
				}
				c = 0;
			}
			else if (error <= min_error)
			{
				printer("NEURAL FINISH");
				break;
			}
			else
			{
				TCUDA::TCUDA_BACK_PROPA(NET, target, MEAN);
				std::string str_error = ("ERROR|" + std::to_string(error));
				printer(str_error.c_str());
				//NET->Hidden_layer.pinfo();
				error_list[c] = error;
				c++;
			}
		}
		//INF NAN error
		else
		{
			NET->A_layer.pinfo();
			printer("INF|");
			std::memcpy(NET->Hidden_layer.vectors, init_weight, sizeof(vtype) * NET->Hidden_layer.m * NET->Hidden_layer.n);
			NET->Learning_rate = original_learning_rate * dist(gen);
			std::memcpy(NET->Momentum_layer.vectors, init_momentum, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
			NET->Velocity_rate = dist(gen);
			c = 0;
			INFC++;
			count = 0;
		}
	}
	NET->Learning_rate = original_learning_rate;
	NET->Velocity_rate = original_Momentum_rate;
	std::memcpy(NET->Momentum_layer.vectors, init_momentum, sizeof(vtype) * NET->Momentum_layer.m * NET->Momentum_layer.n);
	std::free(error_list);
	std::free(init_weight);
	std::free(init_momentum);
}
#endif
template<typename vtype>
void LEARN_NERONS(TNEURAL_NET<vtype>* NET, vtype* input_data, vtype* target_data, vtype min_error, std::string file_name, bool gpu, bool init, call_pinter printer)
{
	//Neural setting-----------------------------------
	if (gpu)
	{
#ifdef TNEURON_CUDA_H__
		if (init == true)
			TCUDA::TCUDA_WEIGHT_INIT(NET, XAVIER);
		GPU_SUPERVISER(NET, input_data, target_data, min_error, printer);
#else
		printf("CUDA HEADER IS NOT DEFINED")
#endif
	}
	else
	{
		if (init == true)
			NET->CPU_WEIGHT_INIT(XAVIER);
		CPU_SUPERVISER(NET, input_data, target_data, min_error, printer);
	}
	NET->NEURAL_SAVE(file_name);
}
#endif