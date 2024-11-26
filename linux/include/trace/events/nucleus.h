#undef TRACE_SYSTEM
#define TRACE_SYSTEM nucleus

#if !defined(_TRACE_NUCLEUS_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_NUCLEUS_H

#include <linux/types.h>
#include <linux/tracepoint.h>

TRACE_EVENT(nucleus_split,
        TP_PROTO(unsigned long ehr, unsigned long rhr, unsigned long nr_records, unsigned int memcg_sum_util, unsigned int memcg_num_util, unsigned int avg_accesses_hp, unsigned int memcg_nr_split),
        TP_ARGS(ehr, rhr, nr_records, memcg_sum_util, memcg_num_util, avg_accesses_hp, memcg_nr_split),
        TP_STRUCT__entry(
           __field(unsigned long, ehr)
           __field(unsigned long, rhr)
           __field(unsigned long, nr_records)
           __field(unsigned int, memcg_sum_util)
           __field(unsigned int, memcg_num_util)
           __field(unsigned int, avg_accesses_hp)
           __field(unsigned int, memcg_nr_split)
        ),
        TP_fast_assign(
           __entry->ehr = ehr;
           __entry->rhr = rhr;
           __entry->nr_records = nr_records;
           __entry->memcg_sum_util = memcg_sum_util;
           __entry->memcg_num_util = memcg_num_util;
           __entry->avg_accesses_hp = avg_accesses_hp;
           __entry->memcg_nr_split = memcg_nr_split;
        ),

	    TP_printk("[nucleus] ehr=%lu,rhr=%lu,nr_records=%lu,memcg_sum_util=%u,memcg_num_util=%u,avg_accesses_hp=%u,memcg_nr_split=%u",
	        __entry->ehr, __entry->rhr, __entry->nr_records, __entry->memcg_sum_util, __entry->memcg_num_util, __entry->avg_accesses_hp, __entry->memcg_nr_split)
);

TRACE_EVENT(nucleus_stopsplit,
        TP_PROTO(unsigned long prev_dram_sampled, unsigned long temp_rhr),
        TP_ARGS(prev_dram_sampled, temp_rhr),
        TP_STRUCT__entry(
           __field(unsigned long, prev_dram_sampled)
           __field(unsigned long, temp_rhr)
        ),
        TP_fast_assign(
           __entry->prev_dram_sampled = prev_dram_sampled;
           __entry->temp_rhr = temp_rhr;
        ),

	    TP_printk("[nucleus] prev_dram_sampled=%lu,temp_rhr=%lu",
	        __entry->prev_dram_sampled, __entry->temp_rhr)
);

#endif /* _TRACE_NUCLEUS_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
