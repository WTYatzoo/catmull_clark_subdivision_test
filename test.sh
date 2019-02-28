#! /bin/bash
sub_level=0
##########################################################
project_dir=${HOME}/project/mytest/catmull_clark_subdivision_test
script_dir=${project_dir}/scripts
exe_dir=${project_dir}/build/bin
exe=${exe_dir}/catmull_clark_subdivision
base_output_dir=${project_dir}/example
object=torus
input_path=${base_output_dir}/input_txt/${object}.txt
output_path=${base_output_dir}/output_vtk/${object}_${sub_level}.vtk

MAKEFILE=${script_dir}/makefile
make -f ${MAKEFILE} subdivide EXE=${exe} SUB_LEVEL=${sub_level} INPUT_PATH=${input_path} OUTPUT_PATH=${output_path}
