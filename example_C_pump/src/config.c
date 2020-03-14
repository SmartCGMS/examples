#include "config.h"

/* Content of the config.ini file.
 * Alternatively, you can use bin2hex
 * or load it from an external file 
 * at the runtime.
 *
 * Use the visualization.ini config
 * to view the result of the C-pump
 * glucose-level regulation.
 */

const char* configuration =
	"; Signal generator\n"
	"[Filter_001_{9EEB3451-2A9D-49C1-BA37-2EC0B00E5E6D}]\n"
	"Model = {8114B2A6-B4B2-4C8D-A029-625CBDB682EF}\n"
	"Feedback_Name =\n"
	"Synchronize_To_Signal = false\n"
	"Synchronization_Signal = {FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF}\n"
	"Stepping = 0.003472\n"
	"Maximum_Time = 3.000000\n"
	"Shutdown_After_Last = true\n"
	"Parameters = 0.005000 0.005000 0.000000 0.100000 0.001000 0.001000 8.000000 10.000000 0.050000 0.010000 0.010000 0.010000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 -0.500000 -10.000000 0.000000 -1.000000 0.000000 0.050000 0.028735 0.028344 0.000050 0.300000 0.100000 0.100000 12.000000 70.000000 0.220000 0.050000 0.050000 0.040000 95.000000 9.200000 220.000000 100.000000 0.000000 0.000000 0.000000 0.000000 0.000000 95.000000 0.000000 0.929000 -0.037000 0.000000 0.023310 0.000000 0.018600 0.350000 0.100000 0.100000 0.050000 1.000000 0.300000 0.300000 18.000000 100.000000 1.000000 1.000000 1.000000 1.000000 200.000000 20.000000 300.000000 300.000000 100.000000 200.000000 150.000000 150.000000 50.000000 300.000000 5.000000 2.000000 0.000000 5.000000 0.041667 0.000000 0.041667 1.500000\n"
	
	
	"; Signal mapping\n"
	"[Filter_002_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {582CFE11-68B9-4E5E-B50B-1978C077A84E}\n"
	"Signal_Dst_Id = {3034568D-F498-455B-AC6A-BCF301F69C9E}\n"
	

	"; Signal mapping\n"
	"[Filter_003_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {E1CD07EF-B079-4911-B79B-D203486101C8}\n"
	"Signal_Dst_Id = {F666F6C2-D7C0-43E8-8EE1-C8CAA8F860E5}\n"
	
	"; Signal decoupling\n"
	"[Filter_004_{0BB71190-8709-4990-97FA-6A4DB679EF1D}]\n"
	"Signal_Src_Id = {F666F6C2-D7C0-43E8-8EE1-C8CAA8F860E5}\n"
	"Signal_Dst_Id = {706E7FDB-8F22-486F-BFA5-6A56D3514209}\n"
	"Remove_from_source = false\n"
	"Condition = is_level_event && ((level >= 3.9) && (level <= 10.0))\n"
	"Collect_statistics = true\n"
	"Output_CSV_file = time_in_range.csv\n"
	
	"; Log\n"
	"[Filter_005_{C0E942B9-3928-4B81-9B43-A347668200BA}]\n"
	"Log_File = C_pump.log";	