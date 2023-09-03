---
title: "PPTS: VM Resources Oversubscription"
date: "2022-01-22"
categories:
  - "cpp"
tags:
  - "c"
  - "c++"
  - "CPU"
  - "engineering"
  - "Linux"
  - "RAM"
  - "practical-debugging-skills"
  - "practical-performance-tuning-skills"
  - "programming"
  - "software"
  - "VM"
  - "VMware"
---

Oversubscribing physical resources on a VMs host is a common practice among administrators, who try to maximize the utilization of that host machine. Light oversubscription has only minor impact on the performance of applications running inside VMs, but if they push oversubscription too far, then the applications' performance will suffer along with the host. As one of the applications' engineer, it is important for you to understand what oversubscription is and when a performance issue is indeed caused by oversubscription.


# General oversubscription

When a VM is created, it is assigned a certain amount of resources, such as CPU, RAM, storage, and so on. This can be misleading in that it feels like those allocated resources have been set aside and can only be used by that particular VM, which is not the case. In fact, the resources allocated to one VM are just upper limits, and the **hypervisor**, also known as a **virtual machine monitor** or **VMM**, is responsible for sharing and providing virtualized hardware resources to the VMs as they are requested. In additionally, the hypervisor uses different strategies to share different types of resources. I will talk more about that in the later sections.

Basically, it is the way how hypervisor works that makes oversubscription possible. In general, **oversubscription** means that the combined resources (like CPU, memory, and storage) assigned to all of the virtual machines exceed what are actually available on the physical host. On one hand, oversubscription could be beneficial to maximize the value of resources on a host if sized carefully. On the other hand, oversubscription could become dangerous if all of the VMs suddenly requested access to all of the allocated resources, then the total resources actively in use are more than that of the host could provide. Which can lead to major performance problems and stability issues affecting all of the workloads running on the host server.

Then, **how much is too much?** Well, the right answer depends on a number of factors, and there is no one-size-fits-all solutions to this problem. A wise advice for administrators is to start with small amount of oversubscription and do sufficient testing to make sure it works.

Coming up with the right sizing in a virtualized environment is quite challenging. Detecting issues caused by oversubscription is even harder. For an application developer, the best scenario is of course no oversubscription at all, but it is often not the case, especially when the hardware are owned by your customers. The next best choice is to establish a well tested performance baseline in a virtualized environment that is not oversubscribed. This baseline mainly serves two purposes. First, this baseline can be used as a guide for administrators to allocate resources. Basically, administrators have to size individual VM to support the peak needs of your application. Second, this baseline, which is used as the benchmark to compare all future measurements, is critical for solving performance issues whether they are caused by oversubscription or not.


# Memory oversubscription

As already mentioned, the amount of memory allocated to a VM is just an upper limit. One VM starts with only the memory it requires for startup, and if it requires more memory, the hypervisor grants more to it up to the amount it has been allocated. If memory on the host is oversubscribed, the total amount of memory consumed by VMs may continue to grow, and the host may run out of RAM. When the **total free memory** on the host reaches a certain threshold, the hypervisor will start to use various techniques to reclaim memory from VMs. For VMware as an example, this threshold is usually referred to as **mem.minFree**. Basically, as the host memory shortage increases in severity, the memory reclamation has increasingly severe effects on the VM's performance. If the hypervisor is not able to recover enough memory, and the free memory keeps going down, the hypervisor will start **swapping**, which will noticeably degrade the overall performance of the host. Furthermore, as the last resort, the hypervisor will block the VMs from accessing memory that has been allocated but not yet consumed, which could potentially cause the applications or the guest OS to crash.[<sup>\[1\]</sup>](#references)

