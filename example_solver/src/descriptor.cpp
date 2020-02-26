#include "descriptor.h"
#include "example_solver.h"

#include <iface/DeviceIface.h>
#include <iface/UIIface.h>
#include <lang/dstrings.h>
#include <rtl/descriptor_utils.h>
#include <rtl/UILib.h>
#include <rtl/SolverLib.h>

#include <array>

/*
 * Example solver descriptor block
 */

namespace example_solver {

	const scgms::TSolver_Descriptor descriptor = {
		id,
		L"Example solver",
		false,
		0,
		nullptr
	};
}

/*
 * Array of available solver descriptors
 */

const std::array<scgms::TSolver_Descriptor, 1> solver_descriptions = { example_solver::descriptor };

/*
 * Solver library interface implementations
 */

HRESULT IfaceCalling do_get_solver_descriptors(scgms::TSolver_Descriptor **begin, scgms::TSolver_Descriptor **end) {

	return do_get_descriptors(solver_descriptions, begin, end);
}

HRESULT IfaceCalling do_solve_generic(const GUID *solver_id, solver::TSolver_Setup *setup, solver::TSolver_Progress *progress) {

	if (*solver_id == example_solver::descriptor.id) {
		CExample_Solver solver(*setup);

		return solver.Solve(*progress);
	}

	return E_NOTIMPL;
}
