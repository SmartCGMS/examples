/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#include "example_solver.h"

#include <thread>
#include <chrono>
#include <random>

CExample_Solver::CExample_Solver(solver::TSolver_Setup &setup)
	: mSetup(setup) {

}

CExample_Solver::~CExample_Solver() {

}

void CExample_Solver::Evaluate_Solution(const double* solution, solver::TSolver_Progress &progress) {

	solver::TFitness fitness = solver::Nan_Fitness;
	// objective function computes a new metric value of given solution
	BOOL success = mSetup.objective(mSetup.data, 1, solution, fitness.data());

	// NOTE: SmartCGMS supports multi-objective optimalization, but for the sake of simplicity and demonstration, let us use just a single metric value
	const double metricValue = fitness[0];

	// if the new metric value is a valid number and is better than the current best metric, update our current best solution
	if (!std::isnan(metricValue) && metricValue < mBest_Metric) {

		// for our calculations:
		mBest_Metric = metricValue;

		// for GUI to display in a visual element:
		progress.best_metric = fitness;

		// update current best solution in setup
		std::copy(solution, solution + mSetup.problem_size, mSetup.solution);
	}
}

HRESULT CExample_Solver::Solve(solver::TSolver_Progress &progress) {

	// determine generation count
	const size_t genCount = mSetup.max_generations ? mSetup.max_generations : Default_Max_Generations;

	// set max_progress, so the GUI could display correct progress bar value
	progress.max_progress = genCount;

	// initialize (pseudo-)random number generator for our example solver
	static std::random_device rdev;
	std::default_random_engine reng(rdev());
	std::uniform_real_distribution<double> rdist(0.0, 1.0);

	// prepare vector for our temporary solution
	std::vector<double> current_solution(mSetup.problem_size);

	// NOTE: your solver should also respect mSetup.population_size parameter
	//       therefore, there should be a vector of vectors instead of just a single vector;
	//       for the sake of demonstration, we ignore this parameter


	mBest_Metric = std::numeric_limits<double>::max();
	// outer code may help you with some initial parameter estimation called "hints"
	// there may not be any hints present
	for (size_t i = 0; i < mSetup.hint_count; i++) {
		Evaluate_Solution(mSetup.hints[i], progress);
	}

	const double preSolveMetric = mBest_Metric; // currently, we hold the best metric of all hints given

	// iterate through all generations
	// we must respect the "cancelled" parameter of progress, as outer code and another thread may set it to true - in this case,
	// the solver must interrupt as soon as possible and return
	for (size_t generation = 0; generation < genCount && !progress.cancelled; generation++) {

		// we just "emulate" some kind of a solver, so the following lines serves just as an example of "some work" done by a solver
		// of course, one would choose a better strategy, than introduced here

		// generate a random number vector - every vector coordinate within given bounds!
		for (size_t i = 0; i < mSetup.problem_size; i++) {
			current_solution[i] = mSetup.lower_bound[i] + rdist(reng) * (mSetup.upper_bound[i] - mSetup.lower_bound[i]);
		}

		// evaluate newly "computed" solution - this also sets proper status, etc.
		Evaluate_Solution(current_solution.data(), progress);

		// pretend we're doing something - just to demonstrate the progress bar function in GUI
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		// update current progress, so the GUI would update the visual progress bar element
		progress.current_progress = generation;
	}

	// we may return a different code if the solver was cancelled
	if (progress.cancelled) {
		return S_FALSE;
	}

	// the solver did all it could, but it has not found a better solution, that is already known - in this case, always return S_FALSE
	if (preSolveMetric < mBest_Metric) {
		return S_FALSE;
	}

	// a better solution has been found
	return S_OK;
}
