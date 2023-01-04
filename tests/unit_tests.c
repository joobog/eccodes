/*
 * (C) Copyright 2005- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * In applying this licence, ECMWF does not waive the privileges and immunities granted to it by
 * virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
 */

#include "grib_api_internal.h"

#define STR_EQUAL(s1, s2) (strcmp((s1), (s2)) == 0)
#define NUMBER(x) (sizeof(x) / sizeof(x[0]))

int assertion_caught = 0;
int logging_caught = 0;

typedef enum
{
    IBM_FLOAT,
    IEEE_FLOAT
} FloatRep;

static void compare_doubles(const double d1, const double d2, const double epsilon)
{
    Assert(fabs(d1 - d2) < epsilon);
}

static void check_float_representation(const double val, const double expected, const FloatRep rep)
{
    double out             = 0;
    const double tolerance = 1e-9;
    if (rep == IBM_FLOAT)
        Assert(grib_nearest_smaller_ibm_float(val, &out) == GRIB_SUCCESS);
    else
        Assert(grib_nearest_smaller_ieee_float(val, &out) == GRIB_SUCCESS);

    /*printf("%s: d1=%10.20f, out=%10.20f\n", (rep==IBM_FLOAT)?"ibm":"ieee", val, out);*/

    compare_doubles(out, expected, tolerance);
}

static void test_get_git_sha1()
{
    const char* sha1 = grib_get_git_sha1();
    Assert(sha1 != NULL);
    printf("Testing: test_get_git_sha1... %s\n", sha1);
}

static void test_get_build_date()
{
    const char* bdate = codes_get_build_date();
    Assert(bdate != NULL);
    /* Should be of the format YYYY.MM.DD or empty (not implemented) */
    Assert( strlen(bdate) == 0 || isdigit(bdate[0]) );
    printf("Testing: test_get_build_date... %s\n", bdate);
}

static void test_grib_nearest_smaller_ibmfloat()
{
    printf("Testing: test_grib_nearest_smaller_ibmfloat...\n");
    check_float_representation(-1.0, -1.0, IBM_FLOAT);
    check_float_representation(0.0, 0.0, IBM_FLOAT);
    check_float_representation(1.0, 1.0, IBM_FLOAT);
    check_float_representation(1.1, 1.0999994277954, IBM_FLOAT);
    check_float_representation(10.6, 10.599999427795, IBM_FLOAT);
    check_float_representation(7.85, 7.8499994277954, IBM_FLOAT);
}

static void test_grib_nearest_smaller_ieeefloat()
{
    printf("Testing: test_grib_nearest_smaller_ieeefloat...\n");
    check_float_representation(-1.0, -1.0, IEEE_FLOAT);
    check_float_representation(0.0, 0.0, IEEE_FLOAT);
    check_float_representation(1.0, 1.0, IEEE_FLOAT);
    check_float_representation(1.1, 1.0999999046325, IEEE_FLOAT);
    check_float_representation(10.6, 10.599999427795, IEEE_FLOAT);
    check_float_representation(7.85, 7.8499999046325, IEEE_FLOAT);
}

static void test_gaussian_latitudes(int order)
{
    int ret       = 0;
    const int num = 2 * order;
    double lat1 = 0, lat2 = 0;
    double* lats = (double*)malloc(sizeof(double) * num);
    printf("Testing: test_gaussian_latitudes order=%d...\n", order);
    ret = grib_get_gaussian_latitudes(order, lats);
    Assert(ret == GRIB_SUCCESS);

    lat1 = lats[0];
    lat2 = lats[num - 1];
    /* Check first and last latitudes are the same with opposite sign */
    compare_doubles(lat1, -lat2, 1.0e-6);

    free(lats);
}

