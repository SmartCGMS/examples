#pragma once

#include <rtl/FilterLib.h>
#include <rtl/referencedImpl.h>
#include <rtl/SolverLib.h>
#include <rtl/UILib.h>

class CExample_Solver {

	protected:
		static constexpr size_t Default_Max_Generations = 1000;

	protected:
		solver::TSolver_Setup &mSetup;
		double mBest_Metric = std::numeric_limits<double>::max();

		void Evaluate_Solution(const double* solution, solver::TSolver_Progress &progress);

	public:
		CExample_Solver(solver::TSolver_Setup &setup);
		virtual ~CExample_Solver();
	
		HRESULT Solve(solver::TSolver_Progress &progress);
};
