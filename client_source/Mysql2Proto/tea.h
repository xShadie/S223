#ifdef __cplusplus
extern "C" {
#endif
    int tea_encrypt(unsigned long *dest, const unsigned long *src, const unsigned long *key, int size);
    int tea_decrypt(unsigned long *dest, const unsigned long *src, const unsigned long *key, int size);

#ifdef __cplusplus
};
#endif
