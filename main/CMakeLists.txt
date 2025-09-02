set(includedirs
        .
	VL53LMZ_ULD_API/inc
	examples/
)

idf_component_register(
        SRCS
        "main.c" "platform.c" "vl53lx_test.c"
        "VL53LMZ_ULD_API/src/vl53lmz_api.c"
        "VL53LMZ_ULD_API/src/vl53lmz_plugin_cnh.c"
        "VL53LMZ_ULD_API/src/vl53lmz_plugin_detection_thresholds.c"
        "VL53LMZ_ULD_API/src/vl53lmz_plugin_motion_indicator.c"
        "VL53LMZ_ULD_API/src/vl53lmz_plugin_xtalk.c"
        "examples/Example_1_ranging_basic.c"
        "examples/Example_2_get_set_params.c"
        "examples/Example_3_ranging_modes.c"
        "examples/Example_4_power_modes.c"
        "examples/Example_5_multiple_targets_per_zone.c"
        "examples/Example_6_I2C_and_RAM_optimization.c"
        "examples/Example_7_calibrate_xtalk.c"
        "examples/Example_8_vizualize_xtalk_data.c"
        "examples/Example_9_detection_thresholds.c"
        "examples/Example_10_motion_indicator.c"
        "examples/Example_11_motion_indicator_with_detection_thresholds.c"
        "examples/Example_12_cnh_data.c"
        "examples/Example_13_rtrobot_software.c"
        INCLUDE_DIRS ${includedirs}
)
