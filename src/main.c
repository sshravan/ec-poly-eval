#include <stdio.h>
#include <string.h>
#include <mcl/bn_c384_256.h>

int g_err = 0;

int main()
{
	char buf[1600];
	const char *aStr = "123";
	const char *bStr = "456";
	int ret = mclBn_init(MCL_BLS12_381, MCLBN_COMPILED_TIME_VAR);
	if (ret != 0) {
		printf("err ret=%d\n", ret);
		return 1;
	}
	mclBnFr a, b, ab;
	mclBnG1 P, aP;
	mclBnG2 Q, bQ;
	mclBnGT e, e1, e2;
	mclBnFr_setStr(&a, aStr, strlen(aStr), 10);
	mclBnFr_setStr(&b, bStr, strlen(bStr), 10);
	mclBnFr_mul(&ab, &a, &b);
	mclBnFr_getStr(buf, sizeof(buf), &ab, 10);
	printf("%s x %s = %s\n", aStr, bStr, buf);
	mclBnFr_sub(&a, &a, &b);
	mclBnFr_getStr(buf, sizeof(buf), &a, 10);
	printf("%s - %s = %s\n", aStr, bStr, buf);

	printf("P = %s\n", buf);
	printf("Q = %s\n", buf);

	mclBnG1_mul(&aP, &P, &a);
	mclBnG2_mul(&bQ, &Q, &b);

	mclBn_pairing(&e, &P, &Q);
	printf("e = %s\n", buf);
	mclBnGT_pow(&e1, &e, &a);
	mclBn_pairing(&e2, &aP, &Q);

	mclBnGT_pow(&e1, &e, &b);
	mclBn_pairing(&e2, &P, &bQ);
	if (g_err) {
		printf("err %d\n", g_err);
		return 1;
	} else {
		printf("no err\n");
		return 0;
	}
}
