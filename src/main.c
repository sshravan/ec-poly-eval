#include <mcl/bn_c384_256.h>
#include <stdio.h>
#include <string.h>

// Degree bound is q
void poly_eval(int q) {
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

    // Compute: x = g^a(s)
    mclBnG1 result_1, temp_g1;
    mclBnG1_mul(&result_1, &g1[0], &a_i[0]);
    for (int i = 1; i < q; i++) {
        mclBnG1_mul(&temp_g1, &g1[i], &a_i[i]);
        mclBnG1_add(&result_1, &result_1, &temp_g1);
    }

    // Compute a(s)
    mclBnFr temp_fr;
    mclBnFr exponent = trapdoor;
    mclBnFr sum = a_i[0];
    for (int i = 1; i < q; i++) {
        mclBnFr_mul(&temp_fr, &exponent, &a_i[i]);
        mclBnFr_mul(&exponent, &exponent, &trapdoor);
        mclBnFr_add(&sum, &sum, &temp_fr);
    }

    mclBnG1 result_2;
    mclBnG1_mul(&result_2, &g1[0], &sum);

    printf("IsEqual: %d\n", mclBnG1_isEqual(&result_1, &result_2));
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
