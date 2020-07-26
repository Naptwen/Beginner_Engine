#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef TNEURON_H__
#define TNEURON_H__

#include <random>//c++ style random
#include "TMATRIX.h"


#define ReLU 16
#define Soft_ReLU 17
#define Noisy_ReLU 18
#define Leaky_ReLU 19
#define ELU 4
#define SWISH 11
#define LOGISTIC 5
#define HYPERBOLIC 3

#define MEAN 1
#define CROSS 2

#define XAVIER 87
#define HENORM 13

#define MOMEN 77
#define NESTEROV 73

template<typename vtype>
void Median_Normalize(vtype* data, unsigned int size)
{
	vtype* temp = (vtype*)malloc(sizeof(vtype) * size);
	memmove(temp, data, sizeof(vtype) * size);
	vtype Medium = Find_Mean(temp, size);
	free(temp);

	//standard diviation
	vtype sd = Standard(data, Medium, size);
	//Normalize
	for (unsigned int i = 0; i < size; i++)
		data[i] = (data[i] - Medium) / sd;
}
template<typename vtype>
void Mean_Normalize(vtype* data, unsigned int size)
{
	vtype sum = 0;
	for (unsigned int i = 0; i < size; i++)
		sum += data[i];
	vtype mean = sum / size;
	vtype sd = Standard(data, mean, size);
	for (unsigned int i = 0; i < size; i++)
		data[i] = (data[i] - mean) / sd;
}
template<typename vtype>
void Quantile_Normarlize(vtype* data, unsigned int size)
{
	vtype max = maxArray(data, size);
	vtype min = minArray(data, size);
	if (max - min != 0)
		for (unsigned int i = 0; i < size; i++)
			data[i] = (data[i] - min) / (max - min);
}
template<typename vtype>
int minArray(vtype* data, unsigned int size)
{
	vtype min = data[0];
	int num = 0;
	for (unsigned int i = 1; i < size; i++)
		if (min > data[i])
		{
			min = data[i];
			num = i;
		}
	return num;
}
template<typename vtype>
int maxArray(vtype* data, unsigned int size)
{
	vtype max = data[0];
	int num = 0;
	for (unsigned int i = 1; i < size; i++)
		if (max < data[i])
		{
			max = data[i];
			num = i;
		}
	return num;
}
template<typename vtype>
void buble_sorting(vtype* data, unsigned int size)
{
	for (unsigned int i = 0; i < size - 1; i++)
	{
		vtype temp = data[i];
		if (data[i + 1] < data[i])
		{
			data[i] = data[i + 1];
			data[i + 1] = temp;
			i = 0;
		}
	}
}
template<typename vtype>
vtype Find_Mean(vtype data[], unsigned int size)
{
	buble_sorting(data, size);
	vtype Mean = 0;
	if (size % 2 == 0)
	{
		int A = size / 2;
		int B = size / 2 + 1;
		Mean = data[(A + B) / 2];
	}
	else
	{
		Mean = data[size / 2 + 1];
	}
	return Mean;
}
template<typename vtype>
vtype Standard(vtype* data, vtype Ed, unsigned int size)
{
	vtype sd = 0;
	for (unsigned int i = 0; i < size; i++)
		sd += pow(data[i] - Ed, 2);
	sd = sd / size;
	sd = sqrt(sd);
	return sd;
}
template<typename vtype>
vtype average(vtype* data, unsigned int size)
{
	vtype avg = 0;
	for (unsigned int i = 0; i < size; i++)
		avg = avg + data[i];
	return avg / (vtype)size;
}
template<typename vtype>
vtype variance(vtype* data, vtype avg, unsigned int size)
{
	vtype var = 0;
	for (unsigned int i = 0; i < size; i++)
		var = (data[i] - avg) * (data[i] - avg);
	return var / (vtype)size;
}
template<typename vtype>
vtype z_score(vtype true_mean, vtype avg, vtype var)
{
	if (var == 0)
		return 0;
	return (true_mean - avg) / var;
}
template<typename vtype>
vtype cumulative_std(vtype z)
{
	return erf(z * 0.70710678118) * 0.5 + 0.5;
}
template<typename vtype>
vtype Z_SCORE_TEST(vtype* list, unsigned int size)
{
	vtype avg = average(list, size);
	vtype var = variance(list, avg, size);
	vtype z = z_score(0, avg, var);
	vtype c = cumulative_std(z);
	return c;
}
template<typename vtype>
vtype STANDARD(vtype* list, unsigned int size)
{
	vtype avg = average(list, size);
	vtype var = variance(list, avg, size);
	return sqrt(var);
}