static void test_gaussian_latitude_640()
{
    /* Test all latitudes for one specific Gaussian number */
    const int order        = 640;
    const int num          = 2 * order;
    int ret                = 0;
    const double tolerance = 1e-6;
    double* lats           = (double*)malloc(sizeof(double) * num);
    ret                    = grib_get_gaussian_latitudes(order, lats);
    Assert(ret == GRIB_SUCCESS);
    printf("Testing: test_gaussian_latitude_640...\n");

    compare_doubles(lats[0], 89.892396, tolerance);
    compare_doubles(lats[1], 89.753005, tolerance);
    compare_doubles(lats[2], 89.612790, tolerance);
    compare_doubles(lats[3], 89.472390, tolerance);
    compare_doubles(lats[4], 89.331918, tolerance);
    compare_doubles(lats[5], 89.191413, tolerance);
    compare_doubles(lats[6], 89.050889, tolerance);
    compare_doubles(lats[7], 88.910352, tolerance);
    compare_doubles(lats[8], 88.769808, tolerance);
    compare_doubles(lats[9], 88.629259, tolerance);
    compare_doubles(lats[10], 88.488706, tolerance);
    compare_doubles(lats[11], 88.348150, tolerance);
    compare_doubles(lats[12], 88.207592, tolerance);
    compare_doubles(lats[13], 88.067032, tolerance);
    compare_doubles(lats[14], 87.926471, tolerance);
    compare_doubles(lats[15], 87.785908, tolerance);
    compare_doubles(lats[16], 87.645345, tolerance);
    compare_doubles(lats[17], 87.504781, tolerance);
    compare_doubles(lats[18], 87.364216, tolerance);
    compare_doubles(lats[19], 87.223651, tolerance);
    compare_doubles(lats[20], 87.083085, tolerance);
    compare_doubles(lats[21], 86.942519, tolerance);
    compare_doubles(lats[22], 86.801952, tolerance);
    compare_doubles(lats[23], 86.661385, tolerance);
    compare_doubles(lats[24], 86.520818, tolerance);
    compare_doubles(lats[25], 86.380251, tolerance);
    compare_doubles(lats[26], 86.239684, tolerance);
    compare_doubles(lats[27], 86.099116, tolerance);
    compare_doubles(lats[28], 85.958548, tolerance);
    compare_doubles(lats[29], 85.817980, tolerance);
    compare_doubles(lats[30], 85.677412, tolerance);
    compare_doubles(lats[31], 85.536844, tolerance);
    compare_doubles(lats[32], 85.396275, tolerance);
    compare_doubles(lats[33], 85.255707, tolerance);
    compare_doubles(lats[34], 85.115138, tolerance);
    compare_doubles(lats[35], 84.974570, tolerance);
    compare_doubles(lats[36], 84.834001, tolerance);
    compare_doubles(lats[37], 84.693432, tolerance);
    compare_doubles(lats[38], 84.552863, tolerance);
    compare_doubles(lats[39], 84.412294, tolerance);
    compare_doubles(lats[40], 84.271725, tolerance);
    compare_doubles(lats[41], 84.131156, tolerance);
    compare_doubles(lats[42], 83.990587, tolerance);
    compare_doubles(lats[43], 83.850018, tolerance);
    compare_doubles(lats[44], 83.709449, tolerance);
    compare_doubles(lats[45], 83.568880, tolerance);
    compare_doubles(lats[46], 83.428310, tolerance);
    compare_doubles(lats[47], 83.287741, tolerance);
    compare_doubles(lats[48], 83.147172, tolerance);
    compare_doubles(lats[49], 83.006602, tolerance);
    compare_doubles(lats[50], 82.866033, tolerance);
    compare_doubles(lats[51], 82.725464, tolerance);
    compare_doubles(lats[52], 82.584894, tolerance);
    compare_doubles(lats[53], 82.444325, tolerance);
    compare_doubles(lats[54], 82.303755, tolerance);
    compare_doubles(lats[55], 82.163186, tolerance);
    compare_doubles(lats[56], 82.022616, tolerance);
    compare_doubles(lats[57], 81.882047, tolerance);
    compare_doubles(lats[58], 81.741477, tolerance);
    compare_doubles(lats[59], 81.600908, tolerance);
    compare_doubles(lats[60], 81.460338, tolerance);
    compare_doubles(lats[61], 81.319768, tolerance);
    compare_doubles(lats[62], 81.179199, tolerance);
    compare_doubles(lats[63], 81.038629, tolerance);
    compare_doubles(lats[64], 80.898059, tolerance);
    compare_doubles(lats[65], 80.757490, tolerance);
    compare_doubles(lats[66], 80.616920, tolerance);
    compare_doubles(lats[67], 80.476350, tolerance);
    compare_doubles(lats[68], 80.335781, tolerance);
    compare_doubles(lats[69], 80.195211, tolerance);
    compare_doubles(lats[70], 80.054641, tolerance);
    compare_doubles(lats[71], 79.914072, tolerance);
    compare_doubles(lats[72], 79.773502, tolerance);
    compare_doubles(lats[73], 79.632932, tolerance);
    compare_doubles(lats[74], 79.492362, tolerance);
    compare_doubles(lats[75], 79.351792, tolerance);
    compare_doubles(lats[76], 79.211223, tolerance);
    compare_doubles(lats[77], 79.070653, tolerance);
    compare_doubles(lats[78], 78.930083, tolerance);
    compare_doubles(lats[79], 78.789513, tolerance);
    compare_doubles(lats[80], 78.648943, tolerance);
    compare_doubles(lats[81], 78.508374, tolerance);
    compare_doubles(lats[82], 78.367804, tolerance);
    compare_doubles(lats[83], 78.227234, tolerance);
    compare_doubles(lats[84], 78.086664, tolerance);
    compare_doubles(lats[85], 77.946094, tolerance);
    compare_doubles(lats[86], 77.805524, tolerance);
    compare_doubles(lats[87], 77.664955, tolerance);
    compare_doubles(lats[88], 77.524385, tolerance);
    compare_doubles(lats[89], 77.383815, tolerance);
    compare_doubles(lats[90], 77.243245, tolerance);
    compare_doubles(lats[91], 77.102675, tolerance);
    compare_doubles(lats[92], 76.962105, tolerance);
    compare_doubles(lats[93], 76.821535, tolerance);
    compare_doubles(lats[94], 76.680966, tolerance);
    compare_doubles(lats[95], 76.540396, tolerance);
    compare_doubles(lats[96], 76.399826, tolerance);
    compare_doubles(lats[97], 76.259256, tolerance);
    compare_doubles(lats[98], 76.118686, tolerance);
    compare_doubles(lats[99], 75.978116, tolerance);
    compare_doubles(lats[100], 75.837546, tolerance);
    compare_doubles(lats[101], 75.696976, tolerance);
    compare_doubles(lats[102], 75.556406, tolerance);
    compare_doubles(lats[103], 75.415836, tolerance);
    compare_doubles(lats[104], 75.275266, tolerance);
    compare_doubles(lats[105], 75.134697, tolerance);
    compare_doubles(lats[106], 74.994127, tolerance);
    compare_doubles(lats[107], 74.853557, tolerance);
    compare_doubles(lats[108], 74.712987, tolerance);
    compare_doubles(lats[109], 74.572417, tolerance);
    compare_doubles(lats[110], 74.431847, tolerance);
    compare_doubles(lats[111], 74.291277, tolerance);
    compare_doubles(lats[112], 74.150707, tolerance);
    compare_doubles(lats[113], 74.010137, tolerance);
    compare_doubles(lats[114], 73.869567, tolerance);
    compare_doubles(lats[115], 73.728997, tolerance);
    compare_doubles(lats[116], 73.588427, tolerance);
    compare_doubles(lats[117], 73.447857, tolerance);
    compare_doubles(lats[118], 73.307287, tolerance);
    compare_doubles(lats[119], 73.166717, tolerance);
    compare_doubles(lats[120], 73.026147, tolerance);
    compare_doubles(lats[121], 72.885577, tolerance);
    compare_doubles(lats[122], 72.745007, tolerance);
    compare_doubles(lats[123], 72.604437, tolerance);
    compare_doubles(lats[124], 72.463867, tolerance);
    compare_doubles(lats[125], 72.323298, tolerance);
    compare_doubles(lats[126], 72.182728, tolerance);
    compare_doubles(lats[127], 72.042158, tolerance);
    compare_doubles(lats[128], 71.901588, tolerance);
    compare_doubles(lats[129], 71.761018, tolerance);
    compare_doubles(lats[130], 71.620448, tolerance);
    compare_doubles(lats[131], 71.479878, tolerance);
    compare_doubles(lats[132], 71.339308, tolerance);
    compare_doubles(lats[133], 71.198738, tolerance);
    compare_doubles(lats[134], 71.058168, tolerance);
    compare_doubles(lats[135], 70.917598, tolerance);
    compare_doubles(lats[136], 70.777028, tolerance);
    compare_doubles(lats[137], 70.636458, tolerance);
    compare_doubles(lats[138], 70.495888, tolerance);
    compare_doubles(lats[139], 70.355318, tolerance);
    compare_doubles(lats[140], 70.214748, tolerance);
    compare_doubles(lats[141], 70.074178, tolerance);
    compare_doubles(lats[142], 69.933608, tolerance);
    compare_doubles(lats[143], 69.793038, tolerance);
    compare_doubles(lats[144], 69.652468, tolerance);
    compare_doubles(lats[145], 69.511898, tolerance);
    compare_doubles(lats[146], 69.371328, tolerance);
    compare_doubles(lats[147], 69.230758, tolerance);
    compare_doubles(lats[148], 69.090188, tolerance);
    compare_doubles(lats[149], 68.949618, tolerance);
    compare_doubles(lats[150], 68.809048, tolerance);
    compare_doubles(lats[151], 68.668478, tolerance);
    compare_doubles(lats[152], 68.527908, tolerance);
    compare_doubles(lats[153], 68.387338, tolerance);
    compare_doubles(lats[154], 68.246768, tolerance);
    compare_doubles(lats[155], 68.106198, tolerance);
    compare_doubles(lats[156], 67.965628, tolerance);
    compare_doubles(lats[157], 67.825058, tolerance);
    compare_doubles(lats[158], 67.684488, tolerance);
    compare_doubles(lats[159], 67.543918, tolerance);
    compare_doubles(lats[160], 67.403348, tolerance);
    compare_doubles(lats[161], 67.262778, tolerance);
    compare_doubles(lats[162], 67.122208, tolerance);
    compare_doubles(lats[163], 66.981638, tolerance);
    compare_doubles(lats[164], 66.841067, tolerance);
    compare_doubles(lats[165], 66.700497, tolerance);
    compare_doubles(lats[166], 66.559927, tolerance);
    compare_doubles(lats[167], 66.419357, tolerance);
    compare_doubles(lats[168], 66.278787, tolerance);
    compare_doubles(lats[169], 66.138217, tolerance);
    compare_doubles(lats[170], 65.997647, tolerance);
    compare_doubles(lats[171], 65.857077, tolerance);
    compare_doubles(lats[172], 65.716507, tolerance);
    compare_doubles(lats[173], 65.575937, tolerance);
    compare_doubles(lats[174], 65.435367, tolerance);
    compare_doubles(lats[175], 65.294797, tolerance);
    compare_doubles(lats[176], 65.154227, tolerance);
    compare_doubles(lats[177], 65.013657, tolerance);
    compare_doubles(lats[178], 64.873087, tolerance);
    compare_doubles(lats[179], 64.732517, tolerance);
    compare_doubles(lats[180], 64.591947, tolerance);
    compare_doubles(lats[181], 64.451377, tolerance);
    compare_doubles(lats[182], 64.310807, tolerance);
    compare_doubles(lats[183], 64.170237, tolerance);
    compare_doubles(lats[184], 64.029667, tolerance);
    compare_doubles(lats[185], 63.889097, tolerance);
    compare_doubles(lats[186], 63.748527, tolerance);
    compare_doubles(lats[187], 63.607957, tolerance);
    compare_doubles(lats[188], 63.467387, tolerance);
    compare_doubles(lats[189], 63.326817, tolerance);
    compare_doubles(lats[190], 63.186247, tolerance);
    compare_doubles(lats[191], 63.045677, tolerance);
    compare_doubles(lats[192], 62.905107, tolerance);
    compare_doubles(lats[193], 62.764537, tolerance);
    compare_doubles(lats[194], 62.623967, tolerance);
    compare_doubles(lats[195], 62.483397, tolerance);
    compare_doubles(lats[196], 62.342826, tolerance);
    compare_doubles(lats[197], 62.202256, tolerance);
    compare_doubles(lats[198], 62.061686, tolerance);
    compare_doubles(lats[199], 61.921116, tolerance);
    compare_doubles(lats[200], 61.780546, tolerance);
    compare_doubles(lats[201], 61.639976, tolerance);
    compare_doubles(lats[202], 61.499406, tolerance);
    compare_doubles(lats[203], 61.358836, tolerance);
    compare_doubles(lats[204], 61.218266, tolerance);
    compare_doubles(lats[205], 61.077696, tolerance);
    compare_doubles(lats[206], 60.937126, tolerance);
    compare_doubles(lats[207], 60.796556, tolerance);
    compare_doubles(lats[208], 60.655986, tolerance);
    compare_doubles(lats[209], 60.515416, tolerance);
    compare_doubles(lats[210], 60.374846, tolerance);
    compare_doubles(lats[211], 60.234276, tolerance);
    compare_doubles(lats[212], 60.093706, tolerance);
    compare_doubles(lats[213], 59.953136, tolerance);
    compare_doubles(lats[214], 59.812566, tolerance);
    compare_doubles(lats[215], 59.671996, tolerance);
    compare_doubles(lats[216], 59.531426, tolerance);
    compare_doubles(lats[217], 59.390856, tolerance);
    compare_doubles(lats[218], 59.250286, tolerance);
    compare_doubles(lats[219], 59.109715, tolerance);
    compare_doubles(lats[220], 58.969145, tolerance);
    compare_doubles(lats[221], 58.828575, tolerance);
    compare_doubles(lats[222], 58.688005, tolerance);
    compare_doubles(lats[223], 58.547435, tolerance);
    compare_doubles(lats[224], 58.406865, tolerance);
    compare_doubles(lats[225], 58.266295, tolerance);
    compare_doubles(lats[226], 58.125725, tolerance);
    compare_doubles(lats[227], 57.985155, tolerance);
    compare_doubles(lats[228], 57.844585, tolerance);
    compare_doubles(lats[229], 57.704015, tolerance);
    compare_doubles(lats[230], 57.563445, tolerance);
    compare_doubles(lats[231], 57.422875, tolerance);
    compare_doubles(lats[232], 57.282305, tolerance);
    compare_doubles(lats[233], 57.141735, tolerance);
    compare_doubles(lats[234], 57.001165, tolerance);
    compare_doubles(lats[235], 56.860595, tolerance);
    compare_doubles(lats[236], 56.720025, tolerance);
    compare_doubles(lats[237], 56.579455, tolerance);
    compare_doubles(lats[238], 56.438884, tolerance);
    compare_doubles(lats[239], 56.298314, tolerance);
    compare_doubles(lats[240], 56.157744, tolerance);
    compare_doubles(lats[241], 56.017174, tolerance);
    compare_doubles(lats[242], 55.876604, tolerance);
    compare_doubles(lats[243], 55.736034, tolerance);
    compare_doubles(lats[244], 55.595464, tolerance);
    compare_doubles(lats[245], 55.454894, tolerance);
    compare_doubles(lats[246], 55.314324, tolerance);
    compare_doubles(lats[247], 55.173754, tolerance);
    compare_doubles(lats[248], 55.033184, tolerance);
    compare_doubles(lats[249], 54.892614, tolerance);
    compare_doubles(lats[250], 54.752044, tolerance);
    compare_doubles(lats[251], 54.611474, tolerance);
    compare_doubles(lats[252], 54.470904, tolerance);
    compare_doubles(lats[253], 54.330334, tolerance);
    compare_doubles(lats[254], 54.189764, tolerance);
    compare_doubles(lats[255], 54.049194, tolerance);
    compare_doubles(lats[256], 53.908623, tolerance);
    compare_doubles(lats[257], 53.768053, tolerance);
    compare_doubles(lats[258], 53.627483, tolerance);
    compare_doubles(lats[259], 53.486913, tolerance);
    compare_doubles(lats[260], 53.346343, tolerance);
    compare_doubles(lats[261], 53.205773, tolerance);
    compare_doubles(lats[262], 53.065203, tolerance);
    compare_doubles(lats[263], 52.924633, tolerance);
    compare_doubles(lats[264], 52.784063, tolerance);
    compare_doubles(lats[265], 52.643493, tolerance);
    compare_doubles(lats[266], 52.502923, tolerance);
    compare_doubles(lats[267], 52.362353, tolerance);
    compare_doubles(lats[268], 52.221783, tolerance);
    compare_doubles(lats[269], 52.081213, tolerance);
    compare_doubles(lats[270], 51.940643, tolerance);
    compare_doubles(lats[271], 51.800073, tolerance);
    compare_doubles(lats[272], 51.659502, tolerance);
    compare_doubles(lats[273], 51.518932, tolerance);
    compare_doubles(lats[274], 51.378362, tolerance);
    compare_doubles(lats[275], 51.237792, tolerance);
    compare_doubles(lats[276], 51.097222, tolerance);
    compare_doubles(lats[277], 50.956652, tolerance);
    compare_doubles(lats[278], 50.816082, tolerance);
    compare_doubles(lats[279], 50.675512, tolerance);
    compare_doubles(lats[280], 50.534942, tolerance);
    compare_doubles(lats[281], 50.394372, tolerance);
    compare_doubles(lats[282], 50.253802, tolerance);
    compare_doubles(lats[283], 50.113232, tolerance);
    compare_doubles(lats[284], 49.972662, tolerance);
    compare_doubles(lats[285], 49.832092, tolerance);
    compare_doubles(lats[286], 49.691522, tolerance);
    compare_doubles(lats[287], 49.550952, tolerance);
    compare_doubles(lats[288], 49.410381, tolerance);
    compare_doubles(lats[289], 49.269811, tolerance);
    compare_doubles(lats[290], 49.129241, tolerance);
    compare_doubles(lats[291], 48.988671, tolerance);
    compare_doubles(lats[292], 48.848101, tolerance);
    compare_doubles(lats[293], 48.707531, tolerance);
    compare_doubles(lats[294], 48.566961, tolerance);
    compare_doubles(lats[295], 48.426391, tolerance);
    compare_doubles(lats[296], 48.285821, tolerance);
    compare_doubles(lats[297], 48.145251, tolerance);
    compare_doubles(lats[298], 48.004681, tolerance);
    compare_doubles(lats[299], 47.864111, tolerance);
    compare_doubles(lats[300], 47.723541, tolerance);
    compare_doubles(lats[301], 47.582971, tolerance);
    compare_doubles(lats[302], 47.442401, tolerance);
    compare_doubles(lats[303], 47.301830, tolerance);
    compare_doubles(lats[304], 47.161260, tolerance);
    compare_doubles(lats[305], 47.020690, tolerance);
    compare_doubles(lats[306], 46.880120, tolerance);
    compare_doubles(lats[307], 46.739550, tolerance);
    compare_doubles(lats[308], 46.598980, tolerance);
    compare_doubles(lats[309], 46.458410, tolerance);
    compare_doubles(lats[310], 46.317840, tolerance);
    compare_doubles(lats[311], 46.177270, tolerance);
    compare_doubles(lats[312], 46.036700, tolerance);
    compare_doubles(lats[313], 45.896130, tolerance);
    compare_doubles(lats[314], 45.755560, tolerance);
    compare_doubles(lats[315], 45.614990, tolerance);
    compare_doubles(lats[316], 45.474420, tolerance);
    compare_doubles(lats[317], 45.333850, tolerance);
    compare_doubles(lats[318], 45.193279, tolerance);
    compare_doubles(lats[319], 45.052709, tolerance);
    compare_doubles(lats[320], 44.912139, tolerance);
    compare_doubles(lats[321], 44.771569, tolerance);
    compare_doubles(lats[322], 44.630999, tolerance);
    compare_doubles(lats[323], 44.490429, tolerance);
    compare_doubles(lats[324], 44.349859, tolerance);
    compare_doubles(lats[325], 44.209289, tolerance);
    compare_doubles(lats[326], 44.068719, tolerance);
    compare_doubles(lats[327], 43.928149, tolerance);
    compare_doubles(lats[328], 43.787579, tolerance);
    compare_doubles(lats[329], 43.647009, tolerance);
    compare_doubles(lats[330], 43.506439, tolerance);
    compare_doubles(lats[331], 43.365869, tolerance);
    compare_doubles(lats[332], 43.225299, tolerance);
    compare_doubles(lats[333], 43.084728, tolerance);
    compare_doubles(lats[334], 42.944158, tolerance);
    compare_doubles(lats[335], 42.803588, tolerance);
    compare_doubles(lats[336], 42.663018, tolerance);
    compare_doubles(lats[337], 42.522448, tolerance);
    compare_doubles(lats[338], 42.381878, tolerance);
    compare_doubles(lats[339], 42.241308, tolerance);
    compare_doubles(lats[340], 42.100738, tolerance);
    compare_doubles(lats[341], 41.960168, tolerance);
    compare_doubles(lats[342], 41.819598, tolerance);
    compare_doubles(lats[343], 41.679028, tolerance);
    compare_doubles(lats[344], 41.538458, tolerance);
    compare_doubles(lats[345], 41.397888, tolerance);
    compare_doubles(lats[346], 41.257318, tolerance);
    compare_doubles(lats[347], 41.116747, tolerance);
    compare_doubles(lats[348], 40.976177, tolerance);
    compare_doubles(lats[349], 40.835607, tolerance);
    compare_doubles(lats[350], 40.695037, tolerance);
    compare_doubles(lats[351], 40.554467, tolerance);
    compare_doubles(lats[352], 40.413897, tolerance);
    compare_doubles(lats[353], 40.273327, tolerance);
    compare_doubles(lats[354], 40.132757, tolerance);
    compare_doubles(lats[355], 39.992187, tolerance);
    compare_doubles(lats[356], 39.851617, tolerance);
    compare_doubles(lats[357], 39.711047, tolerance);
    compare_doubles(lats[358], 39.570477, tolerance);
    compare_doubles(lats[359], 39.429907, tolerance);
    compare_doubles(lats[360], 39.289337, tolerance);
    compare_doubles(lats[361], 39.148766, tolerance);
    compare_doubles(lats[362], 39.008196, tolerance);
    compare_doubles(lats[363], 38.867626, tolerance);
    compare_doubles(lats[364], 38.727056, tolerance);
    compare_doubles(lats[365], 38.586486, tolerance);
    compare_doubles(lats[366], 38.445916, tolerance);
    compare_doubles(lats[367], 38.305346, tolerance);
    compare_doubles(lats[368], 38.164776, tolerance);
    compare_doubles(lats[369], 38.024206, tolerance);
    compare_doubles(lats[370], 37.883636, tolerance);
    compare_doubles(lats[371], 37.743066, tolerance);
    compare_doubles(lats[372], 37.602496, tolerance);
    compare_doubles(lats[373], 37.461926, tolerance);
    compare_doubles(lats[374], 37.321356, tolerance);
    compare_doubles(lats[375], 37.180785, tolerance);
    compare_doubles(lats[376], 37.040215, tolerance);
    compare_doubles(lats[377], 36.899645, tolerance);
    compare_doubles(lats[378], 36.759075, tolerance);
    compare_doubles(lats[379], 36.618505, tolerance);
    compare_doubles(lats[380], 36.477935, tolerance);
    compare_doubles(lats[381], 36.337365, tolerance);
    compare_doubles(lats[382], 36.196795, tolerance);
    compare_doubles(lats[383], 36.056225, tolerance);
    compare_doubles(lats[384], 35.915655, tolerance);
    compare_doubles(lats[385], 35.775085, tolerance);
    compare_doubles(lats[386], 35.634515, tolerance);
    compare_doubles(lats[387], 35.493945, tolerance);
    compare_doubles(lats[388], 35.353374, tolerance);
    compare_doubles(lats[389], 35.212804, tolerance);
    compare_doubles(lats[390], 35.072234, tolerance);
    compare_doubles(lats[391], 34.931664, tolerance);
    compare_doubles(lats[392], 34.791094, tolerance);
    compare_doubles(lats[393], 34.650524, tolerance);
    compare_doubles(lats[394], 34.509954, tolerance);
    compare_doubles(lats[395], 34.369384, tolerance);
    compare_doubles(lats[396], 34.228814, tolerance);
    compare_doubles(lats[397], 34.088244, tolerance);
    compare_doubles(lats[398], 33.947674, tolerance);
    compare_doubles(lats[399], 33.807104, tolerance);
    compare_doubles(lats[400], 33.666534, tolerance);
    compare_doubles(lats[401], 33.525964, tolerance);
    compare_doubles(lats[402], 33.385393, tolerance);
    compare_doubles(lats[403], 33.244823, tolerance);
    compare_doubles(lats[404], 33.104253, tolerance);
    compare_doubles(lats[405], 32.963683, tolerance);
    compare_doubles(lats[406], 32.823113, tolerance);
    compare_doubles(lats[407], 32.682543, tolerance);
    compare_doubles(lats[408], 32.541973, tolerance);
    compare_doubles(lats[409], 32.401403, tolerance);
    compare_doubles(lats[410], 32.260833, tolerance);
    compare_doubles(lats[411], 32.120263, tolerance);
    compare_doubles(lats[412], 31.979693, tolerance);
    compare_doubles(lats[413], 31.839123, tolerance);
    compare_doubles(lats[414], 31.698553, tolerance);
    compare_doubles(lats[415], 31.557982, tolerance);
    compare_doubles(lats[416], 31.417412, tolerance);
    compare_doubles(lats[417], 31.276842, tolerance);
    compare_doubles(lats[418], 31.136272, tolerance);
    compare_doubles(lats[419], 30.995702, tolerance);
    compare_doubles(lats[420], 30.855132, tolerance);
    compare_doubles(lats[421], 30.714562, tolerance);
    compare_doubles(lats[422], 30.573992, tolerance);
    compare_doubles(lats[423], 30.433422, tolerance);
    compare_doubles(lats[424], 30.292852, tolerance);
    compare_doubles(lats[425], 30.152282, tolerance);
    compare_doubles(lats[426], 30.011712, tolerance);
    compare_doubles(lats[427], 29.871142, tolerance);
    compare_doubles(lats[428], 29.730572, tolerance);
    compare_doubles(lats[429], 29.590001, tolerance);
    compare_doubles(lats[430], 29.449431, tolerance);
    compare_doubles(lats[431], 29.308861, tolerance);
    compare_doubles(lats[432], 29.168291, tolerance);
    compare_doubles(lats[433], 29.027721, tolerance);
    compare_doubles(lats[434], 28.887151, tolerance);
    compare_doubles(lats[435], 28.746581, tolerance);
    compare_doubles(lats[436], 28.606011, tolerance);
    compare_doubles(lats[437], 28.465441, tolerance);
    compare_doubles(lats[438], 28.324871, tolerance);
    compare_doubles(lats[439], 28.184301, tolerance);
    compare_doubles(lats[440], 28.043731, tolerance);
    compare_doubles(lats[441], 27.903161, tolerance);
    compare_doubles(lats[442], 27.762590, tolerance);
    compare_doubles(lats[443], 27.622020, tolerance);
    compare_doubles(lats[444], 27.481450, tolerance);
    compare_doubles(lats[445], 27.340880, tolerance);
    compare_doubles(lats[446], 27.200310, tolerance);
    compare_doubles(lats[447], 27.059740, tolerance);
    compare_doubles(lats[448], 26.919170, tolerance);
    compare_doubles(lats[449], 26.778600, tolerance);
    compare_doubles(lats[450], 26.638030, tolerance);
    compare_doubles(lats[451], 26.497460, tolerance);
    compare_doubles(lats[452], 26.356890, tolerance);
    compare_doubles(lats[453], 26.216320, tolerance);
    compare_doubles(lats[454], 26.075750, tolerance);
    compare_doubles(lats[455], 25.935179, tolerance);
    compare_doubles(lats[456], 25.794609, tolerance);
    compare_doubles(lats[457], 25.654039, tolerance);
    compare_doubles(lats[458], 25.513469, tolerance);
    compare_doubles(lats[459], 25.372899, tolerance);
    compare_doubles(lats[460], 25.232329, tolerance);
    compare_doubles(lats[461], 25.091759, tolerance);
    compare_doubles(lats[462], 24.951189, tolerance);
    compare_doubles(lats[463], 24.810619, tolerance);
    compare_doubles(lats[464], 24.670049, tolerance);
    compare_doubles(lats[465], 24.529479, tolerance);
    compare_doubles(lats[466], 24.388909, tolerance);
    compare_doubles(lats[467], 24.248339, tolerance);
    compare_doubles(lats[468], 24.107768, tolerance);
    compare_doubles(lats[469], 23.967198, tolerance);
    compare_doubles(lats[470], 23.826628, tolerance);
    compare_doubles(lats[471], 23.686058, tolerance);
    compare_doubles(lats[472], 23.545488, tolerance);
    compare_doubles(lats[473], 23.404918, tolerance);
    compare_doubles(lats[474], 23.264348, tolerance);
    compare_doubles(lats[475], 23.123778, tolerance);
    compare_doubles(lats[476], 22.983208, tolerance);
    compare_doubles(lats[477], 22.842638, tolerance);
    compare_doubles(lats[478], 22.702068, tolerance);
    compare_doubles(lats[479], 22.561498, tolerance);
    compare_doubles(lats[480], 22.420928, tolerance);
    compare_doubles(lats[481], 22.280357, tolerance);
    compare_doubles(lats[482], 22.139787, tolerance);
    compare_doubles(lats[483], 21.999217, tolerance);
    compare_doubles(lats[484], 21.858647, tolerance);
    compare_doubles(lats[485], 21.718077, tolerance);
    compare_doubles(lats[486], 21.577507, tolerance);
    compare_doubles(lats[487], 21.436937, tolerance);
    compare_doubles(lats[488], 21.296367, tolerance);
    compare_doubles(lats[489], 21.155797, tolerance);
    compare_doubles(lats[490], 21.015227, tolerance);
    compare_doubles(lats[491], 20.874657, tolerance);
    compare_doubles(lats[492], 20.734087, tolerance);
    compare_doubles(lats[493], 20.593517, tolerance);
    compare_doubles(lats[494], 20.452946, tolerance);
    compare_doubles(lats[495], 20.312376, tolerance);
    compare_doubles(lats[496], 20.171806, tolerance);
    compare_doubles(lats[497], 20.031236, tolerance);
    compare_doubles(lats[498], 19.890666, tolerance);
    compare_doubles(lats[499], 19.750096, tolerance);
    compare_doubles(lats[500], 19.609526, tolerance);
    compare_doubles(lats[501], 19.468956, tolerance);
    compare_doubles(lats[502], 19.328386, tolerance);
    compare_doubles(lats[503], 19.187816, tolerance);
    compare_doubles(lats[504], 19.047246, tolerance);
    compare_doubles(lats[505], 18.906676, tolerance);
    compare_doubles(lats[506], 18.766106, tolerance);
    compare_doubles(lats[507], 18.625535, tolerance);
    compare_doubles(lats[508], 18.484965, tolerance);
    compare_doubles(lats[509], 18.344395, tolerance);
    compare_doubles(lats[510], 18.203825, tolerance);
    compare_doubles(lats[511], 18.063255, tolerance);
    compare_doubles(lats[512], 17.922685, tolerance);
    compare_doubles(lats[513], 17.782115, tolerance);
    compare_doubles(lats[514], 17.641545, tolerance);
    compare_doubles(lats[515], 17.500975, tolerance);
    compare_doubles(lats[516], 17.360405, tolerance);
    compare_doubles(lats[517], 17.219835, tolerance);
    compare_doubles(lats[518], 17.079265, tolerance);
    compare_doubles(lats[519], 16.938694, tolerance);
    compare_doubles(lats[520], 16.798124, tolerance);
    compare_doubles(lats[521], 16.657554, tolerance);
    compare_doubles(lats[522], 16.516984, tolerance);
    compare_doubles(lats[523], 16.376414, tolerance);
    compare_doubles(lats[524], 16.235844, tolerance);
    compare_doubles(lats[525], 16.095274, tolerance);
    compare_doubles(lats[526], 15.954704, tolerance);
    compare_doubles(lats[527], 15.814134, tolerance);
    compare_doubles(lats[528], 15.673564, tolerance);
    compare_doubles(lats[529], 15.532994, tolerance);
    compare_doubles(lats[530], 15.392424, tolerance);
    compare_doubles(lats[531], 15.251854, tolerance);
    compare_doubles(lats[532], 15.111283, tolerance);
    compare_doubles(lats[533], 14.970713, tolerance);
    compare_doubles(lats[534], 14.830143, tolerance);
    compare_doubles(lats[535], 14.689573, tolerance);
    compare_doubles(lats[536], 14.549003, tolerance);
    compare_doubles(lats[537], 14.408433, tolerance);
    compare_doubles(lats[538], 14.267863, tolerance);
    compare_doubles(lats[539], 14.127293, tolerance);
    compare_doubles(lats[540], 13.986723, tolerance);
    compare_doubles(lats[541], 13.846153, tolerance);
    compare_doubles(lats[542], 13.705583, tolerance);
    compare_doubles(lats[543], 13.565013, tolerance);
    compare_doubles(lats[544], 13.424443, tolerance);
    compare_doubles(lats[545], 13.283872, tolerance);
    compare_doubles(lats[546], 13.143302, tolerance);
    compare_doubles(lats[547], 13.002732, tolerance);
    compare_doubles(lats[548], 12.862162, tolerance);
    compare_doubles(lats[549], 12.721592, tolerance);
    compare_doubles(lats[550], 12.581022, tolerance);
    compare_doubles(lats[551], 12.440452, tolerance);
    compare_doubles(lats[552], 12.299882, tolerance);
    compare_doubles(lats[553], 12.159312, tolerance);
    compare_doubles(lats[554], 12.018742, tolerance);
    compare_doubles(lats[555], 11.878172, tolerance);
    compare_doubles(lats[556], 11.737602, tolerance);
    compare_doubles(lats[557], 11.597032, tolerance);
    compare_doubles(lats[558], 11.456461, tolerance);
    compare_doubles(lats[559], 11.315891, tolerance);
    compare_doubles(lats[560], 11.175321, tolerance);
    compare_doubles(lats[561], 11.034751, tolerance);
    compare_doubles(lats[562], 10.894181, tolerance);
    compare_doubles(lats[563], 10.753611, tolerance);
    compare_doubles(lats[564], 10.613041, tolerance);
    compare_doubles(lats[565], 10.472471, tolerance);
    compare_doubles(lats[566], 10.331901, tolerance);
    compare_doubles(lats[567], 10.191331, tolerance);
    compare_doubles(lats[568], 10.050761, tolerance);
    compare_doubles(lats[569], 9.910191, tolerance);
    compare_doubles(lats[570], 9.769620, tolerance);
    compare_doubles(lats[571], 9.629050, tolerance);
    compare_doubles(lats[572], 9.488480, tolerance);
    compare_doubles(lats[573], 9.347910, tolerance);
    compare_doubles(lats[574], 9.207340, tolerance);
    compare_doubles(lats[575], 9.066770, tolerance);
    compare_doubles(lats[576], 8.926200, tolerance);
    compare_doubles(lats[577], 8.785630, tolerance);
    compare_doubles(lats[578], 8.645060, tolerance);
    compare_doubles(lats[579], 8.504490, tolerance);
    compare_doubles(lats[580], 8.363920, tolerance);
    compare_doubles(lats[581], 8.223350, tolerance);
    compare_doubles(lats[582], 8.082780, tolerance);
    compare_doubles(lats[583], 7.942209, tolerance);
    compare_doubles(lats[584], 7.801639, tolerance);
    compare_doubles(lats[585], 7.661069, tolerance);
    compare_doubles(lats[586], 7.520499, tolerance);
    compare_doubles(lats[587], 7.379929, tolerance);
    compare_doubles(lats[588], 7.239359, tolerance);
    compare_doubles(lats[589], 7.098789, tolerance);
    compare_doubles(lats[590], 6.958219, tolerance);
    compare_doubles(lats[591], 6.817649, tolerance);
    compare_doubles(lats[592], 6.677079, tolerance);
    compare_doubles(lats[593], 6.536509, tolerance);
    compare_doubles(lats[594], 6.395939, tolerance);
    compare_doubles(lats[595], 6.255369, tolerance);
    compare_doubles(lats[596], 6.114798, tolerance);
    compare_doubles(lats[597], 5.974228, tolerance);
    compare_doubles(lats[598], 5.833658, tolerance);
    compare_doubles(lats[599], 5.693088, tolerance);
    compare_doubles(lats[600], 5.552518, tolerance);
    compare_doubles(lats[601], 5.411948, tolerance);
    compare_doubles(lats[602], 5.271378, tolerance);
    compare_doubles(lats[603], 5.130808, tolerance);
    compare_doubles(lats[604], 4.990238, tolerance);
    compare_doubles(lats[605], 4.849668, tolerance);
    compare_doubles(lats[606], 4.709098, tolerance);
    compare_doubles(lats[607], 4.568528, tolerance);
    compare_doubles(lats[608], 4.427957, tolerance);
    compare_doubles(lats[609], 4.287387, tolerance);
    compare_doubles(lats[610], 4.146817, tolerance);
    compare_doubles(lats[611], 4.006247, tolerance);
    compare_doubles(lats[612], 3.865677, tolerance);
    compare_doubles(lats[613], 3.725107, tolerance);
    compare_doubles(lats[614], 3.584537, tolerance);
    compare_doubles(lats[615], 3.443967, tolerance);
    compare_doubles(lats[616], 3.303397, tolerance);
    compare_doubles(lats[617], 3.162827, tolerance);
    compare_doubles(lats[618], 3.022257, tolerance);
    compare_doubles(lats[619], 2.881687, tolerance);
    compare_doubles(lats[620], 2.741117, tolerance);
    compare_doubles(lats[621], 2.600546, tolerance);
    compare_doubles(lats[622], 2.459976, tolerance);
    compare_doubles(lats[623], 2.319406, tolerance);
    compare_doubles(lats[624], 2.178836, tolerance);
    compare_doubles(lats[625], 2.038266, tolerance);
    compare_doubles(lats[626], 1.897696, tolerance);
    compare_doubles(lats[627], 1.757126, tolerance);
    compare_doubles(lats[628], 1.616556, tolerance);
    compare_doubles(lats[629], 1.475986, tolerance);
    compare_doubles(lats[630], 1.335416, tolerance);
    compare_doubles(lats[631], 1.194846, tolerance);
    compare_doubles(lats[632], 1.054276, tolerance);
    compare_doubles(lats[633], 0.913706, tolerance);
    compare_doubles(lats[634], 0.773135, tolerance);
    compare_doubles(lats[635], 0.632565, tolerance);
    compare_doubles(lats[636], 0.491995, tolerance);
    compare_doubles(lats[637], 0.351425, tolerance);
    compare_doubles(lats[638], 0.210855, tolerance);
    compare_doubles(lats[639], 0.070285, tolerance);
    compare_doubles(lats[640], -0.070285, tolerance);
    compare_doubles(lats[641], -0.210855, tolerance);
    compare_doubles(lats[642], -0.351425, tolerance);
    compare_doubles(lats[643], -0.491995, tolerance);
    compare_doubles(lats[644], -0.632565, tolerance);
    compare_doubles(lats[645], -0.773135, tolerance);
    compare_doubles(lats[646], -0.913706, tolerance);
    compare_doubles(lats[647], -1.054276, tolerance);
    compare_doubles(lats[648], -1.194846, tolerance);
    compare_doubles(lats[649], -1.335416, tolerance);
    compare_doubles(lats[650], -1.475986, tolerance);
    compare_doubles(lats[651], -1.616556, tolerance);
    compare_doubles(lats[652], -1.757126, tolerance);
    compare_doubles(lats[653], -1.897696, tolerance);
    compare_doubles(lats[654], -2.038266, tolerance);
    compare_doubles(lats[655], -2.178836, tolerance);
    compare_doubles(lats[656], -2.319406, tolerance);
    compare_doubles(lats[657], -2.459976, tolerance);
    compare_doubles(lats[658], -2.600546, tolerance);
    compare_doubles(lats[659], -2.741117, tolerance);
    compare_doubles(lats[660], -2.881687, tolerance);
    compare_doubles(lats[661], -3.022257, tolerance);
    compare_doubles(lats[662], -3.162827, tolerance);
    compare_doubles(lats[663], -3.303397, tolerance);
    compare_doubles(lats[664], -3.443967, tolerance);
    compare_doubles(lats[665], -3.584537, tolerance);
    compare_doubles(lats[666], -3.725107, tolerance);
    compare_doubles(lats[667], -3.865677, tolerance);
    compare_doubles(lats[668], -4.006247, tolerance);
    compare_doubles(lats[669], -4.146817, tolerance);
    compare_doubles(lats[670], -4.287387, tolerance);
    compare_doubles(lats[671], -4.427957, tolerance);
    compare_doubles(lats[672], -4.568528, tolerance);
    compare_doubles(lats[673], -4.709098, tolerance);
    compare_doubles(lats[674], -4.849668, tolerance);
    compare_doubles(lats[675], -4.990238, tolerance);
    compare_doubles(lats[676], -5.130808, tolerance);
    compare_doubles(lats[677], -5.271378, tolerance);
    compare_doubles(lats[678], -5.411948, tolerance);
    compare_doubles(lats[679], -5.552518, tolerance);
    compare_doubles(lats[680], -5.693088, tolerance);
    compare_doubles(lats[681], -5.833658, tolerance);
    compare_doubles(lats[682], -5.974228, tolerance);
    compare_doubles(lats[683], -6.114798, tolerance);
    compare_doubles(lats[684], -6.255369, tolerance);
    compare_doubles(lats[685], -6.395939, tolerance);
    compare_doubles(lats[686], -6.536509, tolerance);
    compare_doubles(lats[687], -6.677079, tolerance);
    compare_doubles(lats[688], -6.817649, tolerance);
    compare_doubles(lats[689], -6.958219, tolerance);
    compare_doubles(lats[690], -7.098789, tolerance);
    compare_doubles(lats[691], -7.239359, tolerance);
    compare_doubles(lats[692], -7.379929, tolerance);
    compare_doubles(lats[693], -7.520499, tolerance);
    compare_doubles(lats[694], -7.661069, tolerance);
    compare_doubles(lats[695], -7.801639, tolerance);
    compare_doubles(lats[696], -7.942209, tolerance);
    compare_doubles(lats[697], -8.082780, tolerance);
    compare_doubles(lats[698], -8.223350, tolerance);
    compare_doubles(lats[699], -8.363920, tolerance);
    compare_doubles(lats[700], -8.504490, tolerance);
    compare_doubles(lats[701], -8.645060, tolerance);
    compare_doubles(lats[702], -8.785630, tolerance);
    compare_doubles(lats[703], -8.926200, tolerance);
    compare_doubles(lats[704], -9.066770, tolerance);
    compare_doubles(lats[705], -9.207340, tolerance);
    compare_doubles(lats[706], -9.347910, tolerance);
    compare_doubles(lats[707], -9.488480, tolerance);
    compare_doubles(lats[708], -9.629050, tolerance);
    compare_doubles(lats[709], -9.769620, tolerance);
    compare_doubles(lats[710], -9.910191, tolerance);
    compare_doubles(lats[711], -10.050761, tolerance);
    compare_doubles(lats[712], -10.191331, tolerance);
    compare_doubles(lats[713], -10.331901, tolerance);
    compare_doubles(lats[714], -10.472471, tolerance);
    compare_doubles(lats[715], -10.613041, tolerance);
    compare_doubles(lats[716], -10.753611, tolerance);
    compare_doubles(lats[717], -10.894181, tolerance);
    compare_doubles(lats[718], -11.034751, tolerance);
    compare_doubles(lats[719], -11.175321, tolerance);
    compare_doubles(lats[720], -11.315891, tolerance);
    compare_doubles(lats[721], -11.456461, tolerance);
    compare_doubles(lats[722], -11.597032, tolerance);
    compare_doubles(lats[723], -11.737602, tolerance);
    compare_doubles(lats[724], -11.878172, tolerance);
    compare_doubles(lats[725], -12.018742, tolerance);
    compare_doubles(lats[726], -12.159312, tolerance);
    compare_doubles(lats[727], -12.299882, tolerance);
    compare_doubles(lats[728], -12.440452, tolerance);
    compare_doubles(lats[729], -12.581022, tolerance);
    compare_doubles(lats[730], -12.721592, tolerance);
    compare_doubles(lats[731], -12.862162, tolerance);
    compare_doubles(lats[732], -13.002732, tolerance);
    compare_doubles(lats[733], -13.143302, tolerance);
    compare_doubles(lats[734], -13.283872, tolerance);
    compare_doubles(lats[735], -13.424443, tolerance);
    compare_doubles(lats[736], -13.565013, tolerance);
    compare_doubles(lats[737], -13.705583, tolerance);
    compare_doubles(lats[738], -13.846153, tolerance);
    compare_doubles(lats[739], -13.986723, tolerance);
    compare_doubles(lats[740], -14.127293, tolerance);
    compare_doubles(lats[741], -14.267863, tolerance);
    compare_doubles(lats[742], -14.408433, tolerance);
    compare_doubles(lats[743], -14.549003, tolerance);
    compare_doubles(lats[744], -14.689573, tolerance);
    compare_doubles(lats[745], -14.830143, tolerance);
    compare_doubles(lats[746], -14.970713, tolerance);
    compare_doubles(lats[747], -15.111283, tolerance);
    compare_doubles(lats[748], -15.251854, tolerance);
    compare_doubles(lats[749], -15.392424, tolerance);
    compare_doubles(lats[750], -15.532994, tolerance);
    compare_doubles(lats[751], -15.673564, tolerance);
    compare_doubles(lats[752], -15.814134, tolerance);
    compare_doubles(lats[753], -15.954704, tolerance);
    compare_doubles(lats[754], -16.095274, tolerance);
    compare_doubles(lats[755], -16.235844, tolerance);
    compare_doubles(lats[756], -16.376414, tolerance);
    compare_doubles(lats[757], -16.516984, tolerance);
    compare_doubles(lats[758], -16.657554, tolerance);
    compare_doubles(lats[759], -16.798124, tolerance);
    compare_doubles(lats[760], -16.938694, tolerance);
    compare_doubles(lats[761], -17.079265, tolerance);
    compare_doubles(lats[762], -17.219835, tolerance);
    compare_doubles(lats[763], -17.360405, tolerance);
    compare_doubles(lats[764], -17.500975, tolerance);
    compare_doubles(lats[765], -17.641545, tolerance);
    compare_doubles(lats[766], -17.782115, tolerance);
    compare_doubles(lats[767], -17.922685, tolerance);
    compare_doubles(lats[768], -18.063255, tolerance);
    compare_doubles(lats[769], -18.203825, tolerance);
    compare_doubles(lats[770], -18.344395, tolerance);
    compare_doubles(lats[771], -18.484965, tolerance);
    compare_doubles(lats[772], -18.625535, tolerance);
    compare_doubles(lats[773], -18.766106, tolerance);
    compare_doubles(lats[774], -18.906676, tolerance);
    compare_doubles(lats[775], -19.047246, tolerance);
    compare_doubles(lats[776], -19.187816, tolerance);
    compare_doubles(lats[777], -19.328386, tolerance);
    compare_doubles(lats[778], -19.468956, tolerance);
    compare_doubles(lats[779], -19.609526, tolerance);
    compare_doubles(lats[780], -19.750096, tolerance);
    compare_doubles(lats[781], -19.890666, tolerance);
    compare_doubles(lats[782], -20.031236, tolerance);
    compare_doubles(lats[783], -20.171806, tolerance);
    compare_doubles(lats[784], -20.312376, tolerance);
    compare_doubles(lats[785], -20.452946, tolerance);
    compare_doubles(lats[786], -20.593517, tolerance);
    compare_doubles(lats[787], -20.734087, tolerance);
    compare_doubles(lats[788], -20.874657, tolerance);
    compare_doubles(lats[789], -21.015227, tolerance);
    compare_doubles(lats[790], -21.155797, tolerance);
    compare_doubles(lats[791], -21.296367, tolerance);
    compare_doubles(lats[792], -21.436937, tolerance);
    compare_doubles(lats[793], -21.577507, tolerance);
    compare_doubles(lats[794], -21.718077, tolerance);
    compare_doubles(lats[795], -21.858647, tolerance);
    compare_doubles(lats[796], -21.999217, tolerance);
    compare_doubles(lats[797], -22.139787, tolerance);
    compare_doubles(lats[798], -22.280357, tolerance);
    compare_doubles(lats[799], -22.420928, tolerance);
    compare_doubles(lats[800], -22.561498, tolerance);
    compare_doubles(lats[801], -22.702068, tolerance);
    compare_doubles(lats[802], -22.842638, tolerance);
    compare_doubles(lats[803], -22.983208, tolerance);
    compare_doubles(lats[804], -23.123778, tolerance);
    compare_doubles(lats[805], -23.264348, tolerance);
    compare_doubles(lats[806], -23.404918, tolerance);
    compare_doubles(lats[807], -23.545488, tolerance);
    compare_doubles(lats[808], -23.686058, tolerance);
    compare_doubles(lats[809], -23.826628, tolerance);
    compare_doubles(lats[810], -23.967198, tolerance);
    compare_doubles(lats[811], -24.107768, tolerance);
    compare_doubles(lats[812], -24.248339, tolerance);
    compare_doubles(lats[813], -24.388909, tolerance);
    compare_doubles(lats[814], -24.529479, tolerance);
    compare_doubles(lats[815], -24.670049, tolerance);
    compare_doubles(lats[816], -24.810619, tolerance);
    compare_doubles(lats[817], -24.951189, tolerance);
    compare_doubles(lats[818], -25.091759, tolerance);
    compare_doubles(lats[819], -25.232329, tolerance);
    compare_doubles(lats[820], -25.372899, tolerance);
    compare_doubles(lats[821], -25.513469, tolerance);
    compare_doubles(lats[822], -25.654039, tolerance);
    compare_doubles(lats[823], -25.794609, tolerance);
    compare_doubles(lats[824], -25.935179, tolerance);
    compare_doubles(lats[825], -26.075750, tolerance);
    compare_doubles(lats[826], -26.216320, tolerance);
    compare_doubles(lats[827], -26.356890, tolerance);
    compare_doubles(lats[828], -26.497460, tolerance);
    compare_doubles(lats[829], -26.638030, tolerance);
    compare_doubles(lats[830], -26.778600, tolerance);
    compare_doubles(lats[831], -26.919170, tolerance);
    compare_doubles(lats[832], -27.059740, tolerance);
    compare_doubles(lats[833], -27.200310, tolerance);
    compare_doubles(lats[834], -27.340880, tolerance);
    compare_doubles(lats[835], -27.481450, tolerance);
    compare_doubles(lats[836], -27.622020, tolerance);
    compare_doubles(lats[837], -27.762590, tolerance);
    compare_doubles(lats[838], -27.903161, tolerance);
    compare_doubles(lats[839], -28.043731, tolerance);
    compare_doubles(lats[840], -28.184301, tolerance);
    compare_doubles(lats[841], -28.324871, tolerance);
    compare_doubles(lats[842], -28.465441, tolerance);
    compare_doubles(lats[843], -28.606011, tolerance);
    compare_doubles(lats[844], -28.746581, tolerance);
    compare_doubles(lats[845], -28.887151, tolerance);
    compare_doubles(lats[846], -29.027721, tolerance);
    compare_doubles(lats[847], -29.168291, tolerance);
    compare_doubles(lats[848], -29.308861, tolerance);
    compare_doubles(lats[849], -29.449431, tolerance);
    compare_doubles(lats[850], -29.590001, tolerance);
    compare_doubles(lats[851], -29.730572, tolerance);
    compare_doubles(lats[852], -29.871142, tolerance);
    compare_doubles(lats[853], -30.011712, tolerance);
    compare_doubles(lats[854], -30.152282, tolerance);
    compare_doubles(lats[855], -30.292852, tolerance);
    compare_doubles(lats[856], -30.433422, tolerance);
    compare_doubles(lats[857], -30.573992, tolerance);
    compare_doubles(lats[858], -30.714562, tolerance);
    compare_doubles(lats[859], -30.855132, tolerance);
    compare_doubles(lats[860], -30.995702, tolerance);
    compare_doubles(lats[861], -31.136272, tolerance);
    compare_doubles(lats[862], -31.276842, tolerance);
    compare_doubles(lats[863], -31.417412, tolerance);
    compare_doubles(lats[864], -31.557982, tolerance);
    compare_doubles(lats[865], -31.698553, tolerance);
    compare_doubles(lats[866], -31.839123, tolerance);
    compare_doubles(lats[867], -31.979693, tolerance);
    compare_doubles(lats[868], -32.120263, tolerance);
    compare_doubles(lats[869], -32.260833, tolerance);
    compare_doubles(lats[870], -32.401403, tolerance);
    compare_doubles(lats[871], -32.541973, tolerance);
    compare_doubles(lats[872], -32.682543, tolerance);
    compare_doubles(lats[873], -32.823113, tolerance);
    compare_doubles(lats[874], -32.963683, tolerance);
    compare_doubles(lats[875], -33.104253, tolerance);
    compare_doubles(lats[876], -33.244823, tolerance);
    compare_doubles(lats[877], -33.385393, tolerance);
    compare_doubles(lats[878], -33.525964, tolerance);
    compare_doubles(lats[879], -33.666534, tolerance);
    compare_doubles(lats[880], -33.807104, tolerance);
    compare_doubles(lats[881], -33.947674, tolerance);
    compare_doubles(lats[882], -34.088244, tolerance);
    compare_doubles(lats[883], -34.228814, tolerance);
    compare_doubles(lats[884], -34.369384, tolerance);
    compare_doubles(lats[885], -34.509954, tolerance);
    compare_doubles(lats[886], -34.650524, tolerance);
    compare_doubles(lats[887], -34.791094, tolerance);
    compare_doubles(lats[888], -34.931664, tolerance);
    compare_doubles(lats[889], -35.072234, tolerance);
    compare_doubles(lats[890], -35.212804, tolerance);
    compare_doubles(lats[891], -35.353374, tolerance);
    compare_doubles(lats[892], -35.493945, tolerance);
    compare_doubles(lats[893], -35.634515, tolerance);
    compare_doubles(lats[894], -35.775085, tolerance);
    compare_doubles(lats[895], -35.915655, tolerance);
    compare_doubles(lats[896], -36.056225, tolerance);
    compare_doubles(lats[897], -36.196795, tolerance);
    compare_doubles(lats[898], -36.337365, tolerance);
    compare_doubles(lats[899], -36.477935, tolerance);
    compare_doubles(lats[900], -36.618505, tolerance);
    compare_doubles(lats[901], -36.759075, tolerance);
    compare_doubles(lats[902], -36.899645, tolerance);
    compare_doubles(lats[903], -37.040215, tolerance);
    compare_doubles(lats[904], -37.180785, tolerance);
    compare_doubles(lats[905], -37.321356, tolerance);
    compare_doubles(lats[906], -37.461926, tolerance);
    compare_doubles(lats[907], -37.602496, tolerance);
    compare_doubles(lats[908], -37.743066, tolerance);
    compare_doubles(lats[909], -37.883636, tolerance);
    compare_doubles(lats[910], -38.024206, tolerance);
    compare_doubles(lats[911], -38.164776, tolerance);
    compare_doubles(lats[912], -38.305346, tolerance);
    compare_doubles(lats[913], -38.445916, tolerance);
    compare_doubles(lats[914], -38.586486, tolerance);
    compare_doubles(lats[915], -38.727056, tolerance);
    compare_doubles(lats[916], -38.867626, tolerance);
    compare_doubles(lats[917], -39.008196, tolerance);
    compare_doubles(lats[918], -39.148766, tolerance);
    compare_doubles(lats[919], -39.289337, tolerance);
    compare_doubles(lats[920], -39.429907, tolerance);
    compare_doubles(lats[921], -39.570477, tolerance);
    compare_doubles(lats[922], -39.711047, tolerance);
    compare_doubles(lats[923], -39.851617, tolerance);
    compare_doubles(lats[924], -39.992187, tolerance);
    compare_doubles(lats[925], -40.132757, tolerance);
    compare_doubles(lats[926], -40.273327, tolerance);
    compare_doubles(lats[927], -40.413897, tolerance);
    compare_doubles(lats[928], -40.554467, tolerance);
    compare_doubles(lats[929], -40.695037, tolerance);
    compare_doubles(lats[930], -40.835607, tolerance);
    compare_doubles(lats[931], -40.976177, tolerance);
    compare_doubles(lats[932], -41.116747, tolerance);
    compare_doubles(lats[933], -41.257318, tolerance);
    compare_doubles(lats[934], -41.397888, tolerance);
    compare_doubles(lats[935], -41.538458, tolerance);
    compare_doubles(lats[936], -41.679028, tolerance);
    compare_doubles(lats[937], -41.819598, tolerance);
    compare_doubles(lats[938], -41.960168, tolerance);
    compare_doubles(lats[939], -42.100738, tolerance);
    compare_doubles(lats[940], -42.241308, tolerance);
    compare_doubles(lats[941], -42.381878, tolerance);
    compare_doubles(lats[942], -42.522448, tolerance);
    compare_doubles(lats[943], -42.663018, tolerance);
    compare_doubles(lats[944], -42.803588, tolerance);
    compare_doubles(lats[945], -42.944158, tolerance);
    compare_doubles(lats[946], -43.084728, tolerance);
    compare_doubles(lats[947], -43.225299, tolerance);
    compare_doubles(lats[948], -43.365869, tolerance);
    compare_doubles(lats[949], -43.506439, tolerance);
    compare_doubles(lats[950], -43.647009, tolerance);
    compare_doubles(lats[951], -43.787579, tolerance);
    compare_doubles(lats[952], -43.928149, tolerance);
    compare_doubles(lats[953], -44.068719, tolerance);
    compare_doubles(lats[954], -44.209289, tolerance);
    compare_doubles(lats[955], -44.349859, tolerance);
    compare_doubles(lats[956], -44.490429, tolerance);
    compare_doubles(lats[957], -44.630999, tolerance);
    compare_doubles(lats[958], -44.771569, tolerance);
    compare_doubles(lats[959], -44.912139, tolerance);
    compare_doubles(lats[960], -45.052709, tolerance);
    compare_doubles(lats[961], -45.193279, tolerance);
    compare_doubles(lats[962], -45.333850, tolerance);
    compare_doubles(lats[963], -45.474420, tolerance);
    compare_doubles(lats[964], -45.614990, tolerance);
    compare_doubles(lats[965], -45.755560, tolerance);
    compare_doubles(lats[966], -45.896130, tolerance);
    compare_doubles(lats[967], -46.036700, tolerance);
    compare_doubles(lats[968], -46.177270, tolerance);
    compare_doubles(lats[969], -46.317840, tolerance);
    compare_doubles(lats[970], -46.458410, tolerance);
    compare_doubles(lats[971], -46.598980, tolerance);
    compare_doubles(lats[972], -46.739550, tolerance);
    compare_doubles(lats[973], -46.880120, tolerance);
    compare_doubles(lats[974], -47.020690, tolerance);
    compare_doubles(lats[975], -47.161260, tolerance);
    compare_doubles(lats[976], -47.301830, tolerance);
    compare_doubles(lats[977], -47.442401, tolerance);
    compare_doubles(lats[978], -47.582971, tolerance);
    compare_doubles(lats[979], -47.723541, tolerance);
    compare_doubles(lats[980], -47.864111, tolerance);
    compare_doubles(lats[981], -48.004681, tolerance);
    compare_doubles(lats[982], -48.145251, tolerance);
    compare_doubles(lats[983], -48.285821, tolerance);
    compare_doubles(lats[984], -48.426391, tolerance);
    compare_doubles(lats[985], -48.566961, tolerance);
    compare_doubles(lats[986], -48.707531, tolerance);
    compare_doubles(lats[987], -48.848101, tolerance);
    compare_doubles(lats[988], -48.988671, tolerance);
    compare_doubles(lats[989], -49.129241, tolerance);
    compare_doubles(lats[990], -49.269811, tolerance);
    compare_doubles(lats[991], -49.410381, tolerance);
    compare_doubles(lats[992], -49.550952, tolerance);
    compare_doubles(lats[993], -49.691522, tolerance);
    compare_doubles(lats[994], -49.832092, tolerance);
    compare_doubles(lats[995], -49.972662, tolerance);
    compare_doubles(lats[996], -50.113232, tolerance);
    compare_doubles(lats[997], -50.253802, tolerance);
    compare_doubles(lats[998], -50.394372, tolerance);
    compare_doubles(lats[999], -50.534942, tolerance);
    compare_doubles(lats[1000], -50.675512, tolerance);
    compare_doubles(lats[1001], -50.816082, tolerance);
    compare_doubles(lats[1002], -50.956652, tolerance);
    compare_doubles(lats[1003], -51.097222, tolerance);
    compare_doubles(lats[1004], -51.237792, tolerance);
    compare_doubles(lats[1005], -51.378362, tolerance);
    compare_doubles(lats[1006], -51.518932, tolerance);
    compare_doubles(lats[1007], -51.659502, tolerance);
    compare_doubles(lats[1008], -51.800073, tolerance);
    compare_doubles(lats[1009], -51.940643, tolerance);
    compare_doubles(lats[1010], -52.081213, tolerance);
    compare_doubles(lats[1011], -52.221783, tolerance);
    compare_doubles(lats[1012], -52.362353, tolerance);
    compare_doubles(lats[1013], -52.502923, tolerance);
    compare_doubles(lats[1014], -52.643493, tolerance);
    compare_doubles(lats[1015], -52.784063, tolerance);
    compare_doubles(lats[1016], -52.924633, tolerance);
    compare_doubles(lats[1017], -53.065203, tolerance);
    compare_doubles(lats[1018], -53.205773, tolerance);
    compare_doubles(lats[1019], -53.346343, tolerance);
    compare_doubles(lats[1020], -53.486913, tolerance);
    compare_doubles(lats[1021], -53.627483, tolerance);
    compare_doubles(lats[1022], -53.768053, tolerance);
    compare_doubles(lats[1023], -53.908623, tolerance);
    compare_doubles(lats[1024], -54.049194, tolerance);
    compare_doubles(lats[1025], -54.189764, tolerance);
    compare_doubles(lats[1026], -54.330334, tolerance);
    compare_doubles(lats[1027], -54.470904, tolerance);
    compare_doubles(lats[1028], -54.611474, tolerance);
    compare_doubles(lats[1029], -54.752044, tolerance);
    compare_doubles(lats[1030], -54.892614, tolerance);
    compare_doubles(lats[1031], -55.033184, tolerance);
    compare_doubles(lats[1032], -55.173754, tolerance);
    compare_doubles(lats[1033], -55.314324, tolerance);
    compare_doubles(lats[1034], -55.454894, tolerance);
    compare_doubles(lats[1035], -55.595464, tolerance);
    compare_doubles(lats[1036], -55.736034, tolerance);
    compare_doubles(lats[1037], -55.876604, tolerance);
    compare_doubles(lats[1038], -56.017174, tolerance);
    compare_doubles(lats[1039], -56.157744, tolerance);
    compare_doubles(lats[1040], -56.298314, tolerance);
    compare_doubles(lats[1041], -56.438884, tolerance);
    compare_doubles(lats[1042], -56.579455, tolerance);
    compare_doubles(lats[1043], -56.720025, tolerance);
    compare_doubles(lats[1044], -56.860595, tolerance);
    compare_doubles(lats[1045], -57.001165, tolerance);
    compare_doubles(lats[1046], -57.141735, tolerance);
    compare_doubles(lats[1047], -57.282305, tolerance);
    compare_doubles(lats[1048], -57.422875, tolerance);
    compare_doubles(lats[1049], -57.563445, tolerance);
    compare_doubles(lats[1050], -57.704015, tolerance);
    compare_doubles(lats[1051], -57.844585, tolerance);
    compare_doubles(lats[1052], -57.985155, tolerance);
    compare_doubles(lats[1053], -58.125725, tolerance);
    compare_doubles(lats[1054], -58.266295, tolerance);
    compare_doubles(lats[1055], -58.406865, tolerance);
    compare_doubles(lats[1056], -58.547435, tolerance);
    compare_doubles(lats[1057], -58.688005, tolerance);
    compare_doubles(lats[1058], -58.828575, tolerance);
    compare_doubles(lats[1059], -58.969145, tolerance);
    compare_doubles(lats[1060], -59.109715, tolerance);
    compare_doubles(lats[1061], -59.250286, tolerance);
    compare_doubles(lats[1062], -59.390856, tolerance);
    compare_doubles(lats[1063], -59.531426, tolerance);
    compare_doubles(lats[1064], -59.671996, tolerance);
    compare_doubles(lats[1065], -59.812566, tolerance);
    compare_doubles(lats[1066], -59.953136, tolerance);
    compare_doubles(lats[1067], -60.093706, tolerance);
    compare_doubles(lats[1068], -60.234276, tolerance);
    compare_doubles(lats[1069], -60.374846, tolerance);
    compare_doubles(lats[1070], -60.515416, tolerance);
    compare_doubles(lats[1071], -60.655986, tolerance);
    compare_doubles(lats[1072], -60.796556, tolerance);
    compare_doubles(lats[1073], -60.937126, tolerance);
    compare_doubles(lats[1074], -61.077696, tolerance);
    compare_doubles(lats[1075], -61.218266, tolerance);
    compare_doubles(lats[1076], -61.358836, tolerance);
    compare_doubles(lats[1077], -61.499406, tolerance);
    compare_doubles(lats[1078], -61.639976, tolerance);
    compare_doubles(lats[1079], -61.780546, tolerance);
    compare_doubles(lats[1080], -61.921116, tolerance);
    compare_doubles(lats[1081], -62.061686, tolerance);
    compare_doubles(lats[1082], -62.202256, tolerance);
    compare_doubles(lats[1083], -62.342826, tolerance);
    compare_doubles(lats[1084], -62.483397, tolerance);
    compare_doubles(lats[1085], -62.623967, tolerance);
    compare_doubles(lats[1086], -62.764537, tolerance);
    compare_doubles(lats[1087], -62.905107, tolerance);
    compare_doubles(lats[1088], -63.045677, tolerance);
    compare_doubles(lats[1089], -63.186247, tolerance);
    compare_doubles(lats[1090], -63.326817, tolerance);
    compare_doubles(lats[1091], -63.467387, tolerance);
    compare_doubles(lats[1092], -63.607957, tolerance);
    compare_doubles(lats[1093], -63.748527, tolerance);
    compare_doubles(lats[1094], -63.889097, tolerance);
    compare_doubles(lats[1095], -64.029667, tolerance);
    compare_doubles(lats[1096], -64.170237, tolerance);
    compare_doubles(lats[1097], -64.310807, tolerance);
    compare_doubles(lats[1098], -64.451377, tolerance);
    compare_doubles(lats[1099], -64.591947, tolerance);
    compare_doubles(lats[1100], -64.732517, tolerance);
    compare_doubles(lats[1101], -64.873087, tolerance);
    compare_doubles(lats[1102], -65.013657, tolerance);
    compare_doubles(lats[1103], -65.154227, tolerance);
    compare_doubles(lats[1104], -65.294797, tolerance);
    compare_doubles(lats[1105], -65.435367, tolerance);
    compare_doubles(lats[1106], -65.575937, tolerance);
    compare_doubles(lats[1107], -65.716507, tolerance);
    compare_doubles(lats[1108], -65.857077, tolerance);
    compare_doubles(lats[1109], -65.997647, tolerance);
    compare_doubles(lats[1110], -66.138217, tolerance);
    compare_doubles(lats[1111], -66.278787, tolerance);
    compare_doubles(lats[1112], -66.419357, tolerance);
    compare_doubles(lats[1113], -66.559927, tolerance);
    compare_doubles(lats[1114], -66.700497, tolerance);
    compare_doubles(lats[1115], -66.841067, tolerance);
    compare_doubles(lats[1116], -66.981638, tolerance);
    compare_doubles(lats[1117], -67.122208, tolerance);
    compare_doubles(lats[1118], -67.262778, tolerance);
    compare_doubles(lats[1119], -67.403348, tolerance);
    compare_doubles(lats[1120], -67.543918, tolerance);
    compare_doubles(lats[1121], -67.684488, tolerance);
    compare_doubles(lats[1122], -67.825058, tolerance);
    compare_doubles(lats[1123], -67.965628, tolerance);
    compare_doubles(lats[1124], -68.106198, tolerance);
    compare_doubles(lats[1125], -68.246768, tolerance);
    compare_doubles(lats[1126], -68.387338, tolerance);
    compare_doubles(lats[1127], -68.527908, tolerance);
    compare_doubles(lats[1128], -68.668478, tolerance);
    compare_doubles(lats[1129], -68.809048, tolerance);
    compare_doubles(lats[1130], -68.949618, tolerance);
    compare_doubles(lats[1131], -69.090188, tolerance);
    compare_doubles(lats[1132], -69.230758, tolerance);
    compare_doubles(lats[1133], -69.371328, tolerance);
    compare_doubles(lats[1134], -69.511898, tolerance);
    compare_doubles(lats[1135], -69.652468, tolerance);
    compare_doubles(lats[1136], -69.793038, tolerance);
    compare_doubles(lats[1137], -69.933608, tolerance);
    compare_doubles(lats[1138], -70.074178, tolerance);
    compare_doubles(lats[1139], -70.214748, tolerance);
    compare_doubles(lats[1140], -70.355318, tolerance);
    compare_doubles(lats[1141], -70.495888, tolerance);
    compare_doubles(lats[1142], -70.636458, tolerance);
    compare_doubles(lats[1143], -70.777028, tolerance);
    compare_doubles(lats[1144], -70.917598, tolerance);
    compare_doubles(lats[1145], -71.058168, tolerance);
    compare_doubles(lats[1146], -71.198738, tolerance);
    compare_doubles(lats[1147], -71.339308, tolerance);
    compare_doubles(lats[1148], -71.479878, tolerance);
    compare_doubles(lats[1149], -71.620448, tolerance);
    compare_doubles(lats[1150], -71.761018, tolerance);
    compare_doubles(lats[1151], -71.901588, tolerance);
    compare_doubles(lats[1152], -72.042158, tolerance);
    compare_doubles(lats[1153], -72.182728, tolerance);
    compare_doubles(lats[1154], -72.323298, tolerance);
    compare_doubles(lats[1155], -72.463867, tolerance);
    compare_doubles(lats[1156], -72.604437, tolerance);
    compare_doubles(lats[1157], -72.745007, tolerance);
    compare_doubles(lats[1158], -72.885577, tolerance);
    compare_doubles(lats[1159], -73.026147, tolerance);
    compare_doubles(lats[1160], -73.166717, tolerance);
    compare_doubles(lats[1161], -73.307287, tolerance);
    compare_doubles(lats[1162], -73.447857, tolerance);
    compare_doubles(lats[1163], -73.588427, tolerance);
    compare_doubles(lats[1164], -73.728997, tolerance);
    compare_doubles(lats[1165], -73.869567, tolerance);
    compare_doubles(lats[1166], -74.010137, tolerance);
    compare_doubles(lats[1167], -74.150707, tolerance);
    compare_doubles(lats[1168], -74.291277, tolerance);
    compare_doubles(lats[1169], -74.431847, tolerance);
    compare_doubles(lats[1170], -74.572417, tolerance);
    compare_doubles(lats[1171], -74.712987, tolerance);
    compare_doubles(lats[1172], -74.853557, tolerance);
    compare_doubles(lats[1173], -74.994127, tolerance);
    compare_doubles(lats[1174], -75.134697, tolerance);
    compare_doubles(lats[1175], -75.275266, tolerance);
    compare_doubles(lats[1176], -75.415836, tolerance);
    compare_doubles(lats[1177], -75.556406, tolerance);
    compare_doubles(lats[1178], -75.696976, tolerance);
    compare_doubles(lats[1179], -75.837546, tolerance);
    compare_doubles(lats[1180], -75.978116, tolerance);
    compare_doubles(lats[1181], -76.118686, tolerance);
    compare_doubles(lats[1182], -76.259256, tolerance);
    compare_doubles(lats[1183], -76.399826, tolerance);
    compare_doubles(lats[1184], -76.540396, tolerance);
    compare_doubles(lats[1185], -76.680966, tolerance);
    compare_doubles(lats[1186], -76.821535, tolerance);
    compare_doubles(lats[1187], -76.962105, tolerance);
    compare_doubles(lats[1188], -77.102675, tolerance);
    compare_doubles(lats[1189], -77.243245, tolerance);
    compare_doubles(lats[1190], -77.383815, tolerance);
    compare_doubles(lats[1191], -77.524385, tolerance);
    compare_doubles(lats[1192], -77.664955, tolerance);
    compare_doubles(lats[1193], -77.805524, tolerance);
    compare_doubles(lats[1194], -77.946094, tolerance);
    compare_doubles(lats[1195], -78.086664, tolerance);
    compare_doubles(lats[1196], -78.227234, tolerance);
    compare_doubles(lats[1197], -78.367804, tolerance);
    compare_doubles(lats[1198], -78.508374, tolerance);
    compare_doubles(lats[1199], -78.648943, tolerance);
    compare_doubles(lats[1200], -78.789513, tolerance);
    compare_doubles(lats[1201], -78.930083, tolerance);
    compare_doubles(lats[1202], -79.070653, tolerance);
    compare_doubles(lats[1203], -79.211223, tolerance);
    compare_doubles(lats[1204], -79.351792, tolerance);
    compare_doubles(lats[1205], -79.492362, tolerance);
    compare_doubles(lats[1206], -79.632932, tolerance);
    compare_doubles(lats[1207], -79.773502, tolerance);
    compare_doubles(lats[1208], -79.914072, tolerance);
    compare_doubles(lats[1209], -80.054641, tolerance);
    compare_doubles(lats[1210], -80.195211, tolerance);
    compare_doubles(lats[1211], -80.335781, tolerance);
    compare_doubles(lats[1212], -80.476350, tolerance);
    compare_doubles(lats[1213], -80.616920, tolerance);
    compare_doubles(lats[1214], -80.757490, tolerance);
    compare_doubles(lats[1215], -80.898059, tolerance);
    compare_doubles(lats[1216], -81.038629, tolerance);
    compare_doubles(lats[1217], -81.179199, tolerance);
    compare_doubles(lats[1218], -81.319768, tolerance);
    compare_doubles(lats[1219], -81.460338, tolerance);
    compare_doubles(lats[1220], -81.600908, tolerance);
    compare_doubles(lats[1221], -81.741477, tolerance);
    compare_doubles(lats[1222], -81.882047, tolerance);
    compare_doubles(lats[1223], -82.022616, tolerance);
    compare_doubles(lats[1224], -82.163186, tolerance);
    compare_doubles(lats[1225], -82.303755, tolerance);
    compare_doubles(lats[1226], -82.444325, tolerance);
    compare_doubles(lats[1227], -82.584894, tolerance);
    compare_doubles(lats[1228], -82.725464, tolerance);
    compare_doubles(lats[1229], -82.866033, tolerance);
    compare_doubles(lats[1230], -83.006602, tolerance);
    compare_doubles(lats[1231], -83.147172, tolerance);
    compare_doubles(lats[1232], -83.287741, tolerance);
    compare_doubles(lats[1233], -83.428310, tolerance);
    compare_doubles(lats[1234], -83.568880, tolerance);
    compare_doubles(lats[1235], -83.709449, tolerance);
    compare_doubles(lats[1236], -83.850018, tolerance);
    compare_doubles(lats[1237], -83.990587, tolerance);
    compare_doubles(lats[1238], -84.131156, tolerance);
    compare_doubles(lats[1239], -84.271725, tolerance);
    compare_doubles(lats[1240], -84.412294, tolerance);
    compare_doubles(lats[1241], -84.552863, tolerance);
    compare_doubles(lats[1242], -84.693432, tolerance);
    compare_doubles(lats[1243], -84.834001, tolerance);
    compare_doubles(lats[1244], -84.974570, tolerance);
    compare_doubles(lats[1245], -85.115138, tolerance);
    compare_doubles(lats[1246], -85.255707, tolerance);
    compare_doubles(lats[1247], -85.396275, tolerance);
    compare_doubles(lats[1248], -85.536844, tolerance);
    compare_doubles(lats[1249], -85.677412, tolerance);
    compare_doubles(lats[1250], -85.817980, tolerance);
    compare_doubles(lats[1251], -85.958548, tolerance);
    compare_doubles(lats[1252], -86.099116, tolerance);
    compare_doubles(lats[1253], -86.239684, tolerance);
    compare_doubles(lats[1254], -86.380251, tolerance);
    compare_doubles(lats[1255], -86.520818, tolerance);
    compare_doubles(lats[1256], -86.661385, tolerance);
    compare_doubles(lats[1257], -86.801952, tolerance);
    compare_doubles(lats[1258], -86.942519, tolerance);
    compare_doubles(lats[1259], -87.083085, tolerance);
    compare_doubles(lats[1260], -87.223651, tolerance);
    compare_doubles(lats[1261], -87.364216, tolerance);
    compare_doubles(lats[1262], -87.504781, tolerance);
    compare_doubles(lats[1263], -87.645345, tolerance);
    compare_doubles(lats[1264], -87.785908, tolerance);
    compare_doubles(lats[1265], -87.926471, tolerance);
    compare_doubles(lats[1266], -88.067032, tolerance);
    compare_doubles(lats[1267], -88.207592, tolerance);
    compare_doubles(lats[1268], -88.348150, tolerance);
    compare_doubles(lats[1269], -88.488706, tolerance);
    compare_doubles(lats[1270], -88.629259, tolerance);
    compare_doubles(lats[1271], -88.769808, tolerance);
    compare_doubles(lats[1272], -88.910352, tolerance);
    compare_doubles(lats[1273], -89.050889, tolerance);
    compare_doubles(lats[1274], -89.191413, tolerance);
    compare_doubles(lats[1275], -89.331918, tolerance);
    compare_doubles(lats[1276], -89.472390, tolerance);
    compare_doubles(lats[1277], -89.612790, tolerance);
    compare_doubles(lats[1278], -89.753005, tolerance);
    compare_doubles(lats[1279], -89.892396, tolerance);

    free(lats);
}

static void test_string_splitting()
{
    int i          = 0;
    char input[80] = "Born|To|Be|Wild";
    char** list    = 0;
    printf("Testing: test_string_splitting...\n");

    list           = string_split(input, "|");
    if (!list) { Assert(!"List is NULL"); return; }
    for (i = 0; list[i] != NULL; ++i) {} /* count how many tokens */
    Assert(i == 4);
    if (!list[0] || !STR_EQ(list[0], "Born")) Assert(0);
    if (!list[1] || !STR_EQ(list[1], "To"))   Assert(0);
    if (!list[2] || !STR_EQ(list[2], "Be"))   Assert(0);
    if (!list[3] || !STR_EQ(list[3], "Wild")) Assert(0);
    Assert(list[4] == NULL);
    for (i = 0; list[i] != NULL; ++i) free(list[i]);
    free(list);

    strcpy(input, "12345|a gap|");
    list = string_split(input, "|");
    if (!list) { Assert(0); return; }
    for (i = 0; list[i] != NULL; ++i) {} /* count how many tokens */
    Assert(i == 2);
    if (!list[0] || !STR_EQ(list[0], "12345")) Assert(0);
    if (!list[1] || !STR_EQ(list[1], "a gap")) Assert(0);
    Assert(list[2] == NULL);
    for (i = 0; list[i] != NULL; ++i) free(list[i]);
    free(list);

    strcpy(input, "Steppenwolf");
    list = string_split(input, ",");
    if (!list) { Assert(0); return; }
    for (i = 0; list[i] != NULL; ++i) {} /* count how many tokens */
    Assert(i == 1);
    if (!list[0] || !STR_EQ(list[0], "Steppenwolf")) Assert(0);
    Assert(list[1] == NULL);
    for (i = 0; list[i] != NULL; ++i) free(list[i]);
    free(list);

    /* Note: currently cannot cope with */
    /*  input being NULL */
    /*  input being empty */
    /*  input having several adjacent delimiters e.g. 'A||B|||C' */
}

static void my_assertion_proc(const char* message)
{
    printf("It's OK. I caught the assertion: %s\n", message);
    assertion_caught = 1;
}

static void test_assertion_catching()
{
    char empty[] = "";
    char** list  = 0;
    int i        = 0;
    Assert(assertion_caught == 0);
    codes_set_codes_assertion_failed_proc(&my_assertion_proc);

    printf("Testing: test_assertion_catching...\n");

    /* Do something illegal */
    list = string_split(empty, " ");

    Assert(assertion_caught == 1);

    /* Restore everything */
    codes_set_codes_assertion_failed_proc(NULL);
    assertion_caught = 0;

    for (i = 0; list[i] != NULL; ++i)
        free(list[i]);
    free(list);
}


static void my_logging_proc(const grib_context* c, int level, const char* mesg)
{
    logging_caught = 1;
}
static void test_logging_proc()
{
    grib_context* context = grib_context_get_default();
    Assert(logging_caught == 0);

    /* Override default behaviour */
    grib_context_set_logging_proc(context, my_logging_proc);
    grib_context_log(context, GRIB_LOG_ERROR, "This error will be handled by me");
    Assert(logging_caught == 1);

    /* Restore the logging proc */
    logging_caught = 0;
    grib_context_set_logging_proc(context, NULL);
    grib_context_log(context, GRIB_LOG_ERROR, "This will come out as normal");
    Assert(logging_caught == 0);
}

