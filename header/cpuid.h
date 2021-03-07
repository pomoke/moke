char * cpuid_vendor(void);
u32 cpuid_feature(u32 feature);
u32 cpuid_feature_cx(u32 feature);

enum {
	CPUID_EDX_TSC	= 1 << 4;
};
