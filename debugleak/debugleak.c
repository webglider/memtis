#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cgroup.h>
#include <linux/memcontrol.h>
#include <linux/err.h>

struct mem_cgroup *get_mem_cgroup_from_path(const char *path) {
    struct cgroup *cg;
    struct cgroup_subsys_state *css;
    struct mem_cgroup *memcg;

    // Get the cgroup from the path
    cg = cgroup_get_from_path(path);
    if (IS_ERR(cg)) {
        pr_err("Failed to get cgroup from path: %s\n", path);
        return ERR_CAST(cg);
    }

    // Get the memory controller's cgroup_subsys_state for cgroup v2
    css = cgroup_e_css(cg, &memory_cgrp_subsys);
    if (!css) {
        pr_err("Failed to get css for memory cgroup\n");
        cgroup_put(cg);
        return ERR_PTR(-EINVAL);
    }

    // Convert the cgroup_subsys_state to a mem_cgroup
    memcg = mem_cgroup_from_css(css);

    // Release the reference to the cgroup
    cgroup_put(cg);

    return memcg;
}
 
static int debugleak_init(void)
{

    struct mem_cgroup *memcg;

    memcg = get_mem_cgroup_from_path("/sys/fs/cgroup/htmm");
    if(IS_ERR(memcg)) {
        pr_info("memcg not found");
    } else {
        pr_info("found memcg");
    }

    pr_info("debugleak init done");
    return 0;
}
 
static void debugleak_exit(void)
{

   
    pr_info("debugleak exit done");
}
 
module_init(debugleak_init);
module_exit(debugleak_exit);
MODULE_AUTHOR("Midhul");
MODULE_LICENSE("GPL");