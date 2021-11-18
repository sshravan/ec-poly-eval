#include <mcl/bn_c384_256.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Degree bound is q
void poly_eval(long int q) {
    // Pick a random group generator and a trapdoor
    const char *g1_str = "G1-generator";
    const char *trapdoor_str = "12321903483802913";

    mclBnG1 g1[q];
    mclBnG1_hashAndMapTo(&g1[0], g1_str, strlen(g1_str));

    mclBnFr trapdoor;
    mclBnFr_setStr(&trapdoor, trapdoor_str, strlen(trapdoor_str), 10);

    // Compute: g^(s^i) for i in [0, q)
    for (int i = 1; i < q; i++) {
        mclBnG1_mul(&g1[i], &g1[i - 1], &trapdoor);
    }

    // Sample random coefficients for a q-degree bound polynomial
    mclBnFr a_i[q];
    for (int i = 1; i < q; i++) {
        mclBnFr_setByCSPRNG(&a_i[i]);
    }

    clock_t t;
    double time_taken = 0;
    t = clock();
    // Compute: x = g^a(s)
    mclBnG1 result_1, temp_g1;
    mclBnG1_mul(&result_1, &g1[0], &a_i[0]);
    for (int i = 1; i < q; i++) {
        mclBnG1_mul(&temp_g1, &g1[i], &a_i[i]);
        mclBnG1_add(&result_1, &result_1, &temp_g1);
    }
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Using CRS: %f seconds\n", time_taken);

    // Compute a(s)
    mclBnFr temp_fr;
    mclBnFr exponent = trapdoor;
    mclBnFr sum = a_i[0];
    t = clock();
    for (int i = 1; i < q; i++) {
        mclBnFr_mul(&temp_fr, &exponent, &a_i[i]);
        mclBnFr_mul(&exponent, &exponent, &trapdoor);
        mclBnFr_add(&sum, &sum, &temp_fr);
    }
    mclBnG1 result_2;
    mclBnG1_mul(&result_2, &g1[0], &sum);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Using trapdoor: %f seconds\n", time_taken);
}

int main() {
    int ret = mclBn_init(MCL_BLS12_381, MCLBN_COMPILED_TIME_VAR);
    if (ret != 0) {
        printf("err ret=%d\n", ret);
        return 1;
    }

    poly_eval(1 << 15);
    return 0;
}
