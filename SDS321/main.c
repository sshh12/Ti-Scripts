#define COMMENT_STRING         ""
#define COMMENT_PROGRAM_NAME   "SDS321 Tools"
#define COMMENT_VERSION_STRING "0.1.0.0"
#define COMMENT_VERSION_NUMBER 0,1,0,0
#define COMMENT_BW_ICON \
	{0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000}
#define COMMENT_GRAY_ICON \
	{0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000}, \
	{0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000, \
	 0b0000000000000000}

#define MIN_AMS 200
#define OPTIMIZE_ROM_CALLS
#define USE_TI89

#include <tigcclib.h>

/* ----------------- CONSTANTS ----------------- */
const int numMainOptions = 1;
const char *MainOptions[] = {
	"Distributions"
};

const int numDistributionOptions = 8;
const char *DistributionOptions[] = {
	"Normal",
	"Binomial",
	"Uniform",
	"Poisson",
	"Geometric",
	"NegBinom",
	"HyperGeom",
	"Exponential"
};

/* ----------------- Utils ----------------- */
int prompt_ratio(char* title, char* vals[], int nvals) {
	HANDLE popup = PopupNew(title, 0);
	int i;
	for(i = 0; i < nvals; i++) {
		PopupAddText(popup, -1, vals[i], 0);
	}
	HANDLE execHandle = PopupBegin(popup, 0);
	short result = PopupBeginDo(execHandle, CENTER, CENTER, 0);
	MenuEnd(execHandle);
	return result;
}

float* prompt_nums(char* title, char* vals[], int nvals) {
	char buffer[20 * 10];
	float resp[10];
	int i;
	HANDLE handle = DialogNewSimple(140, 35 + 10 * nvals);
	DialogAdd(handle,0,0,0,15,4);
	DialogAddTitle(handle, title, BT_OK, BT_CANCEL);
	for(i = 0; i < nvals; i++) {
		*(buffer + 20 * i) = '0';
		*(buffer + 20 * i + 1) = '\0';
		DialogAddRequest(handle, 3, 15 + i * 10, vals[i], 20 * i, 20 * (i + 1), 10);
	}
	DialogDo(handle, CENTER, CENTER, buffer, NULL);
	clrscr();
	for(i = 0; i < nvals; i++) {
		resp[i] = atof(buffer + 20 * i);
	}
	return resp;
}

float nCr(float n, float k) { 
	if(k == 0) return 1.0;
  return (n * nCr(n - 1, k - 1)) / k;
} 

float factorial(float n) {
	float res = 1;
	int i;
	for(i = 1; i <= (int)n; i++) {
		res *= i;	
	}
	return res;
}

float clip(float x, float a, float b) {
	return max(min(x, b), a);	
}

float E = 2.7182818284590452353602874713527;