template<typename vtype>
class TNEURAL_NET
{
public:
	TMATRIX<vtype> Input_layer;
	TMATRIX<vtype> Hidden_layer;
	TMATRIX<vtype> Z_layer;
	TMATRIX<vtype> A_layer;
	TMATRIX<vtype> Output_layer;
	TMATRIX<vtype> Momentum_layer;
	unsigned int r;
	unsigned int e;
	unsigned int m; // col x pos
	unsigned int n; // row y pos
	char Active_type;
	char BACK_type;
	vtype Learning_rate;
	vtype Velocity_rate;
	//****************************************************************************
	//*[r] : import part *[m] : hiddenlayer row *[lr]   : learning rate
	//*[e] : export part *[n] : hiddenlayer col *[TYPE] : activation function type 
	//*[vr] : velocity rate
	//<Type> - Soft_ReLU, Noisy_ReLU, Leaky_ReLU, ELU, SWISH, LOGISTIC, HYPERBOLIC
	//<BType> - DEFAULT(any number), MOMEN, NAG
	//****************************************************************************
	TNEURAL_NET(int r, int m, int n, int e, vtype lr, vtype vr, char Type, char BType)
	{
		this->Input_layer = TMATRIX<vtype>(m, 1);
		this->Hidden_layer = TMATRIX<vtype>(m, n * 2);
		this->Z_layer = TMATRIX<vtype>(m, n);
		this->A_layer = TMATRIX<vtype>(m, n);
		this->Output_layer = TMATRIX<vtype>(m, 1);
		this->Momentum_layer = TMATRIX<vtype>(m, n * 2);
		this->Learning_rate = lr;
		this->Velocity_rate = vr;
		this->Active_type = Type;
		this->BACK_type = BType;
		this->r = r;
		this->e = e;
		this->m = m;
		this->n = n;
		for (unsigned int i = 0; i < r; i++)
			this->Input_layer.vectors[i] = 1;
		this->Hidden_layer.ONES();
		for (unsigned int i = 0; i < e; i++)
			this->Output_layer.vectors[i] = 1;
	}
	void CPU_WORKING()
	{
		unsigned int i = 0;
		int _m = this->Hidden_layer.m;
		int _n = this->Hidden_layer.n / 2;
		TMATRIX<vtype>TEMP(_m + 1, 1);
		for (int k = 0; k < _m + 1; k++)
		{
			if (k == _m)
				TEMP.vectors[k] = 1;
			else
				TEMP.vectors[k] = this->Input_layer.vectors[k];
		}
		for (unsigned int i = 0; i < _n * 2; i += 2)
		{
			TMATRIX<vtype>X(this->m, this->n);
			X = this->Hidden_layer;
			X.COLUMCUT(i, i);
			TMATRIX<vtype>B(this->m, this->n);
			B = this->Hidden_layer;
			B.COLUMCUT(i + 1, i + 1);
			TMATRIX<vtype>X_(this->m, this->n);
			X_ = X;
			MATRIXCOPY_COL<vtype>(&X_, 0, _m - 1);
			TMATRIX<vtype>* XB = MATRIXADDCOL(&X_, &B);
			TMATRIX<vtype>* XBA = (*XB) * (TEMP);
			for (int j = 0; j < _m; j++)
			{
				TEMP.vectors[j] = TACTIVATION_FUN(XBA->vectors[j]);
				//DATA SAVE
				this->Z_layer.vectors[i / 2 + j * _n] = XBA->vectors[j];
				this->A_layer.vectors[i / 2 + j * _n] = TEMP.vectors[j];
			}
			std::free(XB);
			std::free(XBA);
		}
		for (int p = 0; p < this->m; p++)
		{
			if (p < this->e)
				this->Output_layer.vectors[p] = TEMP.vectors[p];
			else
				this->Output_layer.vectors[p] = 0;
		}

	}
	//TYPE : MEAN - Mean square Error, CROSS - Cross Entropy
	void BACK_PROPAGATION(vtype* actual_data, const char TYPE)
	{
		unsigned int _m = this->Hidden_layer.m;
		unsigned int _n = this->Hidden_layer.n / 2;

		vtype* partial_E = (vtype*)calloc(_m, sizeof(vtype));
		vtype* partial_A = (vtype*)calloc(_m, sizeof(vtype));
		vtype* partial_Z = (vtype*)calloc(_m, sizeof(vtype));
		vtype* partial_EW = (vtype*)calloc(_m, sizeof(vtype));

		switch (TYPE)
		{
		case MEAN:
			for (unsigned int i = 0; i < this->e; i++)
				partial_E[i] = (this->Output_layer.vectors[i] - actual_data[i]);
			break;
		case CROSS:
			for (unsigned int i = 0; i < this->e; i++)
				partial_E[i] = -actual_data[i] / this->Output_layer.vectors[i];
			break;
		}
		int k = _n - 1;
		while (k >= 0)
		{
			for (unsigned int j = 0; j < _m; j++)
			{
				vtype temp = 0;
				vtype x = this->Z_layer.vectors[j + k * _n];
				switch (this->Active_type)
				{
				case ReLU:
					temp = 1;
					break;
				case Soft_ReLU:
					temp = 1 / (1 + exp(-x));
					break;
				case Noisy_ReLU:
					temp = ((0 < x) ? 0 : 1);
					break;
				case SWISH:
					temp = (x - 1) / (exp(x) + 1) - x / ((exp(x) + 1) * (exp(x) + 1)) + 1;
					break;
				case LOGISTIC:
					temp = exp(x) / ((exp(x) + 1) * (exp(x) + 1));
					break;
				case HYPERBOLIC:
					temp = 4 * exp(2 * x) / ((exp(2 * x) + 1) * (exp(2 * x) + 1));
					break;
				default:
					temp = 1;
					break;
				}
				partial_A[j] = temp;
				partial_Z[j] = this->A_layer.vectors[j];
				partial_EW[j] = partial_E[j] * partial_A[j] * partial_Z[j];
				if (this->BACK_type == MOMEN)
				{
					this->Hidden_layer.vectors[2 * k + j * _n * 2] =
						this->Hidden_layer.vectors[2 * k + j * _n * 2]
						- (this->Learning_rate * partial_EW[j]);
					this->Hidden_layer.vectors[2 * k + 1 + j * _n * 2] =
						this->Hidden_layer.vectors[2 * k + 1 + j * _n * 2]
						- this->Learning_rate * partial_E[j] * partial_A[j];

					this->Momentum_layer.vectors[2 * k + j * _n * 2] =
						this->Hidden_layer.vectors[2 * k + j * _n * 2] + this->Velocity_rate * this->Momentum_layer.vectors[2 * k + j * _n * 2];
					this->Momentum_layer.vectors[2 * k + 1 + j * _n * 2] =
						this->Hidden_layer.vectors[2 * k + 1 + j * _n * 2] + this->Velocity_rate * this->Momentum_layer.vectors[2 * k + 1 + j * _n * 2];

					this->Hidden_layer.vectors[2 * k + j * _n * 2] = this->Momentum_layer.vectors[2 * k + j * _n * 2];
					this->Hidden_layer.vectors[2 * k + 1 + j * _n * 2] = this->Momentum_layer.vectors[2 * k + 1 + j * _n * 2];
				}
				else if (this->BACK_type == NESTEROV)
				{
					vtype v_prev = this->Hidden_layer.vectors[2 * k + j * _n * 2];
					this->Hidden_layer.vectors[2 * k + j * _n * 2] =
						this->Hidden_layer.vectors[2 * k + j * _n * 2] + this->Velocity_rate * this->Hidden_layer.vectors[2 * k + j * _n * 2];
					this->Hidden_layer.vectors[2 * k + 1 + j * _n * 2] =
						(1 + this->Velocity_rate) * this->Hidden_layer.vectors[2 * k + 1 + j * _n] - this->Velocity_rate * v_prev;

					this->Hidden_layer.vectors[2 * k + j * _n * 2] = this->Momentum_layer.vectors[2 * k + j * _n * 2];
					this->Hidden_layer.vectors[2 * k + 1 + j * _n * 2] = this->Momentum_layer.vectors[2 * k + 1 + j * _n * 2];

				}
				else
				{
					this->Hidden_layer.vectors[2 * k + j * _n * 2] =
						this->Hidden_layer.vectors[2 * k + j * _n * 2]
						- (this->Learning_rate * partial_EW[j]);
					this->Hidden_layer.vectors[2 * k + 1 + j * _n * 2] =
						this->Hidden_layer.vectors[2 * k + 1 + j * _n * 2]
						- this->Learning_rate * partial_E[j] * partial_A[j];
				}
			}
			k--;
		}
	}
	void NEURAL_IMPORT(TMATRIX<vtype>& A)
	{
		memmove(this->Input_layer.vectors, A.vectors, this->Input_layer.n);
		this->Input_layer = A;
	}
	void INPUT_VALUE(vtype value[])
	{
		for (unsigned int i = 0; i < this->m; i++)
		{
			if (i < this->r)
				this->Input_layer.vectors[i] = value[i];
			else
				this->Input_layer.vectors[i] = 0;

		}
	}
	//*********************************************************************
	//TYPE 
	//[XAVIER] - Xavier initializing (Using Normalizing)
	//[He Normal] - He initializing
	//empty spce for type - between 0~1
	void CPU_WEIGHT_INIT(const char TYPE)
	{
		int _m = this->Hidden_layer.m;
		int _n = this->Hidden_layer.n;
		//C++ style random generating
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<vtype> rng(0, 2);
		switch (TYPE)
		{
		case XAVIER:
			for (unsigned int i = 0; i < _m * _n; i += 2)
			{
				this->Hidden_layer.vectors[i] = rng(gen) / sqrt(this->r);
				this->Hidden_layer.vectors[i + 1] = 0;
			}
			break;
		case HENORM:
			for (unsigned int i = 0; i < _m * _n; i += 2)
			{
				this->Hidden_layer.vectors[i] = rng(gen) / sqrt(this->r / 2);
				this->Hidden_layer.vectors[i + 1] = 0;
			}
			break;
		default:
			for (unsigned int i = 0; i < _m * _n; i += 2)
			{
				this->Hidden_layer.vectors[i] = static_cast <vtype> (rand()) / static_cast <vtype> (RAND_MAX);
				this->Hidden_layer.vectors[i + 1] = 0;
			}
			break;
		}

	}
	//log info
	void info()
	{
		this->Input_layer.info();
		this->Hidden_layer.info();
		this->Z_layer.info();
		this->A_layer.info();
		this->Output_layer.info();
	}
	// prunsigned int info
	void pinfo()
	{
		std::cout << "----------------------------------------" << std::endl;
		std::cout << "[Activiation TYpe] : ";
		switch (this->Active_type)
		{
		case ReLU:
			printf("ReLU\n");
			break;
		case Soft_ReLU:
			printf("Soft ReLU\n");
			break;
		case Noisy_ReLU:
			printf("Noisy_ReLU\n");
			break;
		case Leaky_ReLU:
			printf("Leaky_ReLU\n");
			break;
		case ELU:
			printf("ELU\n");
			break;
		case SWISH:
			printf("SWISH by google\n");
			break;
		case LOGISTIC:
			printf("LOGISTIC\n");
			break;
		case HYPERBOLIC:
			printf("HYPERBOLIC\n");
			break;
		}
		std::cout << "[BACK TYPE] : ";
		switch (this->BACK_type)
		{
		case MOMEN:
			printf("MOMENTUM\n");
			break;
		case NESTEROV:
			printf("NESTROV\n");
			break;
		default:
			printf("Default\n");
			break;
		}

		std::cout << "[Learning Rate] : " << this->Learning_rate << "[Velocity Rate] : " << this->Velocity_rate << std::endl;
		std::cout << "----------------------------------------" << std::endl;
		std::cout << "[input data]\n";
		this->Input_layer.pinfo();
		std::cout << "[Weight data]\n";
		this->Hidden_layer.pinfo();
		std::cout << "[Momentum data]\n";
		this->Momentum_layer.pinfo();
		std::cout << "[z data]\n";
		this->Z_layer.pinfo();
		std::cout << "[Active data]\n";
		this->A_layer.pinfo();
		std::cout << "[output data]\n";
		this->Output_layer.pinfo();
	}

