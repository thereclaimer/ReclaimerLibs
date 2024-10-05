#ifndef R_COMMON_MACROS_HPP
#define R_COMMON_MACROS_HPP

#define r_align_a_to_b(a,b)         (a + b - 1) & ~(b - 1);
#define r_align_size_struct(struct) r_align_a_to_b(sizeof(struct),alignof(struct))

#endif //R_COMMON_MACROS_HPP