On a host server, where the memory resource is oversubscribed, administrators need to keep a close eye on the **amount of free memory** of the host. It is recommended to always have at least 400% of the minimum free memory threshold, which is used to trigger memory reclamation, available on the host in order to avoid any issues caused by a memory shortage. In additionally, during certain stage of a memory shortage, memory oversubscription may also be noticeable inside the VMs by monitoring abnormally low **free memory**, high number of **paging** or **swapping** on the guest OS, when comparing to the baseline.[<sup>\[1\]</sup>](#references)


# Storage oversubscription

The situation with oversubscribing storage resources is pretty similar to that of memory. In that, when a VM is created, it is told that it has access to the entirety of the allocated spaces. In reality, however, the hypervisor has the ability to give the VM only the space that it is actually consuming. The hypervisor will continue to provide additional chunks of spaces to a VM if needed, until the total spaces reach the original allocated disk size. This technique is referred to as **thin provisioning**, which is the most common technique for overprovisioning storage resources.

However, thin provisioning also comes with its own risk. If the host is not being monitored closely, that could result in the host runs out of space. Then, the VMs will not be able to access spaces they think they still have, which may lead to data losses or application crashes. Thus, administrators need to watch **the amount of free spaces** on the host carefully, if they choose to oversubscribe storage. On the other hand, when inside VMs, it is difficult to defect when storage oversubscription is causing problems, a better choice for application developers is having a mechanism to recover from such situations after the oversubscription issue has been resolved.


# CPU oversubscription

How CPU resources are shared between VMs is quite different from how memory and storage resources are shared. Basically, oversubscribing CPU will cause increased wait times rather than shortages. As you will see shortly, processing resources are arguably the most interesting type of resources to be oversubscribed. But before we dive into the details, to ease the discussion, we need to clarify a few concepts first:

* A **physical CPU**, or **pCPU**, refers to a single physical core on the host machine.
* A **virtual CPU**, or **vCPU**, refers to a virtualized CPU in a VM.
* A host is considered to be oversubscribed, when it has a vCPU to pCPU ratio beyond 1:1.

One factor that makes CPU oversubscription interesting is **hyperthreading**, where a core that has hyperthreading enabled is considered to have two "logical" processors per core. It is clear that a single CPU with hyperthreading cannot provide the performance of two full CPU cores, and in practice the performance gain from hyperthreading is close to 30%. However, experts disagree with each other about whether a physical core should be counted as two pCPU when hyperthreading is enabled. That affects directly how CPU oversubscription is calculated. My opinion is that for VMs running CPU intensive applications only count physical core regardless hyperthreading, and for others be conservative when counting. That being said, to make things simple, we assume no hyperthreading in the rest part of this article.[<sup>\[1\]</sup>](#references)

Another factor that makes CPU oversubscription interesting is that different hypervisors use slightly different strategies to share processing resources.

For VMware as an example, when a VM gets created, it is assigned certain number of vCPU. The maximum number of vCPU one VM can have equals to the number of pCPUs available on the host. In other words, there must be enough pCPUs available to support the number of vCPUs assigned to an individual VM, otherwise that VM will not boot. When the total number of vCPUs among all the VMs running on the host exceeds available pCPUs, then the hypervisor needs to invoke processor scheduling in order to distribute processor time to VMs that need it. **CPU Ready** is a measure of the time a VM is ready to execute but has to wait for CPU resources from the host. Basically, the larger CPU ready value is, the more the VMs are **not** doing actual works and the more your applications are suffering. In addition, for VMs with more than one vCPUs, the hypervisor schedules all the vCPUs for a VM at the same time. If all the allocated vCPUs for a given VM are not available at the same time, then the VM will be in a state of **co-stop** until the host can **co-schedule** all vCPUs. In its simplest form **co-stop** indicates the amount time from when the first vCPU is able to schedule on a pCPU to the last.[<sup>\[1\]</sup>](#references)

The actual achievable vCPU to pCPU ratio in a specific environment will depend on a number of factors, such as hypervisor version, processor age, workload type, and so on. In general, on a CPU oversubscribed host, administrators need to watch for multiple metrics of the host closely to avoid any performance issues. It is recommended to always keep the host **CPU utilization** below 80%, the **CPU ready** per vCPU below 5%, and the **co-stop** per VM below 3%. Inside VMs, CPU oversubscription may also be noticeable in certain situations by looking at the total **CPU usages**. Keeping the CPU utilization at 80 percent or below is considered a best practice.[<sup>\[1\]</sup>](#references)


# Conclusion

VM resources oversubscription is a common practice among administrators, which can make the best use of the host resources. But you want to ensure oversubscription is done in such a way that you can also maintain high performance. In today's post, we have discussed about oversubscribing different type of resources, and its dangers. We have also signified the metrics need to be monitored on a oversubscribed host.

I hope you found this post useful, if so, you may also want to check out [other articles that also belong to the PPTS series](/ppts). <!-- JEKYLL_RELATIVE_URL_CHECK_SKIP_LINE -->


# References

1. [Guidelines for Overcommitting VMware Resources](https://www.heroix.com/download/Guidelines_for_Overcommitting_VMware_Resources.pdf) by Heroix
1. [\[Monitoring\] – Paging and Swapping in Memory](https://performanceengineeringin.wordpress.com/2019/12/09/monitoring-paging-and-swapping-in-memory/) by Akshay Deshpande
1. [Maximizing VMware Performance and CPU Utilization](https://www.heroix.com/blog/vmware-vcpu-over-allocation/) by Ken Leoni
