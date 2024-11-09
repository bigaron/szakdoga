#ifndef MONTECARLO_MONTECARLOPARAMS_HPP
#define MONTECARLO_MONTECARLOPARAMS_HPP

struct MonteCarloParameters {
	int vertexN;
	float eps;
	int sampleN;
	int maxWalk;
};

struct AlgorithmOpts {
	int pass;
	int _padding1, _padding2, _padding3;
};

#endif // !MONTECARLO_MONTECARLOPARAMS_HPP