/* ----------------- Funcs ----------------- */
void show_distributions() {
	
	short res = prompt_ratio("Distributions", DistributionOptions, numDistributionOptions);
	
	int nprops;
	float *vals;
	float x, b, n, p, lambda, r, N, M;
	float mean, std, var;
	float pdf, cdf;
	int k;
	
	if(res == 1) { // NORMAL
		nprops = 4;
		char* props[] = {
			"µ=",
			"s=",
			"a=",
			"b="	
		};
		vals = prompt_nums("Normal", props, nprops);
		mean = vals[0];
		std = vals[1];
		x = vals[2];
		b = vals[3];
		var = pow(vals[1], 2);
	}
	
	else if(res == 2) { // BINOMIAL
		nprops = 4;
		char* props[] = {
			"n=",
			"p=",
			"a=",
			"b="	
		};
		vals = prompt_nums("Binomial", props, nprops);
		n = vals[0];
		p = clip(vals[1], 0, 1);
		x = clip(vals[2], 0, n);
		b = clip(vals[3], x, n);
		mean = n * p;
		var = n * p * (1 - p);
		pdf = nCr(n, x) * pow(p, x) * pow(1 - p, n - x);
		cdf = 0;
		for(k = (int)x; k <= (int)b; k++) {
			cdf += nCr(n, (float)k) * pow(p, (float)k) * pow(1 - p, n - (float)k);
		}
	}
	
	else if(res == 4) { // POISSON
		nprops = 3;
		char* props[] = {
			"lambda=",
			"a=",
			"b="
		};
		vals = prompt_nums("Poisson", props, nprops);
		lambda = vals[0];
		x = vals[1];
		b = vals[2];
		mean = lambda;
		var = lambda;
		pdf = pow(E, -lambda) * pow(lambda, x) / factorial(x);
		cdf = 0;
		for(k = (int)x; k <= (int)b; k++) {
			cdf += pow(E, -lambda) * pow(lambda, (float)k) / factorial((float)k);
		}
	}
	
	else if(res == 5) { // Geometric
		nprops = 3;
		char* props[] = {
			"p=",
			"a=",
			"b="
		};
		vals = prompt_nums("Geometric", props, nprops);
		p = vals[0];
		x = vals[1];
		b = vals[2];
		mean = 1 / p;
		var = (1 - p) / (p * p);
		pdf = pow(1 - p, x - 1) * p;
		cdf = 0;
		for(k = (int)x; k <= (int)b; k++) {
			cdf += pow(1 - p, (float)k - 1) * p;
		}
	}
	
	else if(res == 6) { // NegBinom
		nprops = 4;
		char* props[] = {
			"r successes=",
			"p=",
			"a=",
			"b="
		};
		vals = prompt_nums("NegBinom", props, nprops);
		r = vals[0];
		p = vals[1];
		x = vals[2];
		b = vals[3];
		mean = r / p;
		var = r * ((1 - p) / (p * p));
		pdf = nCr(x - 1, r - 1) * pow(1 - p, x - r) * pow(p, r);
		cdf = 0;
		for(k = (int)x; k <= (int)b; k++) {
			cdf += nCr((float)k - 1, r - 1) * pow(1 - p, (float)k - r) * pow(p, r);
		}
	}
	
	else if(res == 7) { // HyperGeom
		nprops = 5;
		char* props[] = {
			"N=",
			"M=",
			"n=",
			"a=",
			"b="
		};
		vals = prompt_nums("HyperGeom", props, nprops);
		N = vals[0];
		M = vals[1];
		n = vals[2];
		x = vals[3];
		b = vals[4];
		mean = M / N;
		var = n * M * (N - M) * (N - n) / (N * N) / (N - 1);
		pdf = nCr(M, x) * nCr(N - M, n - x) / nCr(N, n);
		cdf = 0;
		for(k = (int)x; k <= (int)b; k++) {
			cdf += nCr(M, (float)k) * nCr(N - M, n - (float)k) / nCr(N, n);
		}
	}
	
	else if(res == 8) { // Exponential
		nprops = 3;
		char* props[] = {
			"lambda=",
			"a=",
			"b="
		};
		vals = prompt_nums("Exponential", props, nprops);
		lambda = vals[0];
		x = vals[1];
		b = vals[2];
		mean = 1 / lambda;
		var = 1 / (lambda * lambda);
		pdf = lambda * pow(E, -lambda * x);
		cdf = 0;
		for(k = (int)x; k <= (int)b; k++) {
			cdf += lambda * pow(E, -lambda * (float)k);
		}
	}
	
	printf("µ=%.6f, s^2=%.6f\n", mean, var);
	printf("pdf=%.6f\ncdf=%.6f\n", pdf, cdf);

}

/* ----------------- Main ----------------- */
void _main(void)
{
	clrscr();
	short res = prompt_ratio("SDS321", MainOptions, numMainOptions);
	if(res == 1) {
			show_distributions();
	}
	ngetchx();
}