static void test_concept_condition_strings()
{
    int err           = 0;
    char result[1024] = {0,};
    grib_context* context = NULL;
    grib_handle* h = grib_handle_new_from_samples(context, "GRIB2");

    printf("Testing: test_concept_condition_strings...\n");

    err = get_concept_condition_string(h, "typeOfLevel", NULL, result);
    Assert(!err);
    Assert(strcmp(result, "typeOfFirstFixedSurface=1,typeOfSecondFixedSurface=255") == 0);

    err = get_concept_condition_string(h, "paramId", NULL, result);
    Assert(!err);
    Assert(strcmp(result, "discipline=0,parameterCategory=0,parameterNumber=0") == 0);

    err = get_concept_condition_string(h, "gridType", NULL, result);
    Assert(!err);
    Assert(strcmp(result, "gridDefinitionTemplateNumber=0,PLPresent=0") == 0);

    err = get_concept_condition_string(h, "stepType", NULL, result);
    Assert(!err);
    Assert(strcmp(result, "selectStepTemplateInstant=1,stepTypeInternal=instant") == 0);

    grib_handle_delete(h);
}

static void test_trimming()
{
    char a[] = " Standing  ";
    char b[] = "  Weeping ";
    char c[] = "  Silhouette ";
    char d[] = " The Forest Of October  ";
    char e[] = "\t\n Apostle In Triumph \r ";
    char* pA = a;
    char* pB = b;
    char* pC = c;
    char* pD = d;
    char* pE = e;

    printf("Testing: test_trimming...\n");

    string_lrtrim(&pA, 0, 1); /*right only*/
    Assert( strcmp(pA, " Standing")==0 );

    string_lrtrim(&pB, 1, 0); /*left only*/
    Assert( strcmp(pB, "Weeping ")==0 );

    string_lrtrim(&pC, 1, 1); /*both ends*/
    Assert( strcmp(pC, "Silhouette")==0 );

    string_lrtrim(&pD, 1, 1); /*make sure other spaces are not removed*/
    Assert( strcmp(pD, "The Forest Of October")==0 );

    string_lrtrim(&pE, 1, 1); /* Other chars */
    Assert( strcmp(pE, "Apostle In Triumph")==0 );
}

