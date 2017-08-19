### Abstract
<hr />

RCBW, called as the Reservation of CPU Bandwidth, is a kernel-level CPU resources manager that exploits the original
CFS to guarantee the runtime of speciic groups of processes when other groups compete CPU resources.


### Source code
<hr />
Before we start to study this topic, CPU controller subsystem had not been merged into the mainline of cgroup-v2. Therefore, we choose a LTS version - Linux-4.4.

To build the kernel with RCBW enabled, the following option must be enabled.
~~~
/* Only under uniprocessor environment now */
CONFIG_SMP = n

CONFIG_CFS_BANDWIDTH = y

/* Enabled RCBW */
CONFIG_CFS_BANDWIDTH_RESERVE = y
/* export debug information about RCBW */
CONFIG_RESERVE_DEBUG_INFO = y
~~~

### Usage
<hr />

Step 1: cgroup 	partition setting.

~~~

/*
 * simulate the scenario where a number of process groups
 * compete for CPU resources
 */
mkdir /sys/fs/cgroup/cpu/part
mkdir /sys/fs/cgroup/cpu/part1
mkdir /sys/fs/cgroup/cpu/part2

/* pid1, pid2 and pid3 are endless-loop processes */
echo <pid1> > part/tasks
echo <pid2> > part1/tasks
echo <pid3> > part2/tasks

/*  
 * Within each given "period", whose unit is microseconds and
 * default value is 100 microseconds, a group is allowed to consume only up
 * to "quota" microseconds of CPU time. Please refer to
 * Documentation/scheduler/sched-bwc.txt
 */
echo 30000 > part/cpu.cfs_quota_us
echo 30000 > part1/cpu.cfs_quota_us
echo 40000 > part2/cpu.cfs_quota_us

/* the value of cpu.cfs_quota_us must be greater cpu.cfs_reserve_us */
echo 40000 > part/cpu.cfs_reserve_us

~~~


Step 2: Observe the running status of the three process groups through htop. We can see that whatever happens, pid1 always get 40 percent of the CPU resources.

!!!WARNNING!!! Because RCBW presents many bugs, it is recommended to reboot the machine after running RCBW.
