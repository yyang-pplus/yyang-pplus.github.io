---
title: "The Second-System Effect: Challenged"
date: "2018-05-11"
categories:
  - "software-engineering"
tags:
  - "architect"
  - "design"
  - "engineering"
  - "frederick-brooks"
  - "how-not-to-be-wrong"
  - "ibm"
  - "mathematics"
  - "programming"
  - "software"
  - "stretch"
  - "the-mythical-man-month"
  - "the-second-system-effect"
---

The second-system effect is a well-known buzz word in the world of software engineering. As many years after it was first described by Frederick Brooks in his classic [The Mythical Man-Month](#the-reference), people are still talking about it. As you can see from my previous posts, the definition of the second-system effect is rather stricter than people think. Then one question raises naturally. Is it even practical to identify this effect from real world software projects? Even further, is it a real thing?

Let's find out. Shall we?

This is my final post of the second-system effect series. I'd highly recommend you to check out the first two.

{% include second-system-effect-navigation.html %}


# The failed first project

As I have already mentioned briefly in [my previous post]({{page.previous.previous.url}}), "spare and clean" doesn't necessarily means success. Projects fail, and most of them do fail, for different reasons. For example, one project may fail because it runs out of time or budget, or both, regardless how "spare and clean" the design is.[<sup>\[2\]</sup>](#the-reference)

As my personal experience, my first project was totally a failure. I was working on a complex system. It is true that I was desperate to show my capability, so I wanted my design to be perfect. I come up with one design, which I thought was "spare and clean", and went to the design review with it. The review went well. The reviewers said good things about my design and added a few comments for improvement. While addressing those comments after the meeting, I learned more about the system, and I caught the light of a totally different but better, at least I thought, design. I chose to abandon the original design and started to work on the new one. Then things start to repeat in this way: design review, improvement, new design, design review again. And this pattern repeated a couple times until I settled with the final one. Of course, my final design is still "spare and clean", but guess what, I missed the original due day, and the whole projects had to be delayed.


# The obvious solution

Again, my point is that a "spare and clean" design doesn't mean the success of the project, it even doesn't means everything for a design itself. By everything I mean all the things that characterize a good design. If it is, then there exists an obvious solution to the second-system effect. Before I talk about my solution, let's first review Dr. Brooks' solution, mentioned in his article[<sup>\[1\]</sup>](#the-reference).

> How does the project manger avoid the second-system effect? By insisting on a senior architect who has at least two system under his belt.

Well, this seems like a nonstarter solution for a long run. But in fact, it may work for a team. Remember, the second-system are concerned about the experience of the majority designers. So, the project manager can still insist on a team that constitute mostly with senior architects, and still have a few pupils to gain experience.

Now we can talk about my solution. It's simple. The project manager should just always hire new architects who has no previous experience with similar systems, then fire them or put them on another system that they are not familiar with. Isn't this just what some companies are practicing?

Apparently, my solution has several advances over Dr. Brooks'. First, it is definitely not a nonstarter with all the fresh graduates from schools each year. Second, it surely will resolve the second-system effect completely, by avoid the second systems at the first place. Third, it works not only for a team but also for a single architect.

Will others start to follow my advise? I doubt. Why? Because, again, "spare and clean" doesn't mean everything. One second system may not be that "spare and clean", but it can still turn out to be a successful project. Just like, one first system that is "spare and clean", and can still fail for various reasons. The second-system effect doesn't imply that second systems are relatively more likely to fail compare to their first ones.

By the way, I am actually with Dr. Brooks that project managers should insist on more senior architects on the team, not to avoid the second-system effect, but to guide the young designers to avoid obvious mistakes on their first system, like mine.


# The outsiders

The second-system effect doesn't exist for the outsiders, who don't have internal knowledge about the system.

The second system effect is emphasized on the experience of the majority of architects involved in the design of the second system. The problem is how can the outsiders know how much experience the majority architects do have. Sure, it is not entirely impossible, but still it is hard.

One interesting example comes from the experience when I read the second-system effect article. In the article, Dr. Brooks give a few IBM projects as examples of second systems. To be honest, I have no idea what those projects are and I have to trust his words completely. Like the Stretch example in the article[<sup>\[1\]</sup>](#the-reference):

> Consider as a stronger case the architecture, implementation, and even the realization of the Stretch computer, an outlet for the pent-up inventive desires of many people, and a second system for most of them.

That looks reasonable, but something a little bit confusing follows almost immediately[<sup>\[1\]</sup>](#the-reference).

> Operating System/360 was the second system for most of its designers. Groups of its designers came from building the 1410-7010 disk operating system, the Stretch operating system, the Project Mercury real-time system, and IBSYS for the 7090. Hardly anyone had experience with two previous operating system.

Wait a second, if Stretch is already a second system, shouldn't Operating System/360 be their third system? Sure, there certainly are ways to explain it properly. However, my point is, the second systems exist for the outsiders only when the insiders said they are.


# The insiders

Even for the insiders, it is still hard to tell if the second-system effect really happened.

"Spare and clean" is subjective. As a programmer myself, I have to constantly suppress the eager to rewrite every single line of the codes that someone else wrote. Even if that someone may just be the three months younger version of me. My point is, something feels "spare and clean" for you, may not be felt the same way for the others. Something feels "spare and clean" for now, may not make you feel the same way several months later. And I think that is a good thing. Because that probably means you have grown up.

Besides, do you really know the other members of your team? Especially, do you really know their previous design experience? Do you really know if the previous projects your teammates has worked on are the same class as your current one?


# The illusion

It is interesting that a similar phenomenon of the second-system effect also exist in other disciplines. As it mentioned in [How Not to Be Wrong<sup>\[3\]</sup>](#the-reference):

> Why is the second novel by a breakout debut writer, or the second album by an explosively popular band, so seldom as good as the first?

And the answer follows immediately in the book[<sup>\[3\]</sup>](#the-reference):

> It's because artistic success is an amalgam of talent and fortune, like everything else in life, and thus subject to regression to the mean.

After all, is it possible that the second-system effect is a phenomenon that just happen randomly, in which the designer got lucky with his first work and not so lucky with the second one?


# The reference

1. [Mythical Man-Month: Essays on Software Engineering, Anniversary Edition (2nd Edition) (#ad)](https://www.amazon.com) by Frederick Brooks.
1. [Why Software Projects Fail and How to Make Them Succeed](https://www.projectsmart.co.uk/why-software-projects-fail.php) by Duncan Haughey.
1. [How Not to Be Wrong: The Power of Mathematical Thinking (#ad)](https://www.amazon.com) by Jordan Ellenberg.
