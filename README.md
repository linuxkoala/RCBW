###abstract
 
RCBW , called the reservation of CPU bandwidth,  is  a management-mechanism for CPU resources  that exploits the original completely fair scheduler to guarantee the runtime of specific groups of processes.
 
The CPU QoS (Quality of Service) ensures optimal allocation of  computing resources for users and prevents resource contention among users. At process level, CPU limit and quota have already been supported on CFS, but CPU reservation is not supported yet. Here the CPU limit is the maximum CPU resources  available to groups of processes. 
 
###usage

Step 1 : To build the kernel with RCBW enabled, the following option must be enabled.
~~~
CONFIG_CFS_BANDWIDTH = y
/* guarantee the runtime of a specific group of processes. */
CONFIG_CFS_BANDWIDTH_RESERVE = y
/* export information about sched_entity and cfs_rq */
CONFIG_RESERVE_DEBUG_INFO = y 
/* Only under uniprocessor environment now */
CONFIG_SMP = n
~~~

Step 2: cgroup 	partition setting.

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

Step 3: Observe the running status of the three process groups through htop. We can see that whatever happens, pid1 always get 40 percent of the CPU resources.

