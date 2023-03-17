#!/bin/sh
# (C) Copyright 2005- ECMWF.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
# 
# In applying this licence, ECMWF does not waive the privileges and immunities granted to it by
# virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
#

. ./include.ctest.sh

label="grib_ecc-1472_test"
#temp=temp.$label

#$EXEC $test_dir/grib_ecc-1472

#export ECCODES_DEBUG=1
#rm -f $temp

echo $PWD
GRIB_COPY="${tools_dir}/grib_copy"
GRIB_SET="${tools_dir}/grib_set"
GRIB_LS="${tools_dir}/grib_ls"

INFN="${data_dir}/gfs.complex.mvmu.grib2"
OUTFN1="${data_dir}/gfs.complex.mvmu.grib2_1.tmp"
OUTFN2="${data_dir}/gfs.complex.mvmu.grib2_2.tmp"
#gfs.complex.mvmu.grib2

COMMANDS=( 
  "$GRIB_COPY -r ${INFN} ${OUTFN1}" # OK
  "$GRIB_COPY -r ${OUTFN1} ${OUTFN2}" # OK
  "$GRIB_SET -r ${INFN} ${OUTFN1}" # OK
  "$GRIB_SET -rs bitmapPresent=1 ${INFN} ${OUTFN1}" # OK
  "$GRIB_SET -rs bitsPerValue=16 ${INFN} ${OUTFN1}"  # OK
  #"$GRIB_SET -s bitsPerValue=16 ${INFN} ${OUTFN1}"  # FAILS
  #"$GRIB_SET -s packingType=grid_simple ${INFN} ${OUTFN1}" # FAILS: unpack_double() uses grid_simple accessors instead of grid_complex
  #"$GRIB_SET -rs bitmapPresent=1,packingType=grid_simple ${INFN} ${OUTFN1}" # FAILS: unpack_double() uses grid_simple accessors instead of grid_complex
  #"$GRIB_SET -s scaleValuesBy=0.98 gfs.complex.mvmu.grib2 test.grib"
  #"$GRIB_SET -r -s packingType=grid_simple /perm/mod/public/eb/cams_ml1_so3_202301100.grib ${OUTFN1}" # OK
  #"$GRIB_SET -r -s packingType=grid_simple /perm/mod/public/eb/cams_ml1_aermr02_202301100.grib ${OUTFN1}" # OK
)

for ((i = 0; i < ${#COMMANDS[@]}; i++)); do
  COMMAND="${COMMANDS[$i]}"
  echo "COMMAND $COMMAND"
  $COMMAND
  output1=$($GRIB_LS -n statistics $INFN | head -n 3 | tail -n 1)
  output2=$($GRIB_LS -n statistics $OUTFN1 | head -n 3 | tail -n 1)
  if [ "$output1" != "$output2" ]; then
    echo "OUTPUT1: $output1"
    echo "OUTPUT2: $output2"
    return 1
  fi
done

# Clean up
if [ -e "$OUTFN1" ]; then
  rm $OUTFN1
fi
if [ -e "$OUTFN2" ]; then
  rm $OUTFN2
fi