static void test_string_ends_with()
{
    printf("Testing: test_string_ends_with...\n");
    Assert( string_ends_with("GRIB2.tmpl", "tmpl") == 1 );
    Assert( string_ends_with("GRIB2.tmpl", ".tmpl") == 1 );
    Assert( string_ends_with("", "") == 1 );
    Assert( string_ends_with(".", ".") == 1 );
    Assert( string_ends_with("Bam", "") == 1 );

    Assert( string_ends_with("GRIB2.tmpl", "tmp") == 0 );
    Assert( string_ends_with("GRIB2.tmpl", "tmpl0") == 0 );
    Assert( string_ends_with("GRIB2.tmpl", "1.tmpl") == 0 );
    Assert( string_ends_with("GRIB2.tmpl", " ") == 0 );
}

static void test_gribex_mode()
{
    grib_context* c = grib_context_get_default();
    printf("Testing: test_gribex_mode...\n");

    Assert( grib_get_gribex_mode(c) == 0 ); /* default is OFF */
    grib_gribex_mode_on(c);
    Assert( grib_get_gribex_mode(c) == 1 );
    grib_gribex_mode_off(c);
    Assert( grib_get_gribex_mode(c) == 0 );
}

static void test_grib_binary_search()
{
    double array_asc[] = {-0.1, 33.4, 56.1, 101.8};
    double array_desc[] = {88, 78, 0, -88};
    const size_t idx_asc_max = NUMBER(array_asc) - 1;
    const size_t idx_desc_max = NUMBER(array_desc) - 1;
    size_t idx_upper=0, idx_lower = 0;

    printf("Testing: test_grib_binary_search...\n");

    grib_binary_search(array_asc, idx_asc_max, 56.0, &idx_upper, &idx_lower);
    Assert(idx_lower == 1 && idx_upper == 2);
    grib_binary_search(array_asc, idx_asc_max, 56.1, &idx_upper, &idx_lower);
    Assert(idx_lower == 2 && idx_upper == 3);
    grib_binary_search(array_asc, idx_asc_max, -0.1, &idx_upper, &idx_lower);
    Assert(idx_lower == 0 && idx_upper == 1);

    grib_binary_search(array_desc, idx_desc_max, 88, &idx_upper, &idx_lower);
    Assert(idx_lower == 0 && idx_upper == 1);
    grib_binary_search(array_desc, idx_desc_max, -88, &idx_upper, &idx_lower);
    Assert(idx_lower == 2 && idx_upper == 3);
    grib_binary_search(array_desc, idx_desc_max, 1, &idx_upper, &idx_lower);
    Assert(idx_lower == 1 && idx_upper == 2);
}

