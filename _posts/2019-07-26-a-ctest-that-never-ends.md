---
title: "A CTest That Never Ends?"
date: "2019-07-26"
categories:
  - "cmake"
tags:
  - "bash"
  - "ctest"
  - "database"
  - "debug"
  - "lsof"
  - "monetdb"
  - "monetdbd"
  - "pipe"
  - "programming"
  - "pstree"
  - "python"
  - "stdout"
---


Recently, I have run into an issue with a python test script which is run by CTest. The test runs normally by itself, but it never terminates until its timeout is reached if launched by CTest. In this post, I'm going to show you how to reproduce this issue first, then how I tackled this problem, and, of course, the solution at the end.

**Update:** As it turns out, this issue seems to be a defect with **MonetDB v11.35.19**. And it has already been fixed in a later version. At least, it is no longer reproducible with **MonetDB v11.43.15** by the method described in this post.

# A Little Background

I encountered this issue when I was working on a project which use **MonetDB** as its data store. Long story short, my task was to write a suite of tests which all share the same instance of MonetDB server. The way I chose to accomplish this is to use **CTest's Test Fixtures**. The issue itself has nothing to do with test fixtures. The only thing you need to know is that fixtures are a suite of otherwise normal tests that run in particular order. In my case, I have a setup test which always runs the first to start the MonetDB instance that is needed by the other tests. As well, a cleanup test which always runs the last to stop the DB server. And it is the setup test where I noticed this CTest issue.


# The issue

Alright, without further ado, here is the buggy code:

```python
{% include src/2019-07-26-ctest-that-never-ends/buggy_setup_db.py %}
```

This code simply creates and starts the MonetDB server that will be needed by the other tests. Of course, this is not a quite useful test, as it doesn't really test anything. However, this is intended so that I can show you the minimum code which can reproduce the bug. As already mentioned, this python script should work just fine by itself, so let's try it:

```bash
./buggy_setup_db.py
```

**buggy_setup_db.py** is the name of the above mentioned test script. Running this command should return immediately after the MonetDB server gets started, which usually takes only a few seconds for an empty DB farm. If you'd like, you can also check the DB server is indeed running.

```bash
$ pgrep monetdbd
10287
```

Now, let's try it again with CTest this time. But first, you need to kill any running MonetDB server instance.

```bash
kill $(pgrep monetdbd)
```

Then, in the right build directory.

```bash
$ ctest
    Start 1: ctest-never-ends.real_db.setup

```

As you will see, CTest just hangs there until it gets killed or reaches the default timeout. No needs to mention, you can surely "resolve" this issue by setting the timeout to a small but not too small value so that the test can timeout faster. However, that is not an ideal solution, and is certainly not the solution we are proposing here in this article. So, let's find a better way.


# CTest Basics

In order to find the root cause of this problem, we need to understand why CTest seems to be hanging; Or put it in another way, when does CTest terminate?

**Normally, CTest ternimates when it can conclude that a test is finished.** Where, normal means the test that CTest is running is well functioning, and also there is no bugs in the particular version of CTest you are using, which could potentially cause it to hang.

Since, we already know that the test works fine with itself, and that kind of CTest bugs rarely happen in a stable release, so we can safely assume that they are not the cases, at least for now. Then, the question becomes, how can CTest conclude that a test is finished? Quoting from [\[1\]](#references):

> CTest waits for all of the following before concluding that a test is finished:
>
> - The child process exits
> - The child's stdout pipe closes
> - The child's stderr pipe closes
>
> If the child creates a grandchild and then exits or crashes, the grandchildren still hold on to the child's out/err pipes and can write to them. CTest will keep listening to the pipes until the test timeout is reached. The default test timeout is 1500 seconds (25 minutes), so it can appear to be a hang when in fact it will eventually finish.

This sounds pretty promising, as it points us to a good direction. So the next step would be figuring out if that is the case, and if so, finding out exactly what CTest is waiting for.


# Debugging

Let's go back to our problematic ctest, and first, check if the child and grandchild processes exit properly. While the test is still running, run the following command in another terminal:

```bash
$ pstree -p $(pgrep ctest)
ctest(11055)
```

As you can see, at this moment, **ctest** has no offspring processes. It basically rules out the possibility that CTest is waiting for its child process to exit.

Next, let's check the pipes, with the help of a little **bash** script.

```bash
{% include src/2019-07-26-ctest-that-never-ends/ls_pipe.sh %}
```

This one is slightly more complicated. Basically, this script can be used to find out the single pipe that is connecting the given two processes. In our case, we want to show the pipe that is between **ctest** and **monetdbd**. The output should be something similar to this:

```bash
$ ./ls_pipe.sh ctest monetdbd
COMMAND   PID  USER   FD      TYPE             DEVICE SIZE/OFF    NODE NAME
ctest    5806 yyang   11r     FIFO               0,13      0t0   52060 pipe
monetdbd 5810 yyang    9w     FIFO               0,13      0t0   52060 pipe
```

As you can see, at this point, **monetdbd** still holds on to the pipe **52060** and can **w**rite to it, while **ctest** keeps trying to **r**ead from the same pipe, thus preventing CTest from terminating.


# Solution

As it says[<sup>\[2\]</sup>](#references):

> Understanding your problem is half the solution.

And the rest half only requires you to change a few parameters of the **subprocess.call()** function in python. Here is the good code:

```python
{% include src/2019-07-26-ctest-that-never-ends/setup_test_db.py %}
```

Based on [The Python Standard Library Documentation of the subprocess module](https://docs.python.org/3/library/subprocess.html#subprocess.Popen), what that means is:

> PIPE indicates that a new pipe to the child should be created.
> With the default settings of None, no redirection will occur; the child’s file handles will be inherited from the parent.

And that's it. The problem solved.

You can find the full example on [my GitHub]({{site.github.repository_url}}/tree/master/_includes/src/2019-07-26-ctest-that-never-ends).


# Conclusion

The important take-away from this post is the strategy we used to debug the problem. Although we only present a solution for Python, it is likely that there are similar mechanisms in other programming languages, too, so the solution should be able to be extended to other domains easily.


# References

1. [ctest hangs when a test segfaults](https://gitlab.kitware.com/cmake/cmake/issues/16502)
1. [Understanding your problem is half the solution (actually the most important half)](https://www.henricodolfing.com/2018/05/understanding-your-problem-is-half.html) by Henrico Dolfing.
