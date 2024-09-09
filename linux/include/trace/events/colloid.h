#undef TRACE_SYSTEM
#define TRACE_SYSTEM colloid

#if !defined(_TRACE_COLLOID_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_COLLOID_H

#include <linux/types.h>
#include <linux/tracepoint.h>

DECLARE_TRACE(colloid_migrate,
        TP_PROTO(unsigned long nr_to_scan, unsigned long migrate_limit, unsigned long delta_p, unsigned long overall_accesses, bool promotion, unsigned long nr_migrated),
        TP_ARGS(nr_to_scan, migrate_limit, delta_p, overall_accesses, promotion, nr_migrated));

#endif /* _TRACE_COLLOID_H */

/* This part must be outside protection */
#include <trace/define_trace.h>