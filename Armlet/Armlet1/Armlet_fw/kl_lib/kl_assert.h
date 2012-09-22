#ifndef KL_ASSERT_H_
#define KL_ASSERT_H_


#define assert(cond) ((cond) ? (void)0 : assertion_error(__FILE__, __LINE__, #cond))

void assertion_error(const char *filename, int line, const char *cond_string);

extern bool assertion_raised;

#endif /* KL_ASSERT_H_ */