static void test_parse_keyval_string()
{
    int err = 0;
    int values_required = 1;
    int count = 0;
    grib_values values1[128] = {0,};
    grib_values values2[128] = {0,};
    grib_values values3[128] = {0,};
    const int max_count = 128;
    char input1[] = "key1=value1,key2!=value2";
    char input2[] = "x=14";
    char input3[] = "mars.level=0.978";

    printf("Testing: parse_keyval_string...\n");

    count = max_count;
    err = parse_keyval_string(NULL, input1,
                              values_required, GRIB_TYPE_UNDEFINED, values1, &count);
    Assert( !err );
    Assert( count == 2 );
    Assert( strcmp(values1[0].name, "key1")==0 );
    Assert( strcmp(values1[0].string_value, "value1")==0 );
    Assert( values1[0].equal == 1 );
    Assert( strcmp(values1[1].name, "key2")==0 );
    Assert( strcmp(values1[1].string_value, "value2")==0 );
    Assert( values1[1].equal == 0 );
    /* Note how the input is modified by the tokenizer (thanks to strtok_r) */
    Assert( strcmp(input1, "key1=value1")==0 );
    free( (void*)values1[0].name );
    free( (void*)values1[1].name );
    free( (void*)values1[0].string_value );
    free( (void*)values1[1].string_value );

    count = max_count;
    err = parse_keyval_string(NULL, input2,
                              values_required, GRIB_TYPE_LONG, values2, &count);
    Assert( !err );
    Assert( count == 1 );
    Assert( strcmp(values2[0].name, "x")==0 );
    Assert( values2[0].long_value == 14 );
    Assert( values2[0].equal == 1 );
    free( (void*)values2[0].name );

    count = max_count;
    err = parse_keyval_string(NULL, input3,
                              values_required, GRIB_TYPE_DOUBLE, values3, &count);
    Assert( !err );
    Assert( count == 1 );
    Assert( strcmp(values3[0].name, "mars.level")==0 );
    free( (void*)values3[0].name );
}


int main(int argc, char** argv)
{
    printf("Doing unit tests. ecCodes version = %ld\n", grib_get_api_version());

    test_logging_proc();
    test_grib_binary_search();
    test_parse_keyval_string();

    test_trimming();
    test_string_ends_with();

    test_get_git_sha1();
    test_get_build_date();
    test_gribex_mode();

    test_concept_condition_strings();

    test_assertion_catching();

    test_gaussian_latitude_640();

    test_gaussian_latitudes(32);
    test_gaussian_latitudes(48);
    test_gaussian_latitudes(80);
    test_gaussian_latitudes(128);
    test_gaussian_latitudes(160);
    test_gaussian_latitudes(200);
    test_gaussian_latitudes(256);
    test_gaussian_latitudes(320);
    test_gaussian_latitudes(400);
    test_gaussian_latitudes(512);
    test_gaussian_latitudes(640);
    test_gaussian_latitudes(1024);
    test_gaussian_latitudes(1280);
    test_gaussian_latitudes(2000);

    test_grib_nearest_smaller_ibmfloat();
    test_grib_nearest_smaller_ieeefloat();

    test_string_splitting();

    return 0;
}