	bool TVALUE_CHECK()
	{
		int count = 0;

		for (unsigned int i = 0; i < Hidden_layer.m * Hidden_layer.n; i++)
		{
			if (isinf(Hidden_layer.vectors[i]) || isnan(Hidden_layer.vectors[i]))
				return false;
		}

		for (unsigned int i = 0; i < Output_layer.m * Output_layer.n; i++)
		{
			if (isinf(Output_layer.vectors[i]) || isnan(Output_layer.vectors[i]))
				return false;
		}
		return true;
	}

	vtype TNEURON_CHECK(vtype* target, const char TYPE)
	{
		vtype sum = 0.0;
		switch (TYPE)
		{
		case MEAN:
		{
			for (unsigned int i = 0; i < this->e; i++)
			{
				sum += (this->Output_layer.vectors[i] - target[i]) * (this->Output_layer.vectors[i] - target[i]);
			}
			sum = sum / this->e;
			break;
		}
		case CROSS:
		{
			Quantile_Normarlize(this->Output_layer.vectors, this->e);
			for (unsigned int i = 0; i < this->e; i++)
			{
				sum -= (target[i]) * log(this->Output_layer.vectors[i]);
			}
			sum = sum / this->e;
			break;
		}
		default:
			break;
		}
		return sum;
	}

	vtype TACTIVATION_FUN(vtype y)
	{
		switch (this->Active_type)
		{
		case ELU:
			return ((0 < y) ? 0.01 * (exp(y) - 1) : y);
		case ReLU:
			return ((0 < y) ? y : 0);
		case Leaky_ReLU:
			return ((0 < y) ? 0.01 * y : y);
		case Noisy_ReLU:
			return ((0 < y) ? 0 : y);
		case Soft_ReLU:
			return log(1 + exp(y));
		case SWISH:
			return y / (1 + exp(-y));
		case LOGISTIC:
			return 1 / (1 + exp(-y));
		case HYPERBOLIC:
			return (exp(y) - exp(-y)) / (exp(y) + exp(-y));
		default:
			if (y > 0)
				return 1;
			else
				return 0;
		}
		return 0;
	}

	void NEURAL_SAVE(std::string name)
	{
		int length = this->m * this->n * 2;
		int k = 0;
		std::string out_line;
		std::ofstream out(name);
		// information
		out << this->r << ", " << this->m << ", "
			<< this->n << ", " << this->e << ", "
			<< this->Learning_rate << ", " << this->Velocity_rate << ", "
			<< (int)this->Active_type << ", " << (int)this->BACK_type << ", \n";
		for (int j = 0; j < this->n; j++)
		{
			out << "W[" << j << "]" << ", ";
			out << "B[" << j << "]" << ", ";
		}
		for (unsigned int i = 0; i < length; i++)
		{
			if (i % (this->n * 2) == 0)
				out << std::endl << this->Hidden_layer.vectors[i] << ", ";
			else
				out << this->Hidden_layer.vectors[i] << ", ";
		}
		out.close();
	}
	bool NEURAL_LOAD(std::string name)
	{
		std::ifstream NEURAL_file;
		std::string str;
		NEURAL_file.open(name, std::ios::in);
		if (NEURAL_file)
		{
			//information
			getline(NEURAL_file, str, ',');
			this->r = stoi(str);

			getline(NEURAL_file, str, ',');
			this->m = stoi(str);

			getline(NEURAL_file, str, ',');
			this->n = stoi(str);

			getline(NEURAL_file, str, ',');
			this->e = stoi(str);

			getline(NEURAL_file, str, ',');
			this->Learning_rate = atof(str.c_str());

			getline(NEURAL_file, str, ',');
			this->Velocity_rate = atof(str.c_str());

			getline(NEURAL_file, str, ',');
			this->Active_type = stoi(str);

			getline(NEURAL_file, str, ',');
			this->BACK_type = stoi(str);
			getline(NEURAL_file, str, ',');

			free(this->Input_layer.vectors);
			free(this->Hidden_layer.vectors);
			free(this->Z_layer.vectors);
			free(this->A_layer.vectors);
			free(this->Output_layer.vectors);
			free(this->Momentum_layer.vectors);

			this->Input_layer.m = this->m;
			this->Hidden_layer.m = this->m;
			this->Z_layer.m = this->m;
			this->A_layer.m = this->m;
			this->Output_layer.m = this->m;
			this->Momentum_layer.m = this->m;

			this->Input_layer.n = 1;
			this->Hidden_layer.n = this->n * 2;
			this->Z_layer.n = this->n;
			this->A_layer.n = this->n;
			this->Output_layer.n = 1;
			this->Momentum_layer.n = this->n * 2;

			this->Input_layer.vectors = (vtype*)calloc(this->Input_layer.m * this->Input_layer.n, sizeof(vtype));
			this->Hidden_layer.vectors = (vtype*)calloc(this->Hidden_layer.m * this->Hidden_layer.n, sizeof(vtype));
			this->Z_layer.vectors = (vtype*)calloc(this->Z_layer.m * this->Z_layer.n, sizeof(vtype));
			this->A_layer.vectors = (vtype*)calloc(this->A_layer.m * this->A_layer.n, sizeof(vtype));
			this->Output_layer.vectors = (vtype*)calloc(this->Output_layer.m * this->Output_layer.n, sizeof(vtype));
			this->Momentum_layer.vectors = (vtype*)calloc(this->Momentum_layer.m * this->Momentum_layer.n, sizeof(vtype));



			//ingnolre header
			getline(NEURAL_file, str, '\n');
			unsigned int index = 0;
			unsigned int max = this->Hidden_layer.m * this->Hidden_layer.n;
			while (!NEURAL_file.eof())
			{
				if (index > max)
					break;
				getline(NEURAL_file, str, ',');
				vtype value = atof(str.c_str());
				this->Hidden_layer.vectors[index] = value;
				index++;
			}
			NEURAL_file.close();
		}
		else
		{
			std::string temp = "THERE IS NO " + name + "SUCH FILE";
			std::cout << temp << std::endl;
			return false;
		}
	}
};

//----------Data Tool----------------------------
namespace nfe//neural file namsepace
{
	unsigned int  howmanyletter(std::string str, char src)
	{
		unsigned int count = 0;
		for (const char& si : str)
			(si == src) ? count++ : 0;
		return count;
	}
}

#endif